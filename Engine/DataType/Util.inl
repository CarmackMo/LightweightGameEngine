#pragma once
using namespace Engine;


#pragma region ReferenceCount

inline ReferenceCount::ReferenceCount(unsigned long smartRef, unsigned long weakRef) :
	smartRefCount(smartRef),
	weakRefCount(weakRef)
{ }

inline ReferenceCount::~ReferenceCount()
{ }

#pragma endregion



#pragma region SmartPtr

template <class T>
inline SmartPtr<T>::SmartPtr() :
	objectPtr(nullptr),
	refCount(nullptr)
{ }

template <class T>
inline SmartPtr<T>::SmartPtr(T* ptr) :
	objectPtr(ptr),
	refCount(new ReferenceCount(1, 0))
{ }

template <class T>
inline SmartPtr<T>::SmartPtr(const SmartPtr<T>& other) :
	objectPtr(other.objectPtr),
	refCount(other.refCount)
{
	if (objectPtr == nullptr)
		refCount = nullptr;
	else
		refCount->smartRefCount++;
}

template <class T>
template <class U>
inline SmartPtr<T>::SmartPtr(const SmartPtr<U>& other) :
	objectPtr(other.objectPtr),
	refCount(other.refCount)
{
	if (objectPtr == nullptr)
		refCount = nullptr;
	else
		refCount->smartRefCount++;
}

template <class T>
inline SmartPtr<T>::SmartPtr(const WeakPtr<T>& other) :
	objectPtr(other.objectPtr),
	refCount(other.refCount)
{
	if (objectPtr == nullptr)
		refCount = nullptr;
	else
		refCount->smartRefCount++;
}

template <class T>
inline SmartPtr<T>::~SmartPtr()
{
	if (refCount != nullptr)
	{
		refCount->smartRefCount--;

		if (refCount->smartRefCount == 0)
		{
			delete objectPtr;

			if (refCount->weakRefCount == 0)
				delete refCount;
		}
	}
}

/* Access operators */
template <class T>
inline T* SmartPtr<T>::operator->() { return objectPtr; }

template <class T>
inline T& SmartPtr<T>::operator*() { return *objectPtr; }

/* Comparision operators */
template <class T>
inline SmartPtr<T>::operator bool() { return objectPtr != nullptr; }

template <class T>
inline bool SmartPtr<T>::operator==(std::nullptr_t) { return objectPtr == nullptr; }

template <class T>
inline bool SmartPtr<T>::operator!=(std::nullptr_t) { return objectPtr != nullptr; }

template <class T>
inline bool SmartPtr<T>::operator==(const SmartPtr<T>& other) { return objectPtr == other.objectPtr; }

template <class T>
inline bool SmartPtr<T>::operator!=(const SmartPtr<T>& other) { return objectPtr != other.objectPtr; }

#pragma endregion



#pragma region WeakPtr

template <class T>
inline WeakPtr<T>::WeakPtr() :
	objectPtr(nullptr),
	refCount(nullptr)
{ }

template <class T>
inline WeakPtr<T>::WeakPtr(const WeakPtr<T>& other) :
	objectPtr(other.objectPtr),
	refCount(other.refCount)
{
	if (refCount != nullptr)
		refCount->weakRefCount++;
}

template <class T>
template<class U>
inline WeakPtr<T>::WeakPtr(const WeakPtr<U>& other) :
	objectPtr(other.objectPtr),
	refCount(other.refCount)
{
	if (refCount != nullptr)
		refCount->weakRefCount++;
}

template <class T>
inline WeakPtr<T>::WeakPtr(const SmartPtr<T>& other) :
	objectPtr(other.objectPtr),
	refCount(other.refCount)
{
	if (refCount != nullptr)
		refCount->weakRefCount++;
}

template <class T>
inline WeakPtr<T>::~WeakPtr()
{
	if (refCount != nullptr)
	{
		refCount->weakRefCount--;

		if (refCount->smartRefCount == 0 && refCount->weakRefCount == 0)
		{
			delete refCount;
		}
	}
}

/* Comparision operators */
template <class T>
inline WeakPtr<T>::operator bool() { return objectPtr != nullptr; }

template <class T>
inline bool WeakPtr<T>::operator==(std::nullptr_t) { return objectPtr == nullptr; }

template <class T>
inline bool WeakPtr<T>::operator!=(std::nullptr_t) { return objectPtr != nullptr; }

template <class T>
inline bool WeakPtr<T>::operator==(const WeakPtr<T>& other) { return objectPtr == other.objectPtr; }

template <class T>
inline bool WeakPtr<T>::operator!=(const WeakPtr<T>& other) { return objectPtr != other.objectPtr; }

#pragma endregion


#pragma region Singleton

template<class T>
inline Singleton<T>::Singleton() { }

template<class T>
inline Singleton<T>::~Singleton() { }

template<class T>
inline T* Singleton<T>::Instance()
{
	if (instance == nullptr)
		instance = new T();

	return instance;
}

template<class T>
inline void Singleton<T>::Destory()
{
	if (instance != nullptr)
		delete instance;
}

#pragma endregion
