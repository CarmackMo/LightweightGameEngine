#pragma once
#include "Dependency.h"

using namespace std;
using namespace Engine::Debugger;

namespace Engine
{

template<typename T> class Vector2;
template<typename T> class Vector3;
template<typename T> class Vector4;




/**
 *	@brief 2-dimemsional vector. Can be treaded as a 1x2 row vector or
 *		   2x1 column vector base on actual needs. Only accept C/C++
 *		   numerical type as template (i.e. int, float, uint8, uint16...)
 */
template <typename T>
class Vector2
{
public:
	double x;
	double y;

	/* Constructor */
	inline Vector2();
	inline Vector2(T x, T y);
	inline Vector2(const Vector2<T>& vec);

	/* Self modifying operators */
	inline void operator+= (const Vector2<T>& vec);
	inline void operator-= (const Vector2<T>& vec);

	inline void operator*= (const Vector2<T>& vec);
	inline void operator*= (T num);

	inline void operator/= (const Vector2<T>& vec);
	inline void operator/= (T num);

	/* Modifying operators */
	inline Vector2<T> operator+ (const Vector2<T>& vec) const;
	inline Vector2<T> operator- (const Vector2<T>& vec) const;

	inline Vector2<T> operator* (const Vector2<T>& vec) const;
	inline Vector2<T> operator* (T num) const;

	inline Vector2<T> operator/ (const Vector2<T>& vec) const;
	inline Vector2<T> operator/ (T num) const;

	/* Assignment operators */
	inline Vector2<T>& operator= (const Vector2<T>& vec);

	/* Comparison operators */
	inline bool operator== (const Vector2<T>& vec) const;
	inline bool operator!= (const Vector2<T>& vec) const;

	/* Negate */
	inline Vector2<T> operator- (void) const;

	/* Must implement here. Otherwise, the compiler cannot find the specific
	 * template instance, and will report a LNK2019 error */
	inline friend Vector2<T> operator* (T left, Vector2<T> right) { return Vector2<T>(left * right.x, left * right.y); }

	/* @brief Vector2(0, 0) */
	static const Vector2<T> Zero;
	/* @brief Vector2(-1, 0) */
	static const Vector2<T> Left;
	/* @brief Vector2(1, 0) */
	static const Vector2<T> Right;
	/* @brief Vector2(0, 1) */
	static const Vector2<T> Up;
	/* @brief Vector2(0, -1) */
	static const Vector2<T> Down;
};


/**
 *	@brief 3-dimemsional vector. Can be treaded as a 1x3 row vector or
 *		   3x1 column vector base on actual needs. Only accept C/C++
 *		   numerical type as template (i.e. int, float, uint8, uint16...)
 */
template <typename T>
class Vector3
{
public:
	/* Constructor */
	inline Vector3();
	inline Vector3(T x, T y, T z);
	inline Vector3(const Vector3<T>& other);

	/* Convert a vector3 with type "U" to type "T" */
	template<typename U>
	inline static Vector3<T> ConvertType(const Vector3<U>& other);

	/* Self modifying operators */
	inline void operator+= (const Vector3<T>& other);
	inline void operator-= (const Vector3<T>& other);

	inline void operator*= (const Vector3<T>& other);
	inline void operator*= (T num);

	inline void operator/= (const Vector3<T>& other);
	inline void operator/= (T num);

	/* Modifying operators */
	inline Vector3<T> operator+ (const Vector3<T>& other) const;
	inline Vector3<T> operator- (const Vector3<T>& other) const;

	inline Vector3<T> operator* (const Vector3<T>& other) const;
	inline Vector3<T> operator* (T num) const;

	inline Vector3<T> operator/ (const Vector3<T>& other) const;
	inline Vector3<T> operator/ (T num) const;

	/* Assignment operators */
	inline Vector3<T>& operator= (const Vector3<T>& other);

	/* Comparison operators */
	inline bool operator== (const Vector3<T>& other) const;
	inline bool operator!= (const Vector3<T>& other) const;

	/* Negate */
	inline Vector3<T> operator- (void) const;

	/* Indexing */
	inline T& operator[] (int idx);
	inline const T& operator[] (int idx) const;

	/* Must implement here. Otherwise, the compiler cannot find the specific
	 * template instance, and will report a LNK2019 error */
	inline friend Vector3<T> operator* (T left, Vector3<T> right) { return Vector3<T>(left * right[0], left * right[1], left * right[2]); }

	/* @brief Vector3(0, 0, 0) */
	static const Vector3<T> Zero;
	/* @brief Vector3(-1, 0, 0) */
	static const Vector3<T> Left;
	/* @brief Vector3(1, 0, 0) */
	static const Vector3<T> Right;
	/* @brief Vector3(0, 1, 0) */
	static const Vector3<T> Up;
	/* @brief Vector3(0, -1, 0) */
	static const Vector3<T> Down;
	/* @brief Vector3(0, 0, 1) */
	static const Vector3<T> Front;
	/* @brief Vector3(0, 0, -1) */
	static const Vector3<T> Back;

private:
	T val[3];
};


/**
 *	@brief 4-dimemsional vector. Can be treaded as a 1x4 row vector or
 *		   4x1 column vector base on actual needs. Only accept C/C++
 *		   numerical type as template (i.e. int, float, uint8, uint16...)
 */
template<typename T>
class Vector4
{
public:
	inline Vector4();
	inline Vector4(T w, T x, T y, T z);
	inline Vector4(const Vector4<T>& other);

	/* Convert a vector4 with type "U" to type "T" */
	template<typename U>
	inline static Vector4<T> ConvertType(const Vector4<U>& other);

	inline T& operator[] (int idx);
	inline const T& operator[] (int idx) const;

private:
	T val[4];
};





template <typename T>
class Vector5
{
public:
	T val[4];

	inline Vector5()
	{
		val[0] = static_cast<T>(0), val[1] = static_cast<T>(0),
			val[2] = static_cast<T>(0), val[3] = static_cast<T>(0);
	}

	inline Vector5(T w, T x, T y, T z)
	{
		val[0] = w, val[1] = x, val[2] = y, val[3] = z;
	}

	inline Vector5(const Vector5<T>& other)
	{
		val[0] = other[0], val[1] = other[1],
			val[2] = other[2], val[3] = other[3];
	}

	double& operator[] (int idx);
	const double& operator[] (int idx) const;

	static const Vector5<double> Up;
	static const Vector5<int> Down;

};




#include "Vector.inl"

}