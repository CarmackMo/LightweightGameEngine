#include "SmartPtrs.h"

namespace Engine
{

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
inline SmartPtr<T>::SmartPtr()
{ }

template <class T>
inline SmartPtr<T>::SmartPtr(nullptr_t)
{ }

template <class T>
inline SmartPtr<T>::SmartPtr(T* ptr)
{
	this->StandardConstruct(ptr);
}

template <class T>
inline SmartPtr<T>::SmartPtr(T* ptr, function<void(T*)> deleter)
{
	this->StandardConstruct(ptr, deleter);
}

template <class T>
template <class U>
inline SmartPtr<T>::SmartPtr(const SmartPtr<U>& other, T* ptr)
{
	this->AliasConstruct(other, ptr);
}

template <class T>
template <class U>
inline SmartPtr<T>::SmartPtr(SmartPtr<U>&& other, T* ptr)
{
	this->AliasMoveConstruct(move(other), ptr);
}

template<class T>
inline SmartPtr<T>::SmartPtr(const SmartPtr<T>& other)
{
	this->CopyConstruct(other);
}

template<class T>
template<class U>
inline SmartPtr<T>::SmartPtr(const SmartPtr<U>& other)
{
	this->CopyConstruct(other);
}

template<class T>
inline SmartPtr<T>::SmartPtr(SmartPtr<T>&& other)
{
	this->MoveConstruct(move(other));
}

template<class T>
template<class U>
inline SmartPtr<T>::SmartPtr(SmartPtr<U>&& other)
{
	this->MoveConstruct(move(other));
}

template<class T>
inline SmartPtr<T>::~SmartPtr()
{
	this->DecSmartRef();
}


/* Access operators */
template <class T>
inline T* SmartPtr<T>::operator->() 
{ 
	return this->Get(); 
}

template <class T>
inline T& SmartPtr<T>::operator*() 
{ 
	return *(this->Get()); 
}


/* Comparision operators */
template <class T>
inline SmartPtr<T>::operator bool() 
{ 
	return this->Get() != nullptr; 
}

template <class T>
inline bool SmartPtr<T>::operator==(std::nullptr_t) 
{ 
	return this->Get() == nullptr; 
}

template <class T>
inline bool SmartPtr<T>::operator!=(std::nullptr_t) 
{ 
	return this->Get() != nullptr; 
}

template <class T>
template <class U>
inline bool SmartPtr<T>::operator==(const SmartPtr<U>& other) 
{ 
	return this->Get() == other.Get(); 
}

template <class T>
template <class U>
inline bool SmartPtr<T>::operator!=(const SmartPtr<U>& other) 
{ 
	return this->Get() != other.Get(); 
}








/*********************** Old version ************************/
//template <class T>
//inline SmartPtr<T>::SmartPtr() :
//	objectPtr(nullptr),
//	refCount(nullptr)
//{ }
//
//
//template <class T>
//inline SmartPtr<T>::SmartPtr(T* ptr) :
//	objectPtr(ptr),
//	refCount(new ReferenceCount(1, 0))
//{ }
//
//
//template <class T>
//inline SmartPtr<T>::SmartPtr(const SmartPtr<T>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (objectPtr == nullptr)
//		refCount = nullptr;
//	else
//		refCount->smartRefCount++;
//}
//
//
//template <class T>
//template <class U>
//inline SmartPtr<T>::SmartPtr(const SmartPtr<U>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (objectPtr == nullptr)
//		refCount = nullptr;
//	else
//		refCount->smartRefCount++;
//}
//
//
//template <class T>
//inline SmartPtr<T>::SmartPtr(const WeakPtr<T>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (objectPtr == nullptr)
//		refCount = nullptr;
//	else
//		refCount->smartRefCount++;
//}
//
//
//template <class T>
//inline SmartPtr<T>::~SmartPtr()
//{
//	if (refCount != nullptr)
//	{
//		refCount->smartRefCount--;
//
//		if (refCount->smartRefCount == 0)
//		{
//			delete objectPtr;
//
//			if (refCount->weakRefCount == 0)
//				delete refCount;
//		}
//	}
//}
//
//
///* Access operators */
//template <class T>
//inline T* SmartPtr<T>::operator->() { return objectPtr; }
//
//
//template <class T>
//inline T& SmartPtr<T>::operator*() { return *objectPtr; }
//
//
///* Comparision operators */
//template <class T>
//inline SmartPtr<T>::operator bool() { return objectPtr != nullptr; }
//
//
//template <class T>
//inline bool SmartPtr<T>::operator==(std::nullptr_t) { return objectPtr == nullptr; }
//
//
//template <class T>
//inline bool SmartPtr<T>::operator!=(std::nullptr_t) { return objectPtr != nullptr; }
//
//
//template <class T>
//inline bool SmartPtr<T>::operator==(const SmartPtr<T>& other) { return objectPtr == other.objectPtr; }
//
//
//template <class T>
//inline bool SmartPtr<T>::operator!=(const SmartPtr<T>& other) { return objectPtr != other.objectPtr; }
//
//
///* Assignment operators */
//template <class T>
//SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T>& other)
//{
//	if (refCount != nullptr && --(refCount->smartRefCount) == 0)
//	{
//		delete objectPtr;
//
//		if (refCount->weakRefCount == 0)
//			delete refCount;
//	}
//
//	objectPtr = other.objectPtr;
//	refCount = other.refCount;
//
//	if (objectPtr != nullptr)
//		refCount->smartRefCount++;
//	else
//		refCount = nullptr;
//
//	return *this;
//}


#pragma endregion



#pragma region WeakPtr

//template <class T>
//inline WeakPtr<T>::WeakPtr() :
//	objectPtr(nullptr),
//	refCount(nullptr)
//{ }
//
//
//template <class T>
//inline WeakPtr<T>::WeakPtr(const WeakPtr<T>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (refCount != nullptr)
//		refCount->weakRefCount++;
//}
//
//
//template <class T>
//template<class U>
//inline WeakPtr<T>::WeakPtr(const WeakPtr<U>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (refCount != nullptr)
//		refCount->weakRefCount++;
//}
//
//
//template <class T>
//inline WeakPtr<T>::WeakPtr(const SmartPtr<T>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (refCount != nullptr)
//		refCount->weakRefCount++;
//}
//
//
//template <class T>
//inline WeakPtr<T>::~WeakPtr()
//{
//	if (refCount != nullptr)
//	{
//		refCount->weakRefCount--;
//
//		if (refCount->smartRefCount == 0 && refCount->weakRefCount == 0)
//		{
//			delete refCount;
//		}
//	}
//}
//
//
///* Comparision operators */
//template <class T>
//inline WeakPtr<T>::operator bool() { return objectPtr != nullptr; }
//
//
//template <class T>
//inline bool WeakPtr<T>::operator==(std::nullptr_t) { return objectPtr == nullptr; }
//
//
//template <class T>
//inline bool WeakPtr<T>::operator!=(std::nullptr_t) { return objectPtr != nullptr; }
//
//
//template <class T>
//inline bool WeakPtr<T>::operator==(const WeakPtr<T>& other) { return objectPtr == other.objectPtr; }
//
//
//template <class T>
//inline bool WeakPtr<T>::operator!=(const WeakPtr<T>& other) { return objectPtr != other.objectPtr; }
//
//
///* Assignment operators */
//template<class T>
//WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other)
//{
//	if (refCount != nullptr)
//	{
//		refCount->weakRefCount--;
//
//		if (refCount->smartRefCount == 0 && refCount->weakRefCount == 0)
//		{
//			delete refCount;
//		}
//	}
//
//	objectPtr = other.objectPtr;
//	refCount = other.refCount;
//
//	if (refCount != nullptr)
//		refCount->weakRefCount++;
//
//	return *this;
//}


#pragma endregion


}//Namespace Engine

