#pragma once
#include <functional>
#include <utility>
#include <cassert>

using namespace std;

namespace Engine
{
/* Forwared declaration */
class RefCountBase;
template <class T> class RefCount;

template <class T> class PtrBase;
template <class T> class SmartPtr;
template <class T> class WeakPtr;



/**
 *	@brief `RefCount` serves as a control component of both `SmartPtr` and `WeakPtr`. In runtime, 
 *		   `RefCount` is a dynamically-allocated object that holds:
 *				- A pointer to the managed object;
 *				- A user-defined deleter;
 *				- The number of `SmartPtr` that own the managed object;
 *				- the number of `WeakPtr` that refer to the managed object.
 */
class RefCountBase
{
private:
	unsigned long smartCount = 1;
	unsigned long weakCount = 1;

	virtual void Destroy() = 0;
	virtual void DeleteThis() = 0;

public:
	inline RefCountBase() = default;
	inline ~RefCountBase() = default;

	/* @breif Prevent instantiating RefCount by copy constructor and assignment
	 *		  operation. */
	inline RefCountBase(const RefCountBase&) = delete;
	inline RefCountBase& operator= (const RefCountBase&) = delete;


	inline void IncSmartRef();

	inline void IncWeakRef();

	inline void DecSmartRef();

	inline void DecWeakRef();

	inline unsigned long GetSmartCount();

	inline unsigned long GetWeakCount();
};


template <class T>
class RefCount : public RefCountBase
{
private:
	T* ptr;
	function<void(T*)> deleter;

	void Destroy() override;

	void DeleteThis() override;

public:
	explicit RefCount(T* ptr, function<void(T*)> deleter = nullptr);
};


/**
 *	@brief `PtrBase` is the base class for both `SmartPtr` and `WeakPtr`. It serves as a API 
 *		   contract for `SmartPtr` and `WeakPtr`, defining certain rules for their member 
 *		   function implementations.
 */
template <class T>
class PtrBase
{
protected:
	T* ptr = nullptr;
	RefCountBase* refCount = nullptr;

	PtrBase() = default;
	~PtrBase() = default;

	void StandardConstruct(T* ptr, function<void(T*)> deleter = nullptr);

	/* @brief Performs copy-construct for SmartPtr. Using shallow copy to data, since data is 
	 *		  shared by SmartPtrs owning the same object. It is the responsibility of the users 
	 *		  to ensure "U*" is implicitly convertible to "T*" */
	template <class U>
	void CopyConstruct(const SmartPtr<U>& other);

	/* @brief Performs move-constructor for SmartPtr and WeakPtr. Using shallow copy to copy the
	 *		  pointer itself. It is the responsibility of the users to ensure "U*" is implicitly 
	 *		  convertible to "T*" */
	template <class U>
	void MoveConstruct(PtrBase<U>&& other);

	/* @brief Performs aliasing-constructor for SmartPtr. It is the responsibility of the users 
	 *		  to ensure "U*" is implicitly convertible to "T*" */
	template <class U>
	void AliasConstruct(const SmartPtr<U>& other, T* ptr);

	/* @brief Performs aliasing-move-constructor for SmartPtr. It is the responsibility of the 
	 *		  users to ensure "U*" is implicitly convertible to "T*" */
	template <class U>
	void AliasMoveConstruct(SmartPtr<U>&& other, T* ptr);

	/* @brief Performs copy-construct for WeakPtr. Using shallow copy to data, since WeakPtr 
	 *		  holds non-owning reference to share data that managed by SmartPtr. It is the 
	 *		  responsibility of the users to ensure "U*" is implicitly convertible to "T*" */
	template <class U>
	void WeakConstruct(const PtrBase<U>& other);

	/* @brief: Performs copy-construct from WeakPtr to SmartPtr. If the given WeakPtr is empty 
	 *		   or is already expired, return false. Return true if construct success. */
	template <class U>
	bool ConstructFromWeak(const WeakPtr<U>& other);


	inline void IncSmartRef() const;

	inline void DecSmartRef();

	inline void IncWeakRef() const;

	inline void DecWeakRef();


	/* @brief Return the pointer of the managed object */
	inline T* Get() const;

	/* @brief Swaps the managed objects. */
	inline void SwapPtr(PtrBase<T>& other);


public:
	friend class PtrBase;
	friend class SmartPtr<T>;

	PtrBase(const PtrBase<T>&) = delete;
	PtrBase& operator= (const PtrBase<T>&) = delete;

	inline unsigned long GetSmartCount() const;

};


/**
 * @brief `SmartPtr` retains shared ownership of an object through a pointer. Several `SmartPtr` 
 *		  objects may own the same object. The managed object is destroyed and its memory 
 *		  deallocated when either of the following happens:
 *			- The last remaining `SmartPtr` owning the object is destroyed;
 *			- The last remaining `SmartPtr` owning the object is assigned another pointer via 
 *			  `operator=` or `Reset()`.
 */
template <class T>
class SmartPtr : public PtrBase<T>
{
public:
	friend class WeakPtr<T>;

	/* @brief Constructs a SmartPtr as the pointer to the managed object. If no input or a null 
	 *		  pointer is given, construct an empty SmartPtr instead. It is user's responsibility 
	 *		  to make sure not to constructor a SmartPtr using this constructor if the target 
	 *		  object is already owned by another smart pointer. */
	inline SmartPtr();
	inline SmartPtr(nullptr_t);
	inline SmartPtr(T* ptr);
	/* @brief Allow users to specify a customized deleter for data types (e.g. array type) that 
	 *		  cannot be deleted using regular "delete" expression. */
	inline SmartPtr(T* ptr, function<void(T*)> deleter);

	/* @brief Copy constructors. Constructs a SmartPtr which shares ownership of the object 
	 *		  managed by "other". If "other" manages no object, this instance manages no object
	 *		  either. Using shallow copy to copy pointers */
	inline SmartPtr(const SmartPtr<T>& other);
	template <class U> 
	inline SmartPtr(const SmartPtr<U>& other);

	/* @brief Aliasing constructor. Constructs a SmarPtr which shares ownership information with 
	 *		  the initial value of "other", but holds an unrelated and unmanaged pointer "ptr". 
	 *		  It is the responsibility of the users to make sure that "ptr" remains valid as long 
	 *		  as this SmartPtr exists */
	template <class U>
	inline SmartPtr(const SmartPtr<U>& other, T* ptr);
	template <class U>
	inline SmartPtr(SmartPtr<U>&& other, T* ptr);

	/* @brief Move constructors. Move-constructs a SmartPtr from "other". After the construction, 
	 *		  this instance contains a copy of the previous state of "other", "other" is empty and
	 *		  its stored pointer is null.  */
	inline SmartPtr(SmartPtr<T>&& other);
	template <class U>
	inline SmartPtr(SmartPtr<U>&& other);

	/* @brief Constructs a SmartPtr which shares ownership of the object managed by "other". It is
	 *		  the responsibility of the users to ensure "other" is a valid WeakPtr (i.e. "other" is
	 *		  neither empty nor the object it manages is deleted). */
	template <class U>
	inline SmartPtr(const WeakPtr<U>& other);

	inline ~SmartPtr();


	/* @brief Checks whether the managed object is managed only by the current SmartPtr instance. */
	inline bool IsUnique() const;

	/* @brief Swaps the managed objects. */
	inline void Swap(SmartPtr<T>& other);

	/* @brief Release resource and convert this instance to empty SmartPtr object. */
	inline void Reset();
	/* @brief Release original pointer and decrement pointer's reference count. And take ownership 
	 *		  of the new pointer "ptr". */
	template <class U>
	inline void Reset(U* ptr);
	/* @brief Release original pointer and decrement pointer's reference count. And take ownership 
	 *		  of the new pointer "ptr" with deleter */
	template <class U>
	inline void Reset(U* ptr, function<void(U*)> deleter);


	/* Access operators */
	inline T* operator->();
	inline T& operator*();

	/* Comparision operators */
	inline operator bool();

	inline bool operator==(std::nullptr_t);
	inline bool operator!=(std::nullptr_t);

	inline bool operator==(const SmartPtr<T>& other);
	inline bool operator!=(const SmartPtr<T>& other);

	/* Assignment operators */
	inline SmartPtr<T>& operator=(const SmartPtr<T>& other);
	template <class U>
	inline SmartPtr<T>& operator=(const SmartPtr<U>& other);

	inline SmartPtr<T>& operator=(SmartPtr<T>&& other);
	template<class U>
	inline SmartPtr<T>& operator=(SmartPtr<U>&& other);
};


/**
 * @brief WeakPtr is a smart pointer that holds a non-owning ("weak") reference to an object that is
 *		  managed by SmartPtr. It must be converted to SmartPtr in order to access the referenced 
 *		  object.
 *		  WeakPtr models temporary ownership: when an object needs to be accessed only if it exists, 
 *		  and it may be deleted at any time by someone else, WeakPtr is used to track the object, and 
 *		  it is converted to SmartPtr to assume temporary ownership. If the original SmartPtr is 
 *		  destroyed at this time, the object's lifetime is extended until the temporary SmartPtr is 
 *		  destroyed as well. 
 */
template<class T>
class WeakPtr : public PtrBase<T>
{
public:
	friend class SmartPtr<T>;

	/* @brief Stantard constructor. Constructs an empty WeakPtr */
	inline WeakPtr() {}

	/* @brief Copy constructors. Constructs a WeakPtr which shares ownership of the object
	 *		  managed by "other". If "other" manages no object, this instance manages no object
	 *		  either. Using shallow copy to copy pointers. */
	inline WeakPtr(const WeakPtr<T>& other);
	template <class U> 
	inline WeakPtr(const WeakPtr<U>& other);
	template <class U>
	inline WeakPtr(const SmartPtr<U>& other);

	/* @brief Move constructors. Move-constructs a WeakPtr instance from "Other". After this, 
	 *		  "other" is empty */
	inline WeakPtr(WeakPtr<T>&& other);
	template <class U>
	inline WeakPtr(WeakPtr<U>&& other);
	
	inline ~WeakPtr();


	/* @brief Checks whether the referenced object was already deleted */
	inline bool IsExpired() const;

	/* @brief Swaps the managed objects */
	inline void Swap(WeakPtr<T>& other);

	/* @brief Release resource, and convert this instance to empty WeakPtr object */
	inline void Reset();


	/* Comparision operators */
	inline operator bool();

	inline bool operator==(std::nullptr_t);
	inline bool operator!=(std::nullptr_t);

	inline bool operator==(const WeakPtr<T>& other);
	inline bool operator!=(const WeakPtr<T>& other);

	/* Assignment operators */
	inline WeakPtr<T>& operator=(const WeakPtr<T>& other);
	template <class U>
	inline WeakPtr<T>& operator=(const WeakPtr<U>& other);

	inline WeakPtr<T>& operator=(WeakPtr<T>&& other);
	template <class U>
	inline WeakPtr<T>& operator=(WeakPtr<U>&& other);

	template <class U>
	inline WeakPtr<T>& operator=(const SmartPtr<U>& other);
};





namespace Memory
{

/********************************* Unit tests **************************************/
#if defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include "Debugger.h"

using namespace Engine::Debugger;

inline void SmartPtrUnitTest()
{
	/* Preparation */
	class Member
	{
	public:
		int memVal = 1;
		Member() {}
		~Member() { DEBUG_PRINT("Destroying 'Member' class!\n"); }
	};

	class Base
	{
	public:
		int basVal = 2;
		Member* member;

		Base() { void* temp = malloc(sizeof(Member)); member = new(temp) Member(); }
		~Base() { delete member; DEBUG_PRINT("Destroying 'Base' class!\n"); }
	};

	class Derive : public Base
	{
	public:
		int derVal = 3;
		Derive() {}
		~Derive() { DEBUG_PRINT("Destroying 'Derive' class!\n"); }
	};

	int* num = (int*)malloc(sizeof(int));
	int* arr = (int*)malloc(sizeof(int) * 10);
	function<void(int*)> deleter = [](int* p) { free(p); };


	/* Test for SmartPtr standard constructor and Reset() */
	SmartPtr<int> ptr0;
	SmartPtr<int> ptr1 = SmartPtr<int>(nullptr);
	SmartPtr<int> ptr2 = SmartPtr<int>(num);
	ptr2.Reset();
	SmartPtr<int> ptr3 = SmartPtr<int>(arr, deleter);
	ptr3.Reset();

	/* Test for SmartPtr alias constructor */
	void* temp = malloc(sizeof(Base));
	Base* base = new(temp) Base();
	SmartPtr<Base> ptr4 = SmartPtr<Base>(base);
	SmartPtr<Member> ptr5 = SmartPtr<Member>(ptr4, base->member);
	ptr4.Reset();
	ptr5.Reset();

	/* Test for SmartPtr copy constructor */
	temp = malloc(sizeof(Derive));
	Derive* derive = new(temp) Derive();
	SmartPtr<Derive> ptr6 = SmartPtr<Derive>(derive);
	SmartPtr<Base> ptr7 = SmartPtr<Base>(ptr6);

	/* Test for SmartPtr move constructor and move assignment */
	ptr4 = SmartPtr<Base>(ptr6);

	/* Test for comparison operators */
	bool res = ptr4 == ptr7;
	res = ptr3 == nullptr;
	res = ptr3 != nullptr;

	/* Test for Reset(T* ptr) */
	temp = malloc(sizeof(Base));
	base = new(temp) Base();
	ptr4.Reset(base);

	/* Test for accessor operators */
	ptr6->derVal = 9;
	(*ptr7).basVal = 8;
}


inline void WeakPtrUnitTest()
{
	class Base
	{
	public:
		friend class Derive;

		int basVal = 2;

		Base() { }
		~Base() { printf("Destroying 'Base' class!\n"); }
	};

	class Derive : public Base
	{
	public:
		int derVal = 3;
		Derive() {}
		~Derive() { printf("Destroying 'Derive' class!\n"); }
	};

	void* temp = malloc(sizeof(Derive));
	Derive* derive = new(temp) Derive();

	SmartPtr<Derive> sPtr1 = SmartPtr<Derive>(derive);
	SmartPtr<Base> sPtr2 = SmartPtr<Base>(sPtr1);

	/* Test for WeakPtr copy constructor */
	WeakPtr<Derive> wPtr1 = WeakPtr<Derive>(sPtr1);
	WeakPtr<Base> wPtr2 = WeakPtr<Base>(wPtr1);
	WeakPtr<Base> wPtr3 = WeakPtr<Base>(sPtr1);

	/* Test for WeakPtr default constructor and move constructor */
	WeakPtr<Base> wPtr4;
	wPtr4 = WeakPtr<Base>(wPtr1);

	/* Test for WeakPtr Rest() and assignment operators */
	wPtr4.Reset();
	wPtr4 = wPtr1;
	wPtr4 = sPtr1;	// Weak reference count should still be 5

	/* Test for creating SmartPtr from WeakPtr */
	SmartPtr<Base> sPtr3 = SmartPtr<Base>(wPtr1);

	/* Test for comparison operators */
	bool res;
	res = wPtr4 == wPtr2;
	res = wPtr4 == wPtr1;
	res = wPtr4 == nullptr;
	res = wPtr4 != nullptr;

	/* Test for Expire() */
	res = wPtr4.IsExpired();
	res = sPtr3.IsUnique();
	sPtr1.Reset();
	sPtr2.Reset();
	res = sPtr3.IsUnique();
	sPtr3.Reset();
	res = wPtr4.IsExpired();
}

#endif

}//Namespace Memory
}//Namespace Engine
