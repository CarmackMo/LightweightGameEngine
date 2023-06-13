#include "SmartPtrs.h"

namespace Engine
{



#pragma region SmartPtr

/* Constructors */
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
inline SmartPtr<T>::SmartPtr(const SmartPtr<T>& other)
{
	this->CopyConstruct(other);
}


template <class T>
template <class U>
inline SmartPtr<T>::SmartPtr(const SmartPtr<U>& other)
{
	this->CopyConstruct(other);
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
	this->AliasMoveConstruct(std::move(other), ptr);
}


template <class T>
inline SmartPtr<T>::SmartPtr(SmartPtr<T>&& other)
{
	this->MoveConstruct(std::move(other));
}


template <class T>
template <class U>
inline SmartPtr<T>::SmartPtr(SmartPtr<U>&& other)
{
	this->MoveConstruct(std::move(other));
}


template <class T>
template <class U>
inline SmartPtr<T>::SmartPtr(const WeakPtr<U>& other)
{
	bool res = this->ConstructFromWeak(other);
	assert(res == true);
}


template <class T>
inline SmartPtr<T>::~SmartPtr()
{
	this->DecSmartRef();
}


/* Operations */
template <class T>
inline bool SmartPtr<T>::IsUnique() const
{
	return this->GetSmartCount() == 1;
}


template <class T>
inline void SmartPtr<T>::Swap(SmartPtr<T>& other)
{
	this->SwapPtr(other);
}


template <class T>
inline void SmartPtr<T>::Reset()
{
	SmartPtr().Swap(*this);
}


template <class T>
template <class U>
inline void SmartPtr<T>::Reset(U* ptr)
{
	SmartPtr(ptr).Swap(*this);
}


template <class T>
template <class U>
inline void SmartPtr<T>::Reset(U* ptr, function<void(U*)> deleter)
{
	SmartPtr(ptr, deleter).Swap(*this);
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
inline bool SmartPtr<T>::operator==(const SmartPtr<T>& other) 
{ 
	return this->Get() == other.Get(); 
}

template <class T>
inline bool SmartPtr<T>::operator!=(const SmartPtr<T>& other) 
{ 
	return this->Get() != other.Get(); 
}


/* Assignment operators */
template <class T>
inline SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T>& other)
{
	SmartPtr(other).Swap(*this);
	return *this;
}

template <class T>
template <class U>
inline SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<U>& other)
{
	SmartPtr(other).Swap(*this);
	return *this;
}

template <class T>
inline SmartPtr<T>& SmartPtr<T>::operator=(SmartPtr<T>&& other)
{
	SmartPtr(std::move(other)).Swap(*this);
	return *this;
}

template <class T>
template<class U>
inline SmartPtr<T>& SmartPtr<T>::operator=(SmartPtr<U>&& other)
{
	SmartPtr(std::move(other)).Swap(*this);
	return *this;
}

#pragma endregion



#pragma region WeakPtr

/* Constructors */
template <class T>
inline WeakPtr<T>::WeakPtr(const WeakPtr<T>& other)
{
	this->WeakConstruct(other);
}


template <class T>
template <class U>
inline WeakPtr<T>::WeakPtr(const WeakPtr<U>& other)
{
	this->WeakConstruct(other);
}


template <class T>
template <class U>
inline WeakPtr<T>::WeakPtr(const SmartPtr<U>& other)
{
	this->WeakConstruct(other);
}


template <class T>
inline WeakPtr<T>::WeakPtr(WeakPtr<T>&& other)
{
	this->MoveConstruct(std::move(other));
}


template <class T>
template <class U>
inline WeakPtr<T>::WeakPtr(WeakPtr<U>&& other)
{
	this->MoveConstruct(std::move(other));
}


template <class T>
inline WeakPtr<T>::~WeakPtr()
{
	this->DecWeakRef();
}


/* Operations */
template <class T>
inline bool WeakPtr<T>::IsExpired() const
{
	return this->GetSmartCount() == 0;
}


template <class T>
inline void WeakPtr<T>::Swap(WeakPtr<T>& other)
{
	this->SwapPtr(other);
}


template <class T>
inline void WeakPtr<T>::Reset()
{
	WeakPtr().Swap(*this);
}


/* Comparision operators */
template <class T>
inline WeakPtr<T>::operator bool()
{
	return this->Get() != nullptr;
}


template <class T>
inline bool WeakPtr<T>::operator==(std::nullptr_t)
{
	return this->Get() == nullptr;
}


template <class T>
inline bool WeakPtr<T>::operator!=(std::nullptr_t)
{
	return this->Get() != nullptr;
}


template <class T>
inline bool WeakPtr<T>::operator==(const WeakPtr<T>& other)
{
	return this->Get() == other.Get();
}


template <class T>
inline bool WeakPtr<T>::operator!=(const WeakPtr<T>& other)
{
	return this->Get() == other.Get();
}


/* Assignment operators */
template <class T>
inline WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other)
{
	WeakPtr(other).Swap(*this);
	return *this;
}


template <class T>
template <class U>
inline WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<U>& other)
{
	WeakPtr(other).Swap(*this);
	return *this;
}


template <class T>
inline WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other)
{
	WeakPtr(std::move(other)).Swap(*this);
	return *this;
}


template <class T>
template <class U>
inline WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<U>&& other)
{
	WeakPtr(std::move(other)).Swap(*this);
	return *this;
}


template <class T>
template <class U>
inline WeakPtr<T>& WeakPtr<T>::operator=(const SmartPtr<U>& other)
{
	WeakPtr(other).Swap(*this);
	return *this;
}

#pragma endregion


}//Namespace Engine

