#pragma once
#include <cassert>
#include "Mathf.h"


/* TODO: Using array pointer instead of array to store data. This can improve the performance of construction */


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
	T val[2] = { static_cast<T>(0), static_cast<T>(0) };

public:
	/* Constructor */
	inline Vector2() = default;
	inline Vector2(T x, T y)
	{
		val[0] = x; val[1] = y;
	}
	inline Vector2(const Vector2<T>& other)
	{
		val[0] = other[0]; val[1] = other[1];
	}

	/* @brief Convert this instance to a new Vector2 with type "U" */
	template<typename U>
	inline Vector2<U> ConvertTo()
	{
		return Vector2<U>(static_cast<U>(val[0]), static_cast<U>(val[1]));
	}
 
	/*	@brief Calcualte the length of this instance. Return the result in "float" type by 
	 *		   default. (Since I was not able to use explicit template specialization to 
	 *		   specify return type for Vector<double> instances, I have to unify return 
	 *		   type to be "float" to prevent data loss) */
	inline float Length() const
	{
		float lengthSq = static_cast<float>(val[0] * val[0] + val[1] * val[1]);
		return sqrt(lengthSq);
	}
 
	/*	@brief Normalize this instance. Noted that normalize a vector with integer type might
	 *		   have incorrect result. Because the division result might be a float point number. */
	inline void Norm()
	{
		float len = Length();

		if (!IsZero(len))
		{
			val[0] = static_cast<T>(val[0] / len);
			val[1] = static_cast<T>(val[1] / len);
		}
	}
	/** @brief Get the normalization vector of this instance, but don't modify this instance */
	inline Vector2<float> GetNorm() const
	{
		float len = Length();

		if (IsZero(len))
			return Vector2<float>::Zero;
		else
			return Vector2<float>(
				static_cast<float>(val[0] / len),
				static_cast<float>(val[1] / len));
	}

	/* Self modifying operators */
	inline void operator+= (const Vector2<T>& other)
	{
		val[0] += other[0]; val[1] += other[1];
	}
	inline void operator-= (const Vector2<T>& other)
	{
		val[0] -= other[0]; val[1] -= other[1];
	}

	inline void operator*= (const Vector2<T>& other)
	{
		val[0] *= other[0]; val[1] *= other[1];
	}
	inline void operator*= (T num)
	{
		val[0] *= num; val[1] *= num;
	}

	inline void operator/= (const Vector2<T>& other) 
	{
		val[0] /= other[0]; val[1] /= other[1];
	}
	inline void operator/= (T num)
	{
		val[0] /= num; val[1] /= num;
	}

	/* Modifying operators */
	inline Vector2<T> operator+ (const Vector2<T>& other) const
	{
		return Vector2<T>(val[0] + other[0], val[1] + other[1]);
	}
	inline Vector2<T> operator- (const Vector2<T>& other) const
	{
		return Vector2<T>(val[0] - other[0], val[1] - other[1]);
	}

	inline Vector2<T> operator* (const Vector2<T>& other) const
	{
		return Vector2<T>(val[0] * other[0], val[1] * other[1]);
	}
	inline Vector2<T> operator* (T num) const
	{
		return Vector2<T>(val[0] * num, val[1] * num);
	}

	inline Vector2<T> operator/ (const Vector2<T>& other) const
	{
		return Vector2<T>(val[0] / other[0], val[1] / other[1]);
	}
	inline Vector2<T> operator/ (T num) const
	{
		return Vector2<T>(val[0] / num, val[1] / num);
	}
	inline friend Vector2<T> operator* (T left, Vector2<T> right) 
	{ 
		return Vector2<T>(left * right[0], left * right[1]); 
	}

	/* Assignment operators */
	inline Vector2<T>& operator= (const Vector2<T>& other)
	{
		val[0] = other[0]; val[1] = other[1];
		return *this;
	}

	/* Comparison operators */
	inline bool operator== (const Vector2<T>& other) const
	{
		return AreEqual(static_cast<float>(val[0]), static_cast<float>(other[0])) == true &&
			   AreEqual(static_cast<float>(val[1]), static_cast<float>(other[1])) == true;
	}
	inline bool operator!= (const Vector2<T>& other) const
	{
		return AreEqual(static_cast<float>(val[0]), static_cast<float>(other[0])) == false ||
			   AreEqual(static_cast<float>(val[1]), static_cast<float>(other[1])) == false;
	}

	/* Negate */
	inline Vector2<T> operator- (void) const
	{
		return Vector2<T>(-val[0], -val[1]);
	}

	/* Indexing */
	inline T& operator[] (int idx)
	{
		return val[idx];
	}
	inline const T& operator[] (int idx) const
	{
		return val[idx];
	}


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
	T val[3] = { static_cast<T>(0), static_cast<T>(0), static_cast<T>(0) };

public:
	/* Constructor */
	inline Vector3() = default;
	inline Vector3(T x, T y, T z)
	{
		val[0] = x; val[1] = y; val[2] = z;
	}
	inline Vector3(const Vector3<T>& other)
	{
		val[0] = other[0]; val[1] = other[1]; val[2] = other[2];
	}

	/* @brief Convert this instance to a new Vector3 with type "U" */
	template <typename U>
	inline Vector3<U> ConvertTo()
	{
		return Vector3<U>(
			static_cast<U>(val[0]),
			static_cast<U>(val[1]),
			static_cast<U>(val[2]));
	}

	inline T Dot(const Vector3<T>& other) const
	{
		return val[0] * other[0] + val[1] * other[1] + val[2] * other[2];
	}

	inline Vector3<T> Cross(const Vector3<T>& other) const
	{
		return Vector3<T>(
			val[1] * other[2] - val[2] * other[1],
			val[2] * other[0] - val[0] * other[2],
			val[0] * other[1] - val[1] * other[0]);
	}
 
	/*	@brief Calcualte the length of this instance. Return the result in "float" type by
	 *		   default. (Since I was not able to use explicit template specialization to
	 *		   specify return type for Vector<double> instances, I have to unify return
	 *		   type to be "float" to prevent data loss). */
	inline float Length() const
	{
		float lengthSq = static_cast<float>(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]);
		return sqrt(lengthSq);
	}
	
 
	/*	@brief Normalize this instance. Noted that normalize a vector with type "int" might 
	 *		   have incorrect result. Because the division result might be a float point number. */
	inline void Norm()
	{
		float len = Length();

		if (!IsZero(len))
		{
			val[0] = static_cast<T>(val[0] / len);
			val[1] = static_cast<T>(val[1] / len);
			val[2] = static_cast<T>(val[2] / len);
		}
	}
	/** @brief Get the normalization vector of this instance, but don't modify this instance */
	inline Vector3<float> GetNorm() const
	{
		float len = Length();

		if (IsZero(len))
			return Vector3<float>::Zero;
		else
			return Vector3<float>(
				static_cast<float>(val[0] / len),
				static_cast<float>(val[1] / len),
				static_cast<float>(val[2] / len));
	}

	/* Self modifying operators */
	inline void operator+= (const Vector3<T>& other)
	{
		val[0] += other[0]; val[1] += other[1]; val[2] += other[2];
	}
	inline void operator-= (const Vector3<T>& other)
	{
		val[0] -= other[0]; val[1] -= other[1]; val[2] -= other[2];
	}

	inline void operator*= (const Vector3<T>& other)
	{
		val[0] *= other[0]; val[1] *= other[1]; val[2] *= other[2];
	}
	inline void operator*= (T num)
	{
		val[0] *= num; val[1] *= num; val[2] *= num;
	}

	inline void operator/= (const Vector3<T>& other)
	{
		val[0] /= other[0]; val[1] /= other[1]; val[2] /= other[2];
	}
	inline void operator/= (T num)
	{
		val[0] /= num; val[1] /= num; val[2] /= num;
	}

	/* Modifying operators */
	inline Vector3<T> operator+ (const Vector3<T>& other) const
	{
		return Vector3<T>(val[0] + other[0], val[1] + other[1], val[2] + other[2]);
	}
	inline Vector3<T> operator- (const Vector3<T>& other) const
	{
		return Vector3<T>(val[0] - other[0], val[1] - other[1], val[2] - other[2]);
	}

	inline Vector3<T> operator* (const Vector3<T>& other) const
	{
		return Vector3<T>(val[0] * other[0], val[1] * other[1], val[2] * other[2]);
	}
	inline Vector3<T> operator* (T num) const
	{
		return Vector3(val[0] * num, val[1] * num, val[2] * num);
	}
	inline friend Vector3<T> operator* (T left, Vector3<T> right) 
	{ 
		return Vector3<T>(left * right[0], left * right[1], left * right[2]); 
	}

	inline Vector3<T> operator/ (const Vector3<T>& other) const
	{
		return Vector3<T>(val[0] / other[0], val[1] / other[1], val[2] / other[2]);
	}
	inline Vector3<T> operator/ (T num) const
	{
		return Vector3<T>(val[0] / num, val[1] / num, val[2] / num);
	}

	/* Assignment operators */
	inline Vector3<T>& operator= (const Vector3<T>& other)
	{
		val[0] = other[0]; val[1] = other[1]; val[2] = other[2];
		return *this;
	}

	/* Comparison operators */
	inline bool operator== (const Vector3<T>& other) const
	{
		return AreEqual(static_cast<float>(val[0]), static_cast<float>(other[0])) == true &&
			   AreEqual(static_cast<float>(val[1]), static_cast<float>(other[1])) == true &&
			   AreEqual(static_cast<float>(val[2]), static_cast<float>(other[2])) == true;
	}
	inline bool operator!= (const Vector3<T>& other) const
	{
		return AreEqual(static_cast<float>(val[0]), static_cast<float>(other[0])) == false ||
			   AreEqual(static_cast<float>(val[1]), static_cast<float>(other[1])) == false ||
			   AreEqual(static_cast<float>(val[2]), static_cast<float>(other[2])) == false;
	}

	/* Negate */
	inline Vector3<T> operator- (void) const
	{
		return Vector3<T>(-val[0], -val[1], -val[2]);
	}

	/* Indexing */
	inline T& operator[] (int idx)
	{
		return val[idx];
	}
	inline const T& operator[] (int idx) const
	{
		return val[idx];
	}


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
	T val[4] = {
		static_cast<T>(0), static_cast<T>(0), 
		static_cast<T>(0), static_cast<T>(0) };

public:
	inline Vector4() = default;
	inline Vector4(T w, T x, T y, T z)
	{
		val[0] = w, val[1] = x, val[2] = y, val[3] = z;
	}
	inline Vector4(const Vector4<T>& other)
	{
		val[0] = other[0], val[1] = other[1];
		val[2] = other[2], val[3] = other[3];
	}

	/* @brief Convert this instance to a new Vector4 with type "U" */
	template<typename U>
	inline Vector4<U> ConvertTo()
	{
		return Vector4<T>(
			static_cast<T>(val[0]), static_cast<T>(val[1]), 
			static_cast<T>(val[2]), static_cast<T>(val[3]));
	}

	/* Indexing */
	inline T& operator[] (int idx)
	{
		return val[idx];
	}
	inline const T& operator[] (int idx) const
	{
		return val[idx];
	}

	/* Assignment */
	inline bool operator== (const Vector4<T>& other) const
	{
		return (
			AreEqual(static_cast<float>(val[0]), static_cast<float>(other[0])) == true &&
			AreEqual(static_cast<float>(val[1]), static_cast<float>(other[1])) == true &&
			AreEqual(static_cast<float>(val[2]), static_cast<float>(other[2])) == true &&
			AreEqual(static_cast<float>(val[3]), static_cast<float>(other[3])) == true);
	}


	inline Vector4<T> operator* (const Vector4<T>& other) const
	{
		return Vector4<T>(val[0] * other[0], val[1] * other[1], val[2] * other[2], val[3] * other[3]);
	}
	inline Vector4<T> operator* (T num) const
	{
		return Vector4(val[0] * num, val[1] * num, val[2] * num, val[3] * num);
	}

};


/********************************* Global APIs **************************************/
/* Following global APIs are implemented as function templates. Note that template
 * is not a class or a function, it is a "pattern" that the compiler uses to generate 
 * a family of classes or functions. To use function templates, both the template 
 * declaration and definition must be visible to the compiler at the point of instantiation. 
 * As a result, template functions are typically declared and implemented in the same file. 
 */

namespace Vector
{

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


/** 
 *	@brief Calcualte the distance between two vector. Return the result in "float" type by
 *		   default. (Since I was not able to use explicit template specialization to
 *		   specify return type for Vector<double>, I have to unify return type to be "float"
 *		   to prevent data loss)
 */
template <typename T>
inline float Distance(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
	float sq0 = static_cast<float>((lhs[0] - rhs[0]) * (lhs[0] - rhs[0]));
	float sq1 = static_cast<float>((lhs[1] - rhs[1]) * (lhs[1] - rhs[1]));
	return sqrt(sq0 + sq1);
}


template <typename T>
inline float Distance(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	float sq0 = static_cast<float>((lhs[0] - rhs[0]) * (lhs[0] - rhs[0]));
	float sq1 = static_cast<float>((lhs[1] - rhs[1]) * (lhs[1] - rhs[1]));
	float sq2 = static_cast<float>((lhs[2] - rhs[2]) * (lhs[2] - rhs[2]));
	return sqrt(sq0 + sq1 + sq2);
}


template <typename T>
inline Vector3<T> Cross(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	return Vector3<T>(
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}


/********************************* Unit tests **************************************/

#if defined(_DEBUG)

#include <cassert>

inline void Vector2UnitTest()
{
	Vector2<int> case0 = Vector2<int>(4, 3);
	Vector2<float> case1 = Vector2<float>(4.4f, 3.3f);
	Vector2<double> case2 = Vector2<double>(6.6, 8.8);


	Vector2<int> temp0;
	Vector2<float> temp1;
	Vector2<double> temp2;
	float val0;

	temp0 = case1.ConvertTo<int>();
	assert(temp0[0] == 4 && temp0[1] == 3);

	val0 = case0.Length();
	assert(val0 == 5.0f);
	val0 = case1.Length();
	assert(val0 == 5.5f);
	val0 = case2.Length();
	assert(val0 == 11.0f);

	temp0 = case0;
	temp0.Norm();
	assert(temp0[0] == 0 && temp0[1] == 0);
	temp1 = case1;
	temp1.Norm();
	assert(AreEqual(temp1[0], 0.8f) && AreEqual(temp1[1], 0.6f));

	temp1 = case0.GetNorm();
	assert(AreEqual(temp1[0], 0.8f) && AreEqual(temp1[1], 0.6f));
	temp1 = case2.GetNorm();
	assert(AreEqual(temp1[0], 0.6f) && AreEqual(temp1[1], 0.8f));

	temp1 = case1;
	temp1 += Vector2<float>(5.5f, 7.7f);
	assert(AreEqual(temp1[0], 9.9f) && AreEqual(temp1[1], 11.0f));
	temp1 -= Vector2<float>(1.1f, 0.0f);
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f));
	temp1 *= Vector2<float>(0.5f, 0.5f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f));
	temp1 *= 2.0f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f));
	temp1 /= Vector2<float>(2.0f, 2.0f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f));
	temp1 /= 0.5f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f));

	temp1 = case1;
	temp1 = temp1 + Vector2<float>(5.5f, 7.7f);
	assert(AreEqual(temp1[0], 9.9f) && AreEqual(temp1[1], 11.0f));
	temp1 = temp1 - Vector2<float>(1.1f, 0.0f);
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f));
	temp1 = temp1 * Vector2<float>(0.5f, 0.5f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f));
	temp1 = temp1 * 2.0f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f));
	temp1 = temp1 / Vector2<float>(2.0f, 2.0f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f));
	temp1 = temp1 / 0.5f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f));

	assert(temp1 == Vector2<float>(8.8f, 11.0f));
	assert(temp1 != Vector2<float>(8.8f, 0.0f));
	assert(-temp1 == Vector2<float>(-8.8f, -11.0f));

	temp0 = Vector2<int>(5, 6);
	temp1 = Vector2<float>(5.5f, 6.6f);
	assert(Vector::Dot(case0, temp0) == 38);
	assert(AreEqual(Vector::Distance(case0, temp0), 3.16227f));
	assert(AreEqual(Vector::Distance(case1, temp1), 3.47850f));
}


inline void Vector3UnitTest()
{
	Vector3<int> case0 = Vector3<int>(3, 4, 5);
	Vector3<float> case1 = Vector3<float>(3.3f, 4.4f, 5.5f);
	Vector3<double> case2 = Vector3<double>(6.6, 8.8, 9.9);


	Vector3<int> temp0;
	Vector3<float> temp1;
	Vector3<double> temp2;
	float val0;

	temp0 = case1.ConvertTo<int>();
	assert(temp0[0] == 3 && temp0[1] == 4 && temp0[2] == 5);

	val0 = case0.Length();
	assert(AreEqual(val0, 7.071067811f));
	val0 = case1.Length();
	assert(AreEqual(val0, 7.7781745f));
	val0 = case2.Length();
	assert(AreEqual(val0, 14.7989864517f));

	temp0 = case0;
	temp0.Norm();
	assert(temp0[0] == 0 && temp0[1] == 0 && temp0[2] == 0);
	temp1 = case1;
	temp1.Norm();
	assert(AreEqual(temp1[0], 0.42426406f) && AreEqual(temp1[1], 0.565685424f) && AreEqual(temp1[2], 0.70710678f));

	temp1 = case0.GetNorm();
	assert(AreEqual(temp1[0], 0.42426406f) && AreEqual(temp1[1], 0.565685424f) && AreEqual(temp1[2], 0.70710678f));
	temp1 = case2.GetNorm();
	assert(AreEqual(temp1[0], 0.44597648f) && AreEqual(temp1[1], 0.594635316f) && AreEqual(temp1[2], 0.66896473f));

	temp1 = case1;
	temp1 += Vector3<float>(6.6f, 6.6f, 4.4f);
	assert(AreEqual(temp1[0], 9.9f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[2], 9.9f));
	temp1 -= Vector3<float>(1.1f, 0.0f, 1.1f);
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[2], 8.8f));
	temp1 *= Vector3<float>(0.5f, 0.5f, 0.5f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f) && AreEqual(temp1[2], 4.4f));
	temp1 *= 2.0f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[2], 8.8f));
	temp1 /= Vector3<float>(2.0f, 2.0f, 2.0f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f) && AreEqual(temp1[2], 4.4f));
	temp1 /= 0.5f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[0], 8.8f));

	temp1 = case1;
	temp1 = temp1 + Vector3<float>(6.6f, 6.6f, 4.4f);
	assert(AreEqual(temp1[0], 9.9f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[2], 9.9f));
	temp1 = temp1 - Vector3<float>(1.1f, 0.0f, 1.1f);
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[2], 8.8f));
	temp1 = temp1 * Vector3<float>(0.5f, 0.5f, 0.5f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f) && AreEqual(temp1[2], 4.4f));
	temp1 = temp1 * 2.0f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[2], 8.8f));
	temp1 = temp1 / Vector3<float>(2.0f, 2.0f, 2.0f);
	assert(AreEqual(temp1[0], 4.4f) && AreEqual(temp1[1], 5.5f) && AreEqual(temp1[2], 4.4f));
	temp1 = temp1 / 0.5f;
	assert(AreEqual(temp1[0], 8.8f) && AreEqual(temp1[1], 11.0f) && AreEqual(temp1[0], 8.8f));

	assert(temp1 == Vector3<float>(8.8f, 11.0f, 8.8f));
	assert(temp1 != Vector3<float>(0.0f, 11.0f, 8.8f));
	assert(-temp1 == Vector3<float>(-8.8f, -11.0f, -8.8f));

	temp0 = Vector3<int>(5, 6, 7);
	temp1 = Vector3<float>(5.5f, 6.6f, 7.7f);
	assert(Vector::Dot(case0, temp0) == 74);
	assert(AreEqual(Vector::Distance(case0, temp0), 3.464102f));
	assert(AreEqual(Vector::Distance(case1, temp1), 3.810512f, 0.00001f));
}

#endif




}//Namespace Vector
}//Namespace Engine
