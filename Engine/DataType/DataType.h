#pragma once
#include "Dependency.h"
#include "Vector.h"
#include "Matrix.h"
#include "Util.h"

using namespace std;

namespace Engine
{
/* Classes forwared declaration */
class ReferenceCount;
template<class T> class SmartPtr;
template<class T> class WeakPtr;
template<class T> class Singleton;
class GameObject;



//class ReferenceCount
//{
//public:
//	unsigned long smartRefCount;
//	unsigned long weakRefCount;
//
//	inline ReferenceCount(unsigned long smartRef = 0, unsigned long weakRef = 0);
//	inline ~ReferenceCount();
//};


///**
//	* @brief TODO: smart pointer...
//	*
//	* @remarks Does not allow reference count exists when object instance is not exist. When
//	*			a new smart pointer is created after the object instance is deleted, it will
//	*			generate potential harzard if the smart pointer reference count is increased.
//	*			Therefore, when creating a new smart pointer by copying other smart pointer /
//	*			weak pointer instance, if the object instance is not exsit, its corresponding
//	*			reference must not exsit as well.
//	*/
//template <class T>
//class SmartPtr
//{
//private:
//	T* objectPtr;
//	ReferenceCount* refCount;
//
//public:
//	friend class WeakPtr<T>;
//
//	inline SmartPtr() :
//		objectPtr(nullptr),
//		refCount(nullptr)
//	{ }
//
//	inline SmartPtr(T* ptr) :
//		objectPtr(ptr),
//		refCount(new ReferenceCount(1, 0))
//	{ }
//
//	inline SmartPtr(const SmartPtr<T>& other) :
//		objectPtr(other.objectPtr),
//		refCount(other.refCount)
//	{
//		if (objectPtr == nullptr)
//			refCount = nullptr;
//		else
//			refCount->smartRefCount++;
//	}
//
//	template<class U>
//	inline SmartPtr(const SmartPtr<U>& other) :
//		objectPtr(other.objectPtr),
//		refCount(other.refCount)
//	{
//		if (objectPtr == nullptr)
//			refCount = nullptr;
//		else
//			refCount->smartRefCount++;
//	}
//
//	inline SmartPtr(const WeakPtr<T>& other) :
//		objectPtr(other.objectPtr),
//		refCount(other.refCount)
//	{
//		if (objectPtr == nullptr)
//			refCount = nullptr;
//		else
//			refCount->smartRefCount++;
//	}
//
//	inline ~SmartPtr()
//	{
//		if (refCount != nullptr)
//		{
//			refCount->smartRefCount--;
//
//			if (refCount->smartRefCount == 0)
//			{
//				delete objectPtr;
//
//				if (refCount->weakRefCount == 0)
//					delete refCount;
//			}
//		}
//	}
//
//	inline T* operator->() { return objectPtr; }
//
//	inline T& operator*() { return *objectPtr; }
//
//	inline operator bool() { return objectPtr != nullptr; }
//
//	inline bool operator==(std::nullptr_t) { return objectPtr == nullptr; }
//
//	inline bool operator!=(std::nullptr_t) { return objectPtr != nullptr; }
//
//	inline bool operator==(const SmartPtr<T>& other) { return objectPtr == other.objectPtr; }
//
//	inline bool operator!=(const SmartPtr<T>& other) { return objectPtr != other.objectPtr; }
//
//	SmartPtr<T>& operator=(const SmartPtr<T>& other)
//	{
//		if (refCount != nullptr && --(refCount->smartRefCount) == 0)
//		{
//			delete objectPtr;
//
//			if (refCount->weakRefCount == 0)
//				delete refCount;
//		}
//
//		objectPtr = other.objectPtr;
//		refCount = other.refCount;
//
//		if (objectPtr != nullptr)
//			refCount->smartRefCount++;
//		else
//			refCount = nullptr;
//
//		return *this;
//	}
//};
//
//
///**
//	* @brief TODO: weak pointer...
//	*
//	* @remarks Allow reference count exsits when object instance is exist. Weak reference
//	*			count records the amount of "observer" to the object instance no matter it
//	*			exists or not. Therefore, when creating a new weak pointer by copying other
//	*			smart pointer / weak pointer instance, reference count is allowed to be existed
//	*			even if the object instance is not exist. But in most cases, smart pointer's
//	*			reference count will not exist if its object instance is null. The weak pointer
//	*			created from this smart pointer will not have reference as well.
//	*/
//template<class T>
//class WeakPtr
//{
//private:
//	T* objectPtr;
//	ReferenceCount* refCount;
//
//public:
//	friend class SmartPtr<T>;
//
//	inline WeakPtr() :
//		objectPtr(nullptr),
//		refCount(nullptr)
//	{ }
//
//	inline WeakPtr(const WeakPtr<T>& other) :
//		objectPtr(other.objectPtr),
//		refCount(other.refCount)
//	{
//		if (refCount != nullptr)
//			refCount->weakRefCount++;
//	}
//
//	template<class U>
//	inline WeakPtr(const WeakPtr<U>& other) :
//		objectPtr(other.objectPtr),
//		refCount(other.refCount)
//	{
//		if (refCount != nullptr)
//			refCount->weakRefCount++;
//	}
//
//	inline WeakPtr(const SmartPtr<T>& other) :
//		objectPtr(other.objectPtr),
//		refCount(other.refCount)
//	{
//		if (refCount != nullptr)
//			refCount->weakRefCount++;
//	}
//
//	inline ~WeakPtr()
//	{
//		if (refCount != nullptr)
//		{
//			refCount->weakRefCount--;
//
//			if (refCount->smartRefCount == 0 && refCount->weakRefCount == 0)
//			{
//				delete refCount;
//			}
//		}
//	}
//
//	inline operator bool() { return objectPtr != nullptr; }
//
//	inline bool operator==(std::nullptr_t) { return objectPtr == nullptr; }
//
//	inline bool operator!=(std::nullptr_t) { return objectPtr != nullptr; }
//
//	inline bool operator==(const WeakPtr<T>& other) { return objectPtr == other.objectPtr; }
//
//	inline bool operator!=(const WeakPtr<T>& other) { return objectPtr != other.objectPtr; }
//
//	WeakPtr<T>& operator=(const WeakPtr<T>& other)
//	{
//		if (refCount != nullptr)
//		{
//			refCount->weakRefCount--;
//
//			if (refCount->smartRefCount == 0 && refCount->weakRefCount == 0)
//			{
//				delete refCount;
//			}
//		}
//
//		objectPtr = other.objectPtr;
//		refCount = other.refCount;
//
//		if (refCount != nullptr)
//			refCount->weakRefCount++;
//
//		return *this;
//	}
//};


/**
* @brief TODO: singleton...
*
* @remarks: To prevent direct construction of a singleton class, the singleton's
*			 constructor is set to be private. Also, functions that we don't want
*			 (like: copy constructor, assignment operator) is explicitly deleted.
*/
template<class T>
class Singleton
{
protected:
	static T* instance;

	inline Singleton() { }
	inline ~Singleton() { }

public:
	inline Singleton(const Singleton& other) = delete;
	inline void operator=(const Singleton& other) = delete;

	inline static T* Instance()
	{
		if (instance == nullptr)
			instance = new T();

		return instance;
	}

	inline virtual void Destory()
	{
		if (instance != nullptr)
			delete instance;
	}
};

template<class T>
T* Singleton<T>::instance = nullptr;




template<class T>
class LinkedNode
{
public:
	LinkedNode<T>* next;
	LinkedNode<T>* prev;
	T* data;

	inline LinkedNode() :
		next(nullptr),
		prev(nullptr),
		data(nullptr)
	{ }

	inline LinkedNode(LinkedNode<T>* next, LinkedNode<T>* prev, T* data) :
		next(next),
		prev(prev),
		data(data)
	{ }

	inline ~LinkedNode()
	{
		delete data;
	}
};


template<class T>
class LinkedList
{
public:
	int size;
	LinkedNode<T>* head;
	LinkedNode<T>* tail;

	inline LinkedList() : 
		size(0),
		head(nullptr),
		tail(nullptr)
	{ }

	inline ~LinkedList()
	{ }



};






class GameObject
{
private:

public:
	string name;
	Vector2<double> position;

	inline GameObject();
	inline GameObject(const string& name, const Vector2<double>& position = Vector2<double>::Zero);
	inline ~GameObject();

	inline static SmartPtr<GameObject> Create(const string& name, const Vector2<double>& position = Vector2<double>::Zero);
};



#include "DataType.inl"

}