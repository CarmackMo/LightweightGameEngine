//#include "SmartPtrs.h"

namespace Engine
{

//using std::function;

#pragma region RefCountBase

//inline void RefCountBase::IncSmartRef()
//{
//	smartCount++;
//}
//
//
//inline void RefCountBase::IncWeakRef()
//{
//	weakCount++;
//}
//
//
//inline void RefCountBase::DecSmartRef()
//{
//	smartCount--;
//	if (smartCount == 0)
//	{
//		Destroy();
//		DecWeakRef();
//	}
//}
//
//
//inline void RefCountBase::DecWeakRef()
//{
//	weakCount--;
//	if (weakCount == 0)
//	{
//		DeleteThis();
//	}
//}
//
//
//inline unsigned long RefCountBase::GetSmartCount()
//{
//	return smartCount;
//}
//
//
//inline unsigned long RefCountBase::GetWeakCount()
//{
//	return weakCount;
//}

#pragma endregion



#pragma region RefCount

//template <class T>
//void RefCount<T>::Destroy()
//{
//	if (deleter != nullptr)
//		deleter(ptr);
//	else
//		delete ptr;
//}
//
//
//template <class T>
//void RefCount<T>::DeleteThis()
//{
//	delete this;
//}
//
//
//template <class T>
//RefCount<T>::RefCount(T* ptr, function<void(T*)> deleter) :
//	RefCountBase(), ptr(ptr), deleter(deleter)
//{}

#pragma endregion



#pragma region PtrBase

//template <class T>
//void PtrBase<T>::StandardConstruct(T* ptr, function<void(T*)> deleter)
//{
//	this->ptr = ptr;
//	this->refCount = new RefCount<T>(ptr, deleter);
//}
//
//
//template <class T>
//template <class U>
//void PtrBase<T>::CopyConstruct(const SmartPtr<U>& other)
//{
//	other.IncSmartRef();
//	this->ptr = other.ptr;
//	this->refCount = other.refCount;
//}
//
//
//template <class T>
//template <class U>
//void PtrBase<T>::MoveConstruct(PtrBase<U>&& other)
//{
//	this->ptr = other.ptr;
//	this->refCount = other.refCount;
//
//	other.ptr = nullptr;
//	other.refCount = nullptr;
//}
//
//
//template <class T>
//template <class U>
//void PtrBase<T>::AliasConstruct(const SmartPtr<U>& other, T* ptr)
//{
//	other.IncSmartRef();
//	this->ptr = ptr;
//	this->refCount = other.refCount;
//}
//
//
//template <class T>
//template <class U>
//void PtrBase<T>::AliasMoveConstruct(SmartPtr<U>&& other, T* ptr)
//{
//	this->ptr = other.ptr;
//	this->refCount = other.refCount;
//
//	other.ptr = nullptr;
//	other.refCount = nullptr;
//}
//
//
//template <class T>
//template <class U>
//void PtrBase<T>::WeakConstruct(const PtrBase<U>& other)
//{
//	other.IncWeakRef();
//	this->ptr = other.ptr;
//	this->refCount = other.refCount;
//}
//
//
//template <class T>
//template <class U>
//bool PtrBase<T>::ConstructFromWeak(const WeakPtr<U>& other)
//{
//	if (other.refCount != nullptr && other.IsExpired() != true)
//	{
//		other.IncSmartRef();
//		this->ptr = other.ptr;
//		this->refCount = other.refCount;
//		return true;
//	}
//	return false;
//}

//
//template <class T>
//inline void PtrBase<T>::IncSmartRef() const
//{
//	if (refCount != nullptr)
//		refCount->IncSmartRef();
//}
//
//
//template <class T>
//inline void PtrBase<T>::DecSmartRef()
//{
//	if (refCount != nullptr)
//		refCount->DecSmartRef();
//}
//
//
//template <class T>
//inline void PtrBase<T>::IncWeakRef() const
//{
//	if (refCount != nullptr)
//		refCount->IncWeakRef();
//}
//
//
//template <class T>
//inline void PtrBase<T>::DecWeakRef()
//{
//	if (refCount != nullptr)
//		refCount->DecWeakRef();
//}
//
//
//template <class T>
//inline T* PtrBase<T>::Get() const
//{
//	return ptr;
//}
//
//
//template <class T>
//inline void PtrBase<T>::SwapPtr(PtrBase<T>& other)
//{
//	std::swap(this->ptr, other.ptr);
//	std::swap(this->refCount, other.refCount);
//}


//template <class T>
//inline unsigned long PtrBase<T>::GetSmartCount() const
//{
//	return refCount != nullptr ? refCount->GetSmartCount() : 0;
//}

#pragma endregion



#pragma region SmartPtr

/* Constructors */
//template <class T>
//inline SmartPtr<T>::SmartPtr()
//{ }
//
//
//template <class T>
//inline SmartPtr<T>::SmartPtr(nullptr_t)
//{ }
//
//
//template <class T>
//inline SmartPtr<T>::SmartPtr(T* ptr)
//{
//	this->StandardConstruct(ptr);
//}
//
//
//template <class T>
//inline SmartPtr<T>::SmartPtr(T* ptr, function<void(T*)> deleter)
//{
//	this->StandardConstruct(ptr, deleter);
//}


//template <class T>
//inline SmartPtr<T>::SmartPtr(const SmartPtr<T>& other)
//{
//	this->CopyConstruct(other);
//}
//
//
//template <class T>
//template <class U>
//inline SmartPtr<T>::SmartPtr(const SmartPtr<U>& other)
//{
//	this->CopyConstruct(other);
//}


//template <class T>
//template <class U>
//inline SmartPtr<T>::SmartPtr(const SmartPtr<U>& other, T* ptr)
//{
//	this->AliasConstruct(other, ptr);
//}
//
//template <class T>
//template <class U>
//inline SmartPtr<T>::SmartPtr(SmartPtr<U>&& other, T* ptr)
//{
//	this->AliasMoveConstruct(std::move(other), ptr);
//}


//template <class T>
//inline SmartPtr<T>::SmartPtr(SmartPtr<T>&& other) noexcept
//{
//	this->MoveConstruct(std::move(other));
//}
//
//
//template <class T>
//template <class U>
//inline SmartPtr<T>::SmartPtr(SmartPtr<U>&& other)
//{
//	this->MoveConstruct(std::move(other));
//}


//template <class T>
//template <class U>
//inline SmartPtr<T>::SmartPtr(const WeakPtr<U>& other)
//{
//	bool res = this->ConstructFromWeak(other);
//	assert(res == true);
//}


//template <class T>
//inline SmartPtr<T>::~SmartPtr()
//{
//	this->DecSmartRef();
//}
//
//
///* Operations */
//template <class T>
//inline bool SmartPtr<T>::IsUnique() const
//{
//	return this->GetSmartCount() == 1;
//}
//
//
//template <class T>
//inline void SmartPtr<T>::Swap(SmartPtr<T>& other)
//{
//	this->SwapPtr(other);
//}


//template <class T>
//inline void SmartPtr<T>::Reset()
//{
//	SmartPtr().Swap(*this);
//}
//
//
//template <class T>
//template <class U>
//inline void SmartPtr<T>::Reset(U* ptr)
//{
//	SmartPtr(ptr).Swap(*this);
//}


//template <class T>
//template <class U>
//inline void SmartPtr<T>::Reset(U* ptr, function<void(U*)> deleter)
//{
//	SmartPtr(ptr, deleter).Swap(*this);
//}


///* Access operators */
//template <class T>
//inline T* SmartPtr<T>::operator->() 
//{ 
//	return this->Get(); 
//}
//
//template <class T>
//inline T& SmartPtr<T>::operator*() 
//{ 
//	return *(this->Get()); 
//}
//
//
///* Comparision operators */
//template <class T>
//inline SmartPtr<T>::operator bool() 
//{ 
//	return this->Get() != nullptr; 
//}

//template <class T>
//inline bool SmartPtr<T>::operator==(std::nullptr_t) 
//{ 
//	return this->Get() == nullptr; 
//}
//
//template <class T>
//inline bool SmartPtr<T>::operator!=(std::nullptr_t) 
//{ 
//	return this->Get() != nullptr; 
//}
//
//template <class T>
//inline bool SmartPtr<T>::operator==(const SmartPtr<T>& other) 
//{ 
//	return this->Get() == other.Get(); 
//}
//
//template <class T>
//inline bool SmartPtr<T>::operator!=(const SmartPtr<T>& other) 
//{ 
//	return this->Get() != other.Get(); 
//}


/* Assignment operators */
//template <class T>
//inline SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T>& other)
//{
//	SmartPtr(other).Swap(*this);
//	return *this;
//}
//
//template <class T>
//template <class U>
//inline SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<U>& other)
//{
//	SmartPtr(other).Swap(*this);
//	return *this;
//}
//
//template <class T>
//inline SmartPtr<T>& SmartPtr<T>::operator=(SmartPtr<T>&& other) noexcept
//{
//	SmartPtr(std::move(other)).Swap(*this);
//	return *this;
//}
//
//template <class T>
//template<class U>
//inline SmartPtr<T>& SmartPtr<T>::operator=(SmartPtr<U>&& other)
//{
//	SmartPtr(std::move(other)).Swap(*this);
//	return *this;
//}

#pragma endregion



#pragma region WeakPtr

///* Constructors */
//template <class T>
//inline WeakPtr<T>::WeakPtr(const WeakPtr<T>& other)
//{
//	this->WeakConstruct(other);
//}
//
//
//template <class T>
//template <class U>
//inline WeakPtr<T>::WeakPtr(const WeakPtr<U>& other)
//{
//	this->WeakConstruct(other);
//}
//
//
//template <class T>
//template <class U>
//inline WeakPtr<T>::WeakPtr(const SmartPtr<U>& other)
//{
//	this->WeakConstruct(other);
//}


//template <class T>
//inline WeakPtr<T>::WeakPtr(WeakPtr<T>&& other) noexcept
//{
//	this->MoveConstruct(std::move(other));
//}
//
//
//template <class T>
//template <class U>
//inline WeakPtr<T>::WeakPtr(WeakPtr<U>&& other)
//{
//	this->MoveConstruct(std::move(other));
//}


//template <class T>
//inline WeakPtr<T>::~WeakPtr()
//{
//	this->DecWeakRef();
//}


///* Operations */
//template <class T>
//inline bool WeakPtr<T>::IsExpired() const
//{
//	return this->GetSmartCount() == 0;
//}
//
//
//template <class T>
//inline void WeakPtr<T>::Swap(WeakPtr<T>& other)
//{
//	this->SwapPtr(other);
//}
//
//
//template <class T>
//inline void WeakPtr<T>::Reset()
//{
//	WeakPtr().Swap(*this);
//}


///* Comparision operators */
//template <class T>
//inline WeakPtr<T>::operator bool()
//{
//	return this->Get() != nullptr;
//}
//
//
//template <class T>
//inline bool WeakPtr<T>::operator==(std::nullptr_t)
//{
//	return this->Get() == nullptr;
//}
//
//
//template <class T>
//inline bool WeakPtr<T>::operator!=(std::nullptr_t)
//{
//	return this->Get() != nullptr;
//}
//
//
//template <class T>
//inline bool WeakPtr<T>::operator==(const WeakPtr<T>& other)
//{
//	return this->Get() == other.Get();
//}
//
//
//template <class T>
//inline bool WeakPtr<T>::operator!=(const WeakPtr<T>& other)
//{
//	return this->Get() == other.Get();
//}


///* Assignment operators */
//template <class T>
//inline WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other)
//{
//	WeakPtr(other).Swap(*this);
//	return *this;
//}
//
//
//template <class T>
//template <class U>
//inline WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<U>& other)
//{
//	WeakPtr(other).Swap(*this);
//	return *this;
//}
//
//
//template <class T>
//inline WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other) noexcept
//{
//	WeakPtr(std::move(other)).Swap(*this);
//	return *this;
//}
//
//
//template <class T>
//template <class U>
//inline WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<U>&& other)
//{
//	WeakPtr(std::move(other)).Swap(*this);
//	return *this;
//}
//
//
//template <class T>
//template <class U>
//inline WeakPtr<T>& WeakPtr<T>::operator=(const SmartPtr<U>& other)
//{
//	WeakPtr(other).Swap(*this);
//	return *this;
//}

#pragma endregion


}//Namespace Engine

