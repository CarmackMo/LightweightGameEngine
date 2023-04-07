#pragma once
#include "Dependency.h"
#include "Vector.h"

using namespace std;

namespace Engine
{

class Matrix4;



/**
 *	@brief 4x4 Matrix. The index of element in top-left corner is (0,0).
 *		   Down and right is the incremental direction of rows and 
 *		   column resepctively.
 */
class Matrix4
{
public:
	/* Value base constructure */
	inline Matrix4(
		double x00, double x01, double x02, double x03,
		double x10, double x11, double x12, double x13,
		double x20, double x21, double x22, double x23,
		double x30, double x31, double x32, double x33);
	/* Copy constructure */
	inline Matrix4(const Matrix4& other);

	/* Return the value at M(i,j) */
	inline double At(int row, int col) const;
	/* Update the value at M(i,j) */
	inline void Set(int row, int col, double data);
	/* Calculate the determinant of the 3x3 sub-matrix where M(row, col) is the pivot */
	double Det(int row, int col) const;

	/* Invert this instance */
	void Invert(void);
	/* Return a matrix that is the inverse of this instance but don't modify this instance */
	inline Matrix4 GetInverse(void) const;

	/* Transpose this instance */
	inline void Transpose(void);
	/* Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix4 GetTranspose(void) const;

	/* Multiply vector by matrix */
	/* Return v * M */
	Vector4 MultiplyLeft(const Vector4& vec) const;
	/* Return M * v */
	Vector4 MultiplyRight(const Vector4& vec) const;

	/* Multiply matrix by matrix, return this * other */
	Matrix4 operator* (const Matrix4& other) const;
	/* Assignment operators */
	inline Matrix4& operator=(const Matrix4& other);
	/* Comparison operators */
	bool operator== (const Matrix4& other) const;


	/* Identity matrix creator */
	inline static Matrix4 CreateIdentity(void);

	/* Rotation matrix creator */
	/* Rotate matrix around X-axis, rotate counter-clockwise */
	inline static Matrix4 CreateXRotation(double rad);
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	inline static Matrix4 CreateYRotation(double rad);
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	inline static Matrix4 CreateZRotation(double rad);

	/* Translation matrix creator */
	inline static Matrix4 CreateTranslation(Vector3& vec);
	inline static Matrix4 CreateTranslation(double transX, double transY, double transZ);

	/* Scale matrix creator */
	inline static Matrix4 CreateScale(Vector3& vec);
	inline static Matrix4 CreateScale(double scaleX, double scaleY, double scaleZ);

private:
	double val[4][4];

	inline Matrix4();
};

#include "Matrix.inl"

}