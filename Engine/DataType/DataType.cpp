#include "DataType.h"

using namespace Engine;

#pragma region Vector2

const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector2 Vector2::Left(-1.0f, 0.0f);
const Vector2 Vector2::Right(1.0f, 0.0f);
const Vector2 Vector2::Up(0.0f, 1.0f);
const Vector2 Vector2::Down(0.0f, -1.0f);

#pragma endregion


#pragma region SmartPtr

template<class T>
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
}

#pragma endregion



#pragma region GameObject

#pragma endregion
