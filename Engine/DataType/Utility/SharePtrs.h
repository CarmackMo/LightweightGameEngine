#pragma once
#include <functional>

/* TODO: just for test */
#include <memory>

using namespace std;

namespace Engine
{
/* Forwared declaration */
class ReferenceCount;
template <class T> class SmartPtr;
template <class T> class WeakPtr;



class ReferenceCount
{
public:
	unsigned long smartRefCount;
	unsigned long weakRefCount;

	inline ReferenceCount(unsigned long smartRef = 0, unsigned long weakRef = 0);
	inline ~ReferenceCount();
};


/* TODO: implement for weak pointer and implement in .cpp file 
 * @brief Empty SmartPtr will not create RefCount object. Hence the RefCount will only
 *		  be created when the object that is going to be managed is onwed by at least 
 *		  one SmartPtr. Therefore, the initial value of "smartCount" should be 1. */
template <class T>
class RefCount
{
private:
	T* ptr;
	/* TODO: */
	function<void(T*)> deleter;

	unsigned long smartCount = 1;
	unsigned long weakCount = 1;

	void Destroy()
	{
		if (deleter != nullptr)
			deleter(ptr);
		else
			delete ptr;
	}

	void DeleteThis()
	{
		delete this;
	}


public:
	inline RefCount(T* ptr, function<void(T*)> deleter = nullptr) :
		ptr(ptr),
		deleter(deleter)
	{}

	/* @breif Prevent instantiating RefCount by copy constructor and assignment
	 *		  operation. */
	inline RefCount(const RefCount&) = delete;
	inline RefCount& operator= (const RefCount&) = delete;

	inline ~RefCount() = default;

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



/* TODO: */
template <class T>
class PtrBase
{
protected:
	T* ptr = nullptr;
	RefCount* refCount = nullptr;

	/* TODO */
	void StandardConstruct(T* ptr, function<void(T*)> deleter = nullptr)
	{
		this->ptr = ptr;
		this->refCount = new RefCount<T>(ptr, deleter);
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
	void AliasMoveConstruct(const SmartPtr<U>&& other, T* ptr)
	{
		this->ptr = other.ptr;
		this->refCount = other.refCount;

		other.ptr = nullptr;
		other.refCount = nullptr;
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
	void MoveConstruct(const SmartPtr<U>&& other)
	{
		this->ptr = other.ptr;
		this->refCount = other.refCount;

		other.ptr = nullptr;
		other.refCount = nullptr;
	}

public:
	PtrBase() = default;
	~PtrBase() = default;



	/* TODO: */
	inline unsigned long GetSmartCount()
	{
		return refCount != nullptr ? refCount->GetSmartCount();
	}

	/* TODO: */
	inline void IncSmartRef()
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
	inline void IncWeakRef()
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
class SmartPtr : PtrBase<T>
{
private:
	//T* objectPtr;
	//ReferenceCount* refCount;

	/* TODO: just for test */
	std::shared_ptr<int> temp = std::shared_ptr<int>();
	std::default_delete<int> dt;

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
	inline SmartPtr(T*, function<void(T*)> deleter);

	/* TODO: @brief Aliasing constructor. See following link for more detail: 
	 *	https://en.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr
	 *	https://stackoverflow.com/questions/27109379/what-is-shared-ptrs-aliasing-constructor-for */
	template<class U>
	inline SmartPtr(const SmartPtr<U>& other, T* ptr);
	template<class U>
	inline SmartPtr(const SmartPtr<U>&& other, T* ptr);

	/* TODO: @brief Copy Constructor */
	inline SmartPtr(const SmartPtr<T>& other);
	template<class U> 
	inline SmartPtr(const SmartPtr<U>& other); 	/* Copy constructor for class inheritance */


	/* TODO: @brief Move Constructor */
	inline SmartPtr(const SmartPtr<T>&& other);
	template<class U>
	inline SmartPtr(const SmartPtr<U>&& other);

	/* TODO: */
	inline SmartPtr(const WeakPtr<T>& other);

	/* TODO: */
	inline ~SmartPtr() = default;




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
	SmartPtr<T>& operator=(const SmartPtr<T>& other);
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
class WeakPtr
{
private:
	T* objectPtr;
	ReferenceCount* refCount;

public:
	friend class SmartPtr<T>;

	inline WeakPtr();
	inline WeakPtr(const WeakPtr<T>& other);
	inline WeakPtr(const SmartPtr<T>& other);
	template<class U> inline WeakPtr(const WeakPtr<U>& other);		/* Copy constructor for class inheritance */

	inline ~WeakPtr();

	/* Comparision operators */
	inline operator bool();

	inline bool operator==(std::nullptr_t);
	inline bool operator!=(std::nullptr_t);

	inline bool operator==(const WeakPtr<T>& other);
	inline bool operator!=(const WeakPtr<T>& other);

	/* Assignment operators */
	WeakPtr<T>& operator=(const WeakPtr<T>& other);
};


}//Namespace Engine
