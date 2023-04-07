#pragma once
#pragma once
#include "Dependency.h"

using namespace std;

namespace Engine
{
/* Classes forwared declaration */
class Vector2;
class Vector3;
class Vector4;
class Matrix4;
class ReferenceCount;
template<class T> class SmartPtr;
template<class T> class WeakPtr;
template<class T> class Singleton;
class GameObject;



class Vector2
{
public:
	double x;
	double y;

	/* Constructor */
	inline Vector2();
	inline Vector2(double x, double y);
	inline Vector2(const Vector2& vec);

	///* Element getter */
	//inline float x(void) const;
	//inline float y(void) const;
	///* Element setter */
	//inline void x(float x);
	//inline void y(float y);

	/* Self modifying operators */
	inline void operator+= (const Vector2& vec);
	inline void operator-= (const Vector2& vec);

	inline void operator*= (const Vector2& vec);
	inline void operator*= (double num);

	inline void operator/= (const Vector2& vec);
	inline void operator/= (double num);

	/* Modifying operators */
	inline Vector2 operator+ (const Vector2& vec) const;
	inline Vector2 operator- (const Vector2& vec) const;

	inline Vector2 operator* (const Vector2& vec) const;
	inline Vector2 operator* (double num) const;
	inline friend Vector2 operator* (double left, Vector2 right);

	inline Vector2 operator/ (const Vector2& vec) const;
	inline Vector2 operator/ (double num) const;

	/* Assignment operators */
	inline Vector2& operator= (const Vector2& vec);

	/* Comparison operators */
	inline bool operator== (const Vector2& vec) const;
	inline bool operator!= (const Vector2& vec) const;

	/* Negate */
	inline Vector2 operator- (void) const;

	/* @brief Vector2(0, 0) */
	static const Vector2 Zero;
	/* @brief Vector2(-1, 0) */
	static const Vector2 Left;
	/* @brief Vector2(1, 0) */
	static const Vector2 Right;
	/* @brief Vector2(0, 1) */
	static const Vector2 Up;
	/* @brief Vector2(0, -1) */
	static const Vector2 Down;
};


class Vector3
{
public:
	double x;
	double y;
	double z;

	/* Constructor */
	inline Vector3();
	inline Vector3(double x, double y, double z);
	inline Vector3(const Vector3& other);

	/* Self modifying operators */
	inline void operator+= (const Vector3& vec);
	inline void operator-= (const Vector3& vec);

	inline void operator*= (const Vector3& vec);
	inline void operator*= (double num);

	inline void operator/= (const Vector3& vec);
	inline void operator/= (double num);

	/* Modifying operators */
	inline Vector3 operator+ (const Vector3& vec) const;
	inline Vector3 operator- (const Vector3& vec) const;

	inline Vector3 operator* (const Vector3& vec) const;
	inline Vector3 operator* (double num) const;
	inline friend Vector3 operator* (double left, Vector3 right);

	inline Vector3 operator/ (const Vector3& vec) const;
	inline Vector3 operator/ (double num) const;

	/* Assignment operators */
	inline Vector3& operator= (const Vector3& vec);

	/* Comparison operators */
	inline bool operator== (const Vector3& vec) const;
	inline bool operator!= (const Vector3& vec) const;

	/* Negate */
	inline Vector3 operator- (void) const;

	/* @brief Vector3(0, 0, 0) */
	static const Vector3 Zero;
	/* @brief Vector3(-1, 0, 0) */
	static const Vector3 Left;
	/* @brief Vector3(1, 0, 0) */
	static const Vector3 Right;
	/* @brief Vector3(0, 1, 0) */
	static const Vector3 Up;
	/* @brief Vector3(0, -1, 0) */
	static const Vector3 Down;
	/* @brief Vector3(0, 0, 1) */
	static const Vector3 Front;
	/* @brief Vector3(0, 0, -1) */
	static const Vector3 Back;
};


class Vector4
{
public:
	double w, x, y, z;

	Vector4();
	Vector4(double w, double x, double y, double z);
	Vector4(const Vector4& other);

	double At(int idx) const;
	void Set(int idx, double val);
};



class Matrix4
{
public:
	/* Value base constructure */
	Matrix4(
		double x00, double x01, double x02, double x03,
		double x10, double x11, double x12, double x13,
		double x20, double x21, double x22, double x23,
		double x30, double x31, double x32, double x33);
	/* Copy constructure */
	Matrix4(const Matrix4& other);

	/* Identity matrix creator */
	static Matrix4 CreateIdentity(void);

	/* Rotation matrix creator */
	/* Rotate matrix around X-axis, rotate counter-clockwise */
	static Matrix4 CreateXRotation(double rad);
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	static Matrix4 CreateYRotation(double rad);
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	static Matrix4 CreateZRotation(double rad);

	/* Translation matrix creator */
	static Matrix4 CreateTranslation(Vector3& vec);
	static Matrix4 CreateTranslation(double transX, double transY, double transZ);
	
	/* Scale matrix creator */
	static Matrix4 CreateScale(Vector3& vec);
	static Matrix4 CreateScale(double scaleX, double scaleY, double scaleZ);

	/* Return the value at M(i,j) */
	double At(int row, int col) const;
	/* Update the value at M(i,j) */
	void Set(int row, int col, double data);
	/* Calculate the determinant of the 3x3 sub-matrix where M(row, col) is the pivot */
	double Det(int row, int col) const;

	/* Invert this instance */
	void Invert(void);
	/* Return a matrix that is the inverse of this instance but don't modify this instance */
	Matrix4 GetInverse(void) const;

	/* Transpose this instance */
	void Transpose(void);
	/* Return a matrix that is the transpose of this instance but don't modify this instance */
	Matrix4 GetTranspose(void) const;

	/* Multiply vector by matrix */
	/* Return v * M */
	Vector4 MultiplyLeft(const Vector4& vec) const;
	/* Return M * v */
	Vector4 MultiplyRight(const Vector4& vec) const;

	/* Multiply matrix by matrix, return this * other */
	Matrix4 operator* (const Matrix4& other) const;
	/* Comparison operators */
	bool operator== (const Matrix4& other) const;
	/* Assignment operators */
	Matrix4& operator=(const Matrix4& other);

private:
	double val[4][4];
	
	Matrix4();
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

public:
	friend class WeakPtr<T>;

	inline SmartPtr() :
		objectPtr(nullptr),
		refCount(nullptr)
	{ }

	inline SmartPtr(T* ptr) :
		objectPtr(ptr),
		refCount(new ReferenceCount(1, 0))
	{ }

	inline SmartPtr(const SmartPtr<T>& other) :
		objectPtr(other.objectPtr),
		refCount(other.refCount)
	{
		if (objectPtr == nullptr)
			refCount = nullptr;
		else
			refCount->smartRefCount++;
	}

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

	SmartPtr<T>& operator=(const SmartPtr<T>& other)
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

	inline WeakPtr() :
		objectPtr(nullptr),
		refCount(nullptr)
	{ }

	inline WeakPtr(const WeakPtr<T>& other) :
		objectPtr(other.objectPtr),
		refCount(other.refCount)
	{
		if (refCount != nullptr)
			refCount->weakRefCount++;
	}

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

	WeakPtr<T>& operator=(const WeakPtr<T>& other)
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
};


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
	Vector2 position;

	inline GameObject();
	inline GameObject(const string& name, const Vector2& position = Vector2::Zero);
	inline ~GameObject();

	inline static SmartPtr<GameObject> Create(const string& name, const Vector2& position = Vector2::Zero);
};



#include "DataType.inl"

}