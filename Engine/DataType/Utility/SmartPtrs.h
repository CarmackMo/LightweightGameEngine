#pragma once
#include <functional>
#include <utility>
#include <cassert>

using namespace std;

namespace Engine
{
/* Forwared declaration */
template <class T> class SmartPtr;
template <class T> class WeakPtr;



/* TODO: implement for weak pointer and implement in .cpp file
	* @brief Empty SmartPtr will not create RefCount object. Hence the RefCount will only
	*		  be created when the object that is going to be managed is onwed by at least
	*		  one SmartPtr. Therefore, the initial value of "smartCount" should be 1. */
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


	inline void IncSmartRef()
	{
		smartCount++;
	}

	inline void IncWeakRef()
	{
		weakCount++;
	}

	inline void DecSmartRef()
	{
		smartCount--;
		if (smartCount == 0)
		{
			Destroy();
			DecWeakRef();
		}
	}

	inline void DecWeakRef()
	{
		weakCount--;
		if (weakCount == 0)
		{
			DeleteThis();
		}
	}

	inline unsigned long GetSmartCount()
	{
		return smartCount;
	}

	inline unsigned long GetWeakCount()
	{
		return weakCount;
	}
};



template <class T>
class RefCount : public RefCountBase
{
private:
	T* ptr;
	/* TODO: */
	function<void(T*)> deleter;

	void Destroy() override
	{
		if (deleter != nullptr)
			deleter(ptr);
		else
			delete ptr;
	}

	void DeleteThis() override
	{
		delete this;
	}

public:
	explicit RefCount(T* ptr, function<void(T*)> deleter = nullptr) :
		RefCountBase(), ptr(ptr), deleter(deleter) 
	{}
};



/* TODO: */
template <class T>
class PtrBase
{
protected:
	T* ptr = nullptr;
	RefCountBase* refCount = nullptr;

	PtrBase() = default;
	~PtrBase() = default;

	/* TODO */
	void StandardConstruct(T* ptr, function<void(T*)> deleter = nullptr)
	{
		this->ptr = ptr;
		this->refCount = new RefCount<T>(ptr, deleter);
	}

	/* TODO: @brief Copy constructor, using shallow copy to data, since data is shared
	 * by SmartPtr. Assume class "U" is convertible to class "T" */
	template <class U>
	void CopyConstruct(const SmartPtr<U>& other)
	{
		other.IncSmartRef();
		this->ptr = other.ptr;
		this->refCount = other.refCount;
	}

	/* TODO: @brief Move constructor, using shallow copy to copy the pointer itself
	 * Assume class "U" is convertible to class "T" */
	template <class U>
	void MoveConstruct(PtrBase<U>&& other)
	{
		this->ptr = other.ptr;
		this->refCount = other.refCount;

		other.ptr = nullptr;
		other.refCount = nullptr;
	}

	/* TODO: */
	template <class U>
	void AliasConstruct(const SmartPtr<U>& other, T* ptr)
	{
		other.IncSmartRef();
		this->ptr = ptr;
		this->refCount = other.refCount;
	}

	/* TODO: */
	template <class U>
	void AliasMoveConstruct(SmartPtr<U>&& other, T* ptr)
	{
		this->ptr = other.ptr;
		this->refCount = other.refCount;

		other.ptr = nullptr;
		other.refCount = nullptr;
	}

	/* TODO: */
	template <class U>
	void WeakConstruct(const PtrBase<U>& other)
	{
		other.IncWeakRef();
		this->ptr = other.ptr;
		this->refCount = other.refCount;
	}

	/* TODO: @brief: If the given WeakPtr is empty or is already expired, return false. Return
	 *	true if construct success. */
	template <class U>
	bool ConstructFromWeak(const WeakPtr<U>& other)
	{
		if (other.refCount != nullptr && other.Expired() != true)
		{
			other.IncSmartRef();
			this->ptr = other.ptr;
			this->refCount = other.refCount;
			return true;
		}
		return false;
	}

	/* TODO: */
	inline void IncSmartRef() const
	{
		if (refCount != nullptr)
			refCount->IncSmartRef();
	}
	/* TODO: */
	inline void DecSmartRef()
	{
		if (refCount != nullptr)
			refCount->DecSmartRef();
	}
	/* TODO: */
	inline void IncWeakRef() const
	{
		if (refCount != nullptr)
			refCount->IncWeakRef();
	}
	/* TODO: */
	inline void DecWeakRef()
	{
		if (refCount != nullptr)
			refCount->DecWeakRef();
	}

	/* TODO: */
	inline T* Get() const
	{
		return ptr;
	}

	/* TODO: */
	inline void SwapPtr(PtrBase<T>& other)
	{
		std::swap(this->ptr, other.ptr);
		std::swap(this->refCount, other.refCount);

		
		//T* tempPtr = this->ptr;
		//RefCount<T>* tempRef = this->refCount;

		//this->ptr = other.ptr;
		//this->refCount = other.refCount;

		//other.ptr = tempPtr;
		//other.refCount = tempRef;
	}


public:
	friend class PtrBase;
	friend class SmartPtr<T>;

	PtrBase(const PtrBase<T>&) = delete;
	PtrBase& operator= (const PtrBase<T>&) = delete;

	/* TODO: */
	inline unsigned long GetSmartCount() const 
	{
		return refCount != nullptr ? refCount->GetSmartCount() : 0;
	}

};




/**
* @brief TODO: smart pointer...
*
* @remarks Does not allow reference count exists when object instance is not exist. When
*			a new smart pointer is created after the object instance is deleted, it will
*			generate potential harzard if the smart pointer reference count is increased.
*			Therefore, when creating a new smart pointer by copying other smart pointer /
*			weak pointer instance, if the object instance is not exsit, its corresponding
*			reference must not exsit as well.
*/
template <class T>
class SmartPtr : public PtrBase<T>
{
private:

public:
	friend class WeakPtr<T>;

	/* TODO: @brief Constructs a smart pointer with no managed object, i.e. empty smart
	 *	pointer. A reference count object will be constructed but the reference count 
	 *	will be 0. */
	inline SmartPtr();
	inline SmartPtr(nullptr_t);
	/* TODO: @brief Constructs a smart pointer with input pointer as the pointer to the 
	 *	managed object. If a nullptr is given, construct an empty smart pointer instead. 
	 *  It is user's responsibility to make sure not to constructor a smart pointer using this funciton if the target object is
	 *  already owned by another smart pointer. */
	inline SmartPtr(T* ptr);
	/* TODO: @brief Provides a mechanism for users to specify a customized deleter for
	 *	data types, such as array types, that cannot be deleted using the regular "delete" 
	 *	expression. 
	 *  It is user's responsibility to make sure not to constructor a smart pointer using this funciton if the target object is
	 *  already owned by another smart pointer. */
	inline SmartPtr(T* ptr, function<void(T*)> deleter);

	/* TODO: @brief Aliasing constructor. See following link for more detail: 
	 *	https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
	 *	https://stackoverflow.com/questions/27109379/what-is-shared-ptrs-aliasing-constructor-for */
	template<class U>
	inline SmartPtr(const SmartPtr<U>& other, T* ptr);
	template<class U>
	inline SmartPtr(SmartPtr<U>&& other, T* ptr);

	/* TODO: @brief Copy Constructor */
	inline SmartPtr(const SmartPtr<T>& other);
	template<class U> 
	inline SmartPtr(const SmartPtr<U>& other); 	/* Copy constructor for class inheritance */


	/* TODO: @brief Move Constructor */
	inline SmartPtr(SmartPtr<T>&& other);
	template<class U>
	inline SmartPtr(SmartPtr<U>&& other);

	/* TODO: */
	template <class U>
	inline SmartPtr(const WeakPtr<U>& other)
	{
		bool res = this->ConstructFromWeak(other);
		assert(res == true);
	}

	/* TODO: */
	inline ~SmartPtr();



	/* TODO: */
	inline bool IsUnique() const
	{
		return this->GetSmartCount() == 1;
	}

	/* TODO: */
	inline void Swap(SmartPtr<T>& other)
	{
		this->SwapPtr(other);
	}

	/* TODO: @brief Release resource and convert this instance to empty SmartPtr object. */
	inline void Reset()
	{
		SmartPtr().Swap(*this);
	}
	/* TODO: @brief Release original pointer and decrement pointer's reference count of 
	 *	this instance. And take ownership of the new pointer "ptr". */
	template <class U>
	inline void Reset(U* ptr)
	{
		SmartPtr(ptr).Swap(*this);
	}
	/* TODO: @brief Release original pointer and decrement pointer's reference count of
	 *	this instance. And take ownership of the new pointer "ptr" with deleter */
	template <class U>
	inline void Reset(U* ptr, function<void(U*)> deleter)
	{
		SmartPtr(ptr, deleter).Swap(*this);
	}


	/* Access operators */
	inline T* operator->();
	inline T& operator*();

	/* Comparision operators */
	inline operator bool();

	inline bool operator==(std::nullptr_t);
	inline bool operator!=(std::nullptr_t);

	template <class U>
	inline bool operator==(const SmartPtr<U>& other);
	template <class U>
	inline bool operator!=(const SmartPtr<U>& other);

	/* TODO: Assignment operators */
	inline SmartPtr<T>& operator=(const SmartPtr<T>& other)
	{
		SmartPtr(other).Swap(*this);
		return *this;
	}
	template<class U>
	inline SmartPtr<T>& operator=(const SmartPtr<U>& other)
	{
		SmartPtr(other).Swap(*this);
		return *this;
	}

	inline SmartPtr<T>& operator=(SmartPtr<T>&& other)
	{
		SmartPtr(std::move(other)).Swap(*this);
		return *this;
	}
	template<class U>
	inline SmartPtr<T>& operator=(SmartPtr<U>&& other)
	{
		SmartPtr(std::move(other)).Swap(*this);
		return *this;
	}
};


/**
* @brief TODO: weak pointer...
*
* @remarks Allow reference count exsits when object instance is exist. Weak reference
*			count records the amount of "observer" to the object instance no matter it
*			exists or not. Therefore, when creating a new weak pointer by copying other
*			smart pointer / weak pointer instance, reference count is allowed to be existed
*			even if the object instance is not exist. But in most cases, smart pointer's
*			reference count will not exist if its object instance is null. The weak pointer
*			created from this smart pointer will not have reference as well.
*/
template<class T>
class WeakPtr : public PtrBase<T>
{
private:

public:
	friend class SmartPtr<T>;

	inline WeakPtr() {}

	/* TODO: Copy constructor, copy construct from another weak pointer */
	inline WeakPtr(const WeakPtr<T>& other)
	{
		this->WeakConstruct(other);
	}
	template<class U> 
	inline WeakPtr(const WeakPtr<U>& other)
	{
		this->WeakConstruct(other);
	}
	/* TODO: Copy constructor, copy construct from Smart pointer */
	template <class U>
	inline WeakPtr(const SmartPtr<U>& other)
	{
		this->WeakConstruct(other);
	}

	/* TODO: Move constructor */
	inline WeakPtr(WeakPtr<T>&& other)
	{
		this->MoveConstruct(std::move(other));
	}
	template <class U>
	inline WeakPtr(WeakPtr<U>&& other)
	{
		this->MoveConstruct(std::move(other));
	}
	
	/* TODO: */
	inline ~WeakPtr()
	{
		this->DecWeakRef();
	}

	/* TODO: */
	inline void Reset()
	{
		WeakPtr().Swap(*this);
	}
	/* TODO: */
	inline void Swap(WeakPtr<T>& other)
	{
		this->SwapPtr(other);
	}
	/* TODO: */
	inline bool Expired() const
	{
		return this->GetSmartCount() == 0;
	}


	/* TODO: Comparision operators */
	inline operator bool()
	{
		return this->Get() != nullptr;
	}

	inline bool operator==(std::nullptr_t)
	{
		return this->Get() == nullptr;
	}
	inline bool operator!=(std::nullptr_t)
	{
		return this->Get() != nullptr;
	}

	inline bool operator==(const WeakPtr<T>& other)
	{
		return this->Get() == other.Get();
	}
	inline bool operator!=(const WeakPtr<T>& other)
	{
		return this->Get() == other.Get();
	}

	/* TODO: Assignment operators */
	inline WeakPtr<T>& operator=(const WeakPtr<T>& other)
	{
		WeakPtr(other).Swap(*this);
		return *this;
	}
	template <class U>
	inline WeakPtr<T>& operator=(const WeakPtr<U>& other)
	{
		WeakPtr(other).Swap(*this);
		return *this;
	}

	inline WeakPtr<T>& operator=(WeakPtr<T>&& other)
	{
		WeakPtr(std::move(other)).Swap(*this);
		return *this;
	}
	template <class U>
	inline WeakPtr<T>& operator=(WeakPtr<U>&& other)
	{
		WeakPtr(std::move(other)).Swap(*this);
		return *this;
	}

	template <class U>
	inline WeakPtr<T>& operator=(const SmartPtr<U>& other)
	{
		WeakPtr(other).Swap(*this);
		return *this;
	}
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

#endif

}//Namespace Memory
}//Namespace Engine
