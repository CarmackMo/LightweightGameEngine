#pragma once
#include "Dependency.h"
#include "Vector.h"

using namespace std;

namespace Engine
{

template <typename T> class Matrix4;



/**
 *	@brief 4x4 Matrix. The index of element in top-left corner is (0,0). Down and right is 
 *		   the incremental direction of rows and column resepctively. Only accept C/C++
 *		   numerical primitive built-in type as template (i.e. int, float, uint8, uint16...)
 */
template <typename T>
class Matrix4
{
public:
	inline Matrix4();

	inline Matrix4(
		T x00, T x01, T x02, T x03,
		T x10, T x11, T x12, T x13,
		T x20, T x21, T x22, T x23,
		T x30, T x31, T x32, T x33);

	inline Matrix4(const Matrix4<T>& other);

	///* Return the value at M(i,j) */
	//inline double At(int row, int col) const;
	///* Update the value at M(i,j) */
	//inline void Set(int row, int col, double num);
	/* Calculate the determinant of the 3x3 sub-matrix where M(row, col) is the pivot */
	T Det(int row, int col) const;

	/* Invert this instance */
	void Invert(void);
	/* Return a matrix that is the inverse of this instance but don't modify this instance */
	inline Matrix4<T> GetInverse(void) const;

	/* Transpose this instance */
	inline void Transpose(void);
	/* Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix4<T> GetTranspose(void) const;

	/* Multiply vector by matrix */
	/* Return v * M */
	Vector4<T> MultiplyLeft(const Vector4<T>& vec) const;
	/* Return M * v */
	Vector4<T> MultiplyRight(const Vector4<T>& vec) const;

	inline T* operator[](int row);
	inline Matrix4<T> operator+ (const Matrix4<T>& other) const;
	inline Matrix4<T> operator- (const Matrix4<T>& other) const;
	inline Matrix4<T> operator* (double num) const;
	inline Matrix4<T> operator/ (double num) const;
	inline Matrix4<T>& operator= (const Matrix4<T>& other);
	/* Multiply matrix by matrix, return this instance * other */
	Matrix4<T> operator* (const Matrix4<T>& other) const;
	bool operator== (const Matrix4<T>& other) const;


	/* Identity matrix creator */
	inline static Matrix4<T> CreateIdentity(void);

	/* Rotation matrix creator */
	/* Rotate matrix around X-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateXRotation(double rad);
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateYRotation(double rad);
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateZRotation(double rad);

	/* Translation matrix creator */
	inline static Matrix4<T> CreateTranslation(Vector3& vec);
	inline static Matrix4<T> CreateTranslation(double transX, double transY, double transZ);

	/* Scale matrix creator */
	inline static Matrix4<T> CreateScale(Vector3& vec);
	inline static Matrix4<T> CreateScale(double scaleX, double scaleY, double scaleZ);

	template<typename U>
	inline static Matrix4<T> CovertType(const Matrix4<U>& other);

private:
	T val[4][4];
};

#include "Matrix.inl"

}