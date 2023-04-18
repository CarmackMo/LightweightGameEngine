#include "Util.h"

using namespace Engine;


#pragma region SmartPtr

template <class T>
SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T>& other)
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

#pragma endregion


#pragma region WeakPtr

template<class T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other)
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


#pragma endregion
