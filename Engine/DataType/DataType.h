#pragma once
#include <string>

using namespace std;

namespace Engine
{

class Vector2;
class ReferenceCount;
template<class T> class SmartPtr;
template<class T> class WeakPtr;


class Vector2
{
private:
	float x_;
	float y_;

public:

	/* Constructor */
	inline Vector2();
	inline Vector2(float x, float y);
	inline Vector2(const Vector2 &vec);

	/* Element getter */
	inline float x(void) const;
	inline float y(void) const;
	/* Element setter */
	inline void x(float x);
	inline void y(float y);

	/* Self modifying operators */
	inline void operator+= (const Vector2 &vec);
	inline void operator-= (const Vector2 &vec);

	inline void operator*= (const Vector2 &vec);
	inline void operator*= (float num);

	inline void operator/= (const Vector2 &vec);
	inline void operator/= (float num);

	/* Modifying operators */
	inline Vector2 operator+ (const Vector2 &vec) const;
	inline Vector2 operator- (const Vector2 &vec) const;

	inline Vector2 operator* (const Vector2 &vec) const;
	inline Vector2 operator* (float num) const;
	inline friend Vector2 operator* (float left, Vector2 right);

	inline Vector2 operator/ (const Vector2 &vec) const;
	inline Vector2 operator/ (float num) const;

	/* Assignment operators */
	inline Vector2& operator= (const Vector2 &vec);

	/* Comparison operators */
	inline bool operator== (const Vector2 &vec) const;
	inline bool operator!= (const Vector2 &vec) const;

	/* Negate */
	inline Vector2 operator- (void) const;


	static const Vector2 Zero;
	static const Vector2 Left;
	static const Vector2 Right;
	static const Vector2 Up;
	static const Vector2 Down;
};


class ReferenceCount
{
public:
	unsigned long smartRefCount;
	unsigned long weakRefCount;

	inline ReferenceCount(unsigned long smartRef = 0, unsigned long weakRef = 0); 
	inline ~ReferenceCount();
};


/** 
 * @brief TODO...
 * 
 * @remarks Does not allow reference count exists when object instance is not exist. When
 *			a new smart pointer is created after the object instance is deleted, it will
 *			generate potential harzard if the smart pointer reference count is increased.
 *			Therefore, when creating a new smart pointer by copying other smart pointer / 
 *			weak pointer instance, if the object instance is not exsit, its corresponding 
 *			reference must not exsit as well.
 */
template <class T> class SmartPtr
{
private:
	T* objectPtr;
	ReferenceCount* refCount;

public:
	friend class WeakPtr<T>;

	//inline SmartPtr();
	//inline SmartPtr(T* ptr);
	//inline SmartPtr(const SmartPtr& other);
	//inline SmartPtr(const WeakPtr<T>& other);
	//inline ~SmartPtr();

	//inline T* operator->();
	//inline operator bool();
	//inline bool operator==(std::nullptr_t);
	//inline bool operator!=(std::nullptr_t);
	//inline bool operator==(const SmartPtr& other);
	//inline bool operator!=(const SmartPtr& other);


	inline SmartPtr() :
		objectPtr(nullptr),
		refCount(nullptr)
	{ }

	inline SmartPtr(T* ptr) :
		objectPtr(ptr),
		refCount(new ReferenceCount(1, 0))
	{ }

	template<class U>
	inline SmartPtr(const SmartPtr<U>& other) :
		objectPtr(other.objectPtr),
		refCount(other.refCount)
	{
		if (objectPtr == nullptr)
			refCount = nullptr;
		else
			refCount->smartRefCount++;
	}

	inline SmartPtr(const WeakPtr<T>& other) :
		objectPtr(other.objectPtr),
		refCount(other.refCount)
	{
		if (objectPtr == nullptr)
			refCount = nullptr;
		else
			refCount->smartRefCount++;
	}

	inline ~SmartPtr()
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

	inline T* operator->() { return objectPtr; }

	inline T& operator*() { return *objectPtr; }

	inline operator bool() { return objectPtr != nullptr; }

	inline bool operator==(std::nullptr_t) { return objectPtr == nullptr; }

	inline bool operator!=(std::nullptr_t) { return objectPtr != nullptr; }

	inline bool operator==(const SmartPtr<T>& other) { return objectPtr == other.objectPtr; }

	inline bool operator!=(const SmartPtr<T>& other) { return objectPtr != other.objectPtr; }

	SmartPtr<T>& operator=(const SmartPtr<T>& other);
};


/**
 * @brief TODO...
 *
 * @remarks Allow reference count exsits when object instance is exist. Weak reference 
 *			count records the amount of "observer" to the object instance no matter it
 *			exists or not. Therefore, when creating a new weak pointer by copying other 
 *			smart pointer / weak pointer instance, reference count is allowed to be existed
 *			even if the object instance is not exist. But in most cases, smart pointer's
 *			reference count will not exist if its object instance is null. The weak pointer
 *			created from this smart pointer will not have reference as well.
 */
template<class T> class WeakPtr
{
private:
	T* objectPtr;
	ReferenceCount* refCount;

public:
	friend class SmartPtr<T>;

	//inline WeakPtr();
	//inline WeakPtr(const WeakPtr& other);
	//inline WeakPtr(const SmartPtr<T>& other);
	//inline ~WeakPtr();

	//inline operator bool();
	//inline bool operator==(std::nullptr_t);
	//inline bool operator!=(std::nullptr_t);
	//inline bool operator==(const WeakPtr& other);
	//inline bool operator!=(const WeakPtr& other);

	inline WeakPtr() :
		objectPtr(nullptr),
		refCount(nullptr)
	{ }

	template<class U>
	inline WeakPtr(const WeakPtr<U>& other) :
		objectPtr(other.objectPtr),
		refCount(other.refCount)
	{
		if (refCount != nullptr)
			refCount->weakRefCount++;
	}

	inline WeakPtr(const SmartPtr<T>& other) :
		objectPtr(other.objectPtr),
		refCount(other.refCount)
	{
		if (refCount != nullptr)
			refCount->weakRefCount++;
	}

	inline ~WeakPtr() 
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

	inline operator bool() { return objectPtr != nullptr; }

	inline bool operator==(std::nullptr_t) { return objectPtr == nullptr; }

	inline bool operator!=(std::nullptr_t) { return objectPtr != nullptr; }

	inline bool operator==(const WeakPtr<T>& other) { return objectPtr == other.objectPtr; }

	inline bool operator!=(const WeakPtr<T>& other) { return objectPtr != other.objectPtr; }

	WeakPtr<T>& operator=(const WeakPtr<T>& other);
};


class GameObject
{
private:

public:
	float mass;
	string name;
	Vector2 position;
	Vector2 velocity;

	inline GameObject();
	inline GameObject(string name, float mass = 0.0, Vector2 position = Vector2(), Vector2 velocity = Vector2());
	inline ~GameObject();

	inline static SmartPtr<GameObject> Create(string name);
};



#include "DataType.inl"

}