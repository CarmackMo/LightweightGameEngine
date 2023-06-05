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
	/* TODO: */
	function<void(T*)> deleter;

	/* TODO: just for test */
	std::shared_ptr<int> temp = std::shared_ptr<int>();
	std::default_delete<int> dt;

public:
	friend class WeakPtr<T>;

	/* TODO: @brief Constructs a smart pointer with no managed object, i.e. empty smart
	 *	pointer. A reference count object will be constructed but the reference count 
	 *	will be 0. */
	inline SmartPtr();
	/* TODO: @brief Constructs a smart pointer with input pointer as the pointer to the 
	 *	managed object. If a nullptr is given, construct an empty smart pointer instead. */
	inline SmartPtr(T* ptr);
	/* TODO: @brief Provides a mechanism for users to specify a customized deleter for
	 *	data types, such as array types, that cannot be deleted using the regular "delete" 
	 *	expression. */
	inline SmartPtr(T*, function<void(T*)> deleter);
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
