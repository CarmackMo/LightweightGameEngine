#pragma once
#include "Dependency.h"

using namespace std;

namespace Engine
{

class Vector3;
class Vector4;




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


#include "Vector.inl"

}