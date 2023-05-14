#pragma once
#include "Dependency.h"
#include "Mathd.h"


using namespace std;
using namespace Engine::Debugger;

namespace Engine
{
/* Forwared declaration */
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
private:
	T val[2];

public:
	/* Constructor */
	inline Vector2();
	inline Vector2(T x, T y);
	inline Vector2(const Vector2<T>& other);

	/* Convert this instance to a new Vector2 with type "U" */
	template<typename U>
	inline Vector2<U> ConvertTo();

	/* Calcualte the length of this instance. Return the result in "float" type by 
	 * default. (Since I was not able to use explicit template specialization to 
	 * specify return type for Vector<double> instances, I have to unify return 
	 * type to be "float" to prevent data loss)*/
	inline float Length() const;


	/* Normalize this instance. Noted that normalize a vector with integer type might
	 * have incorrect result. Because the division result might be a float point number.*/
	inline void Norm();
	/* Get the normalization vector of this instance, but don't modify this instance */
	inline Vector2<float> GetNorm() const;

	/* Self modifying operators */
	inline void operator+= (const Vector2<T>& other);
	inline void operator-= (const Vector2<T>& other);

	inline void operator*= (const Vector2<T>& other);
	inline void operator*= (T num);

	inline void operator/= (const Vector2<T>& other);
	inline void operator/= (T num);

	/* Modifying operators */
	inline Vector2<T> operator+ (const Vector2<T>& other) const;
	inline Vector2<T> operator- (const Vector2<T>& other) const;

	inline Vector2<T> operator* (const Vector2<T>& other) const;
	inline Vector2<T> operator* (T num) const;

	inline Vector2<T> operator/ (const Vector2<T>& other) const;
	inline Vector2<T> operator/ (T num) const;

	/* Assignment operators */
	inline Vector2<T>& operator= (const Vector2<T>& other);

	/* Comparison operators */
	inline bool operator== (const Vector2<T>& other) const;
	inline bool operator!= (const Vector2<T>& other) const;

	/* Negate */
	inline Vector2<T> operator- (void) const;

	/* Indexing */
	inline T& operator[] (int idx);
	inline const T& operator[] (int idx) const;

	/* Must implement here. Otherwise, the compiler cannot find the specific
	 * template instance, and will report a LNK2019 error */
	inline friend Vector2<T> operator* (T left, Vector2<T> right) { return Vector2<T>(left * right[0], left * right[1]); }

	/* Static constant instances must be implemented explicitly for each 
	 * template type. Since compiler cannot automatically determine the type 
	 * of each static instance */

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
private:
	T val[3];

public:
	/* Constructor */
	inline Vector3();
	inline Vector3(T x, T y, T z);
	inline Vector3(const Vector3<T>& other);

	/* Convert a vector3 with type "U" to type "T" */
	template <typename U>
	inline Vector3<U> CovertTo();

	inline T Dot(const Vector3<T>& other) const;
	inline Vector3<T> Cross(const Vector3<T>& other) const;

	/* Calcualte the length of this instance. Return the result in "float" type by
	 * default. (Since I was not able to use explicit template specialization to
	 * specify return type for Vector<double> instances, I have to unify return
	 * type to be "float" to prevent data loss)*/
	inline float Length() const;
	
	/* Normalize this instance. Noted that normalize a vector with type "int" might 
	 * have incorrect result. Because the division result might be a float point number.*/
	inline void Norm();
	/* Get the normalization vector of this instance, but don't modify this instance */
	inline Vector3<float> GetNorm() const;

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

	/* Static constant instances must be implemented explicitly for each
	 * template type. Since complier cannot automatically detemine the type
	 * of each static instance */

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
};


/**
 *	@brief 4-dimemsional vector. Can be treaded as a 1x4 row vector or
 *		   4x1 column vector base on actual needs. Only accept C/C++
 *		   numerical type as template (i.e. int, float, uint8, uint16...)
 */
template<typename T>
class Vector4
{
private:
	T val[4];

public:
	inline Vector4();
	inline Vector4(T w, T x, T y, T z);
	inline Vector4(const Vector4<T>& other);

	/* Convert a vector4 with type "U" to type "T" */
	template<typename U>
	inline static Vector4<T> ConvertType(const Vector4<U>& other);

	inline T& operator[] (int idx);
	inline const T& operator[] (int idx) const;
	inline bool operator== (const Vector4<T>& other) const;


	inline Vector4<T> operator* (const Vector4<T>& other) const;
	inline Vector4<T> operator* (T num) const;

};



/* TODO */
template <typename T>
inline T Dot(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	return lhs[0] * rhs[0] + lhs[1] * rhs[1];
}


template <typename T>
inline T Dot(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}


template <typename T>
inline float Distance(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	float sq0 = static_cast<float>((lhs[0] - rhs[0]) * (lhs[0] - rhs[0]));
	float sq1 = static_cast<float>((lhs[1] - rhs[1]) * (lhs[1] - rhs[1]));
	return sqrt(sq0 + sq1);
}


inline double Distance(const Vector2<double>& lhs, const Vector2<int>& rhs)
{
	double sq0 = static_cast<double>((lhs[0] - rhs[0]) * (lhs[0] - rhs[0]));
	double sq1 = static_cast<double>((lhs[1] - rhs[1]) * (lhs[1] - rhs[1]));
	return sqrt(sq0 + sq1);
}


template <typename T>
inline T Distance(const Vector3<T>& lhs, const Vector3<T>& rhs);


template <typename T>
inline Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs);





#if defined(_DEBUG)

inline void Vector2UnitTest()
{
	Vector2<double> temp0 = Vector2<double>(4, 3);
	Vector2<float> temp1 = Vector2<float>(6, 8);
	Vector2<int> temp3 = Vector2<int>(2, 8);


}
#endif




}