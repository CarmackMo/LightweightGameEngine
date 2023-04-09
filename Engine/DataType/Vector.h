#pragma once
#include "Dependency.h"

using namespace std;

namespace Engine
{

class Vector3;
template<typename T> class Vector4;




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


/**
 *	@brief 4-dimemsional vector. Can be treaded as a 1x4 row vector or
 *		   4x1 column vector base on actual needs. Only accept C/C++
 *		   numerical type as template (i.e. int, float, uint8, uint16...)
 */
template<typename T>
class Vector4
{
public:
	T val[4];

	inline Vector4();
	inline Vector4(T w, T x, T y, T z);
	inline Vector4(const Vector4<T>& other);

	inline T& operator[] (int idx);
	inline const T& operator[] (int idx) const;

	/* Convert a vector4 with type "U" to type "T" */
	template<typename U>
	inline static Vector4<T> ConvertType(const Vector4<U>& other);

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