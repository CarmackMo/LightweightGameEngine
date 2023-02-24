#pragma once

using namespace Engine;

#pragma region Vector2

/* Constructor */
inline Vector2::Vector2() : 
	x_(0), 
	y_(0)
{ }
inline Vector2::Vector2(float x, float y) : 
	x_(x), 
	y_(y)
{ }
inline Vector2::Vector2(const Vector2 &vec) : 
	x_(vec.x_), 
	y_(vec.y_)
{ }

/* Element getter */
inline float Vector2::x(void) const { return x_; }
inline float Vector2::y(void) const { return y_; }
/* Element setter */
inline void Vector2::x(float x) { x_ = x; }
inline void Vector2::y(float y) { y_ = y; }

/* Self modifying operators */
inline void Vector2::operator+= (const Vector2 &vec)
{
	x_ += vec.x_;
	y_ += vec.y_;
}
inline void Vector2::operator-= (const Vector2 &vec)
{
	x_ -= vec.x_;
	y_ -= vec.y_;
}

inline void Vector2::operator*= (Vector2 const&vec)
{
	x_ *= vec.x_;
	y_ *= vec.y_;
}
inline void Vector2::operator*= (float num)
{
	x_ *= num;
	y_ *= num;
}

inline void Vector2::operator/= (const Vector2 &vec)
{
	x_ /= vec.x_;
	y_ /= vec.y_;
}
inline void Vector2::operator/= (float num)
{
	x_ /= num;
	y_ /= num;
}

/* Modifying operators */
inline Vector2 Vector2::operator+ (const Vector2 &vec) const
{
	return Vector2(x_ + vec.x_, y_ + vec.y_);
}
inline Vector2 Vector2::operator- (const Vector2 &vec) const
{
	return Vector2(x_ - vec.x_, y_ - vec.y_);
}

inline Vector2 Vector2::operator* (const Vector2 &vec) const
{
	return Vector2(x_ * vec.x_, y_ * vec.y_);
}
inline Vector2 Vector2::operator* (float num) const
{
	return Vector2(x_ * num, y_ * num);
}
inline Vector2 operator* (float left, Vector2 right)
{
	return Vector2(left * right.x(), left * right.y());
}

inline Vector2 Vector2::operator/ (const Vector2 &vec) const
{
	return Vector2(x_ / vec.x_, y_ / vec.y_);
}
inline Vector2 Vector2::operator/ (float num) const
{
	return Vector2(x_ / num, y_ / num);
}

/* Assignment operators */
inline Vector2& Vector2::operator= (const Vector2 &vec)
{
	x_ = vec.x_;
	y_ = vec.y_;
	return *this;
}

/* Comparison operators */
inline bool Vector2::operator== (const Vector2 &vec) const
{
	return (x_ == vec.x_) && (y_ == vec.y_);
}

inline bool Vector2::operator!= (const Vector2 &vec) const
{
	return (x_ != vec.x_) || (y_ != vec.y_);
}

/* Negate */
inline Vector2 Vector2::operator- (void) const
{
	return Vector2(-x_, -y_);
}

#pragma endregion


#pragma region ReferenceCount

inline ReferenceCount::ReferenceCount(unsigned long smartRef, unsigned long weakRef) : 
	smartRefCount(smartRef), 
	weakRefCount(weakRef)
{ }

inline ReferenceCount::~ReferenceCount()
{ }

#pragma endregion


#pragma region SmartPtr

//inline SmartPtr<class T>::SmartPtr() : 
//	objectPtr(nullptr),
//	refCount(nullptr)
//{ }
//
//inline SmartPtr<class T>::SmartPtr(T* ptr) :
//	objectPtr(ptr),
//	refCount(new ReferenceCount(1, 0))
//{ }
//
//inline SmartPtr<class T>::SmartPtr(const SmartPtr& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (objectPtr == nullptr)
//		refCount = nullptr;
//	else
//		refCount->smartRefCount++;
//}
//
//inline SmartPtr<class T>::SmartPtr(const WeakPtr<T>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (objectPtr == nullptr)
//		refCount = nullptr;
//	else
//		refCount++;
//}
//
//inline SmartPtr<class T>::~SmartPtr()
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
//inline T* SmartPtr<class T>::operator->()
//{
//	return objectPtr;
//}
//
//
//inline SmartPtr<class T>::operator bool()
//{
//	return objectPtr != nullptr;
//}
//
//inline bool SmartPtr<class T>::operator==(std::nullptr_t)
//{
//	return objectPtr == nullptr;
//}
//
//inline bool SmartPtr<class T>::operator!=(std::nullptr_t)
//{
//	return objectPtr != nullptr;
//}
//
//inline bool SmartPtr<class T>::operator==(const SmartPtr& other)
//{
//	return objectPtr == other.objectPtr;
//}
//
//inline bool SmartPtr<class T>::operator!=(const SmartPtr& other)
//{
//	return objectPtr != other.objectPtr;
//}

#pragma endregion


#pragma region WeakPtr

//inline WeakPtr<class T>::WeakPtr() :
//	objectPtr(nullptr),
//	refCount(nullptr)
//{ }
//
//inline WeakPtr<class T>::WeakPtr(const WeakPtr& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (refCount != nullptr)
//		refCount->weakRefCount++;
//}
//
//inline WeakPtr<class T>::WeakPtr(const SmartPtr<T>& other) :
//	objectPtr(other.objectPtr),
//	refCount(other.refCount)
//{
//	if (refCount != nullptr)
//		refCount->weakRefCount++;
//}
//
//inline WeakPtr<class T>::~WeakPtr()
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
//inline WeakPtr<class T>::operator bool()
//{
//	return objectPtr != nullptr;
//}
//
//inline bool WeakPtr<class T>::operator==(std::nullptr_t)
//{
//	return objectPtr == nullptr;
//}
//
//inline bool WeakPtr<class T>::operator!=(std::nullptr_t)
//{
//	return objectPtr != nullptr;
//}
//
//inline bool WeakPtr<class T>::operator==(const WeakPtr& other)
//{
//	return objectPtr == other.objectPtr;
//}
//
//inline bool WeakPtr<class T>::operator!=(const WeakPtr& other)
//{
//	return objectPtr != other.objectPtr;
//}

#pragma endregion


#pragma region GameObject

inline GameObject::GameObject() : 
	mass(0.0),
	name(""),
	position(Vector2()),
	velocity(Vector2())
{ }

inline GameObject::GameObject(string name, float mass, Vector2 position, Vector2 velocity) :
	mass(mass),
	name(name),
	position(position),
	velocity(velocity)
{ }

inline GameObject::~GameObject()
{ }

inline SmartPtr<GameObject> GameObject::Create(string name)
{
	return SmartPtr<GameObject>(new GameObject(name));
}

#pragma endregion
