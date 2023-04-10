#pragma once
#include "Dependency.h"


using namespace std;

namespace Engine
{
/* Classes forwared declaration */
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
class SmartPtr
{
private:
	T* objectPtr;
	ReferenceCount* refCount;

public:
	friend class WeakPtr<T>;

	inline SmartPtr();
	inline SmartPtr(T* ptr);
	inline SmartPtr(const SmartPtr<T>& other);
	inline SmartPtr(const WeakPtr<T>& other);
	template<class U> inline SmartPtr(const SmartPtr<U>& other); 	/* Copy constructor for class inheritance */

	inline ~SmartPtr();

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
	SmartPtr<T>& operator=(const SmartPtr<T>& other)
	{
		if (refCount != nullptr && --(refCount->smartRefCount) == 0)
		{
			delete objectPtr;

			if (refCount->weakRefCount == 0)
				delete refCount;
		}

		objectPtr = other.objectPtr;
		refCount = other.refCount;

		if (objectPtr != nullptr)
			refCount->smartRefCount++;
		else
			refCount = nullptr;

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
	WeakPtr<T>& operator=(const WeakPtr<T>& other)
	{
		if (refCount != nullptr)
		{
			refCount->weakRefCount--;

			if (refCount->smartRefCount == 0 && refCount->weakRefCount == 0)
			{
				delete refCount;
			}
		}

		objectPtr = other.objectPtr;
		refCount = other.refCount;

		if (refCount != nullptr)
			refCount->weakRefCount++;

		return *this;
	}
};


#include "Util.inl"
}