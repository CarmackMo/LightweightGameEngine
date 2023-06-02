#pragma once
#include "Debugger.h"
#include "Vector.h"

using namespace std;
using namespace Engine::Debugger;


namespace Engine
{
/* Forwared declaration */
template <typename T> class Matrix3;
template <typename T> class Matrix4;



/**
 *	@brief 3x3 Matrix. The index of element in top-left corner is (0,0). Down and right is
 *		   the incremental direction of rows and column resepctively. Only accept C/C++
 *		   numerical type as template (i.e. int, float, uint8, uint16...)
 */
template <typename T> 
class Matrix3
{
private:
	T val[3][3];

public:
	/* Constructor */
	inline Matrix3();
	inline Matrix3(
		T x00, T x01, T x02,
		T x10, T x11, T x12,
		T x20, T x21, T x22);
	inline Matrix3(const Matrix3<T>& other);

	/* Get the specific row / column of the matrix */
	inline Vector3<T> GetRow(int row) const;
	inline Vector3<T> GetCol(int col) const;

	/* Calculate the determinant of the 2x2 minor matrix where M(row, col) is the pivot */
	T Det(int row, int col) const;

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result.
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void);
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance. */
	inline Matrix3<T> GetInverse() const;
	/* @brief Same functionality of Matrix<T>::GetInverse(). This function can avoid incorrect
	 *		  result due to inappropriate data types. (e.g. the inverse of a <int> matrix might
	 *		  has <float> result) */
	template <typename U>
	inline Matrix3<U> GetInverse() const;

	/* Transpose this instance */
	inline void Transpose(void);
	/* Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix3<T> GetTranspose(void) const;

	/* Return a transform matrix of this instance */
	inline Matrix4<T> GetTransform() const;

	/* Convert current to a new matrix with type "U" */
	template <typename U>
	inline Matrix3<U> CovertToType();

	/* Multiply vector by matrix */
	/* Return v * M */
	Vector3<T> MultiplyLeft(const Vector3<T>& vec) const;
	/* Return M * v */
	Vector3<T> MultiplyRight(const Vector3<T>& vec) const;


	inline T* operator[] (int row);
	inline const T* operator[] (int row) const;
	inline Matrix3<T> operator+ (const Matrix3<T>& other) const;
	inline Matrix3<T> operator- (const Matrix3<T>& other) const;
	inline Matrix3<T> operator* (T num) const;
	inline Matrix3<T> operator/ (T num) const;
	inline Matrix3<T>& operator= (const Matrix3<T>& other);
	/* Multiply matrix by matrix, return this instance * other */
	Matrix3<T> operator* (const Matrix3<T>& other) const;

	bool operator== (const Matrix3<T>& other) const;


	/* Identity matrix creator */
	inline static Matrix3<T> CreateIdentity(void);

	/* @brief Rotate matrix around origin point, rotate counter-clockwise */
	inline static Matrix3<T> CreateXRotation(double rad);
	inline static Matrix3<T> CreateYRotation(double rad);
	inline static Matrix3<T> CreateZRotation(double rad);

	/* Translation matrix creator */
	inline static Matrix3<T> CreateTranslation(const Vector2<T>& vec);
	inline static Matrix3<T> CreateTranslation(T transX, T transY);

	/* Scale matrix creator */
	inline static Matrix3<T> CreateScale(const Vector2<T>& vec);
	inline static Matrix3<T> CreateScale(T scaleX, T scaleY);
};



/**
 *	@brief 4x4 Matrix. The index of element in top-left corner is (0,0). Down and right is 
 *		   the incremental direction of rows and column resepctively. Only accept C/C++
 *		   numerical type as template (i.e. int, float, uint8, uint16...)
 */
template <typename T>
class Matrix4
{
private:
	T val[4][4];

public:
	/* Constructure */
	inline Matrix4();
	inline Matrix4(
		T x00, T x01, T x02, T x03,
		T x10, T x11, T x12, T x13,
		T x20, T x21, T x22, T x23,
		T x30, T x31, T x32, T x33);
	inline Matrix4(const Matrix4<T>& other);

	/* Calculate the determinant of the 3x3 minor matrix where M(row, col) is the pivot */
	T Det(int row, int col) const;

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result. 
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void);
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance. */
	inline Matrix4<T> GetInverse() const;
	/* @brief Same functionality of Matrix<T>::GetInverse(). This function can avoid incorrect 
	 *		  result due to inappropriate data types. (e.g. the inverse of a <int> matrix might 
	 *		  has <float> result) */
	template <typename U>
	inline Matrix4<U> GetInverse() const;

	/* Return a matrix that is the inverse ASSUMING this matrix has axis rotation and translation only. */
	inline Matrix4<T> GetInverseRotTrans() const;

	/* Transpose this instance */
	inline void Transpose(void);
	/* Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix4<T> GetTranspose(void) const;

	/* Convert current to a new matrix with type "U" */
	template <typename U>
	inline Matrix4<U> CovertToType();

	/* Multiply vector by matrix */
	/* Return v * M */
	Vector4<T> MultiplyLeft(const Vector4<T>& vec) const;
	/* Return M * v */
	Vector4<T> MultiplyRight(const Vector4<T>& vec) const;

	/* Transform point counter clockwise */
	inline Vector3<T> TransformPoint(const Vector3<T>& point) const;

	inline T* operator[] (int row);
	inline const T* operator[] (int row) const;
	inline Matrix4<T> operator+ (const Matrix4<T>& other) const;
	inline Matrix4<T> operator- (const Matrix4<T>& other) const;
	inline Matrix4<T> operator* (T num) const;
	inline Matrix4<T> operator/ (T num) const;
	inline Matrix4<T>& operator= (const Matrix4<T>& other);
	/* Multiply matrix by matrix, return this instance * other */
	Matrix4<T> operator* (const Matrix4<T>& other) const;

	bool operator== (const Matrix4<T>& other) const;


	/* Identity matrix creator */
	inline static Matrix4<T> CreateIdentity(void);

	/* @brief Rotate matrix around X-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateXRotation(double rad);
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateYRotation(double rad);
	/* @brief Rotate matrix around Z-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateZRotation(double rad);

	/* Translation matrix creator */
	inline static Matrix4<T> CreateTranslation(const Vector3<T>& vec);
	inline static Matrix4<T> CreateTranslation(T transX, T transY, T transZ);

	/* Scale matrix creator */
	inline static Matrix4<T> CreateScale(const Vector3<T>& vec);
	inline static Matrix4<T> CreateScale(T scaleX, T scaleY, T scaleZ);
};





namespace Matrix
{
/********************************* Unit tests **************************************/

#if defined(_DEBUG)

#include <cassert>

inline void Matrix4UnitTest()
{
	Matrix4<double> sample = Matrix4<double>(
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	);

	/* Operator Test */
	Matrix4<double> temp1;
	temp1 = sample + Matrix4<double>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp1 == Matrix4<double>(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17));
	temp1 = sample - Matrix4<double>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp1 == Matrix4<double>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
	temp1 = sample * 2;
	assert(temp1 == Matrix4<double>(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));
	temp1 = sample * Matrix4<double>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	assert(temp1 == Matrix4<double>(90, 100, 110, 120, 202, 228, 254, 280, 314, 356, 398, 440, 426, 484, 542, 600));
	assert(2 == sample[0][1]);

	/* Math Operation Test */
	Vector4<double> temp2;
	temp2 = sample.MultiplyLeft(Vector4<double>(1, 2, 3, 4));
	assert(temp2 == Vector4<double>(90, 100, 110, 120));
	temp2 = sample.MultiplyRight(Vector4<double>(1, 2, 3, 4));
	assert(temp2 == Vector4<double>(30, 70, 110, 150));
	temp1 = sample.GetTranspose();
	assert(temp1 == Matrix4<double>(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16));
	sample = Matrix4<double>(1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, 1);
	temp1 = sample.GetInverse();
	assert(temp1 == Matrix4<double>(0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, 0.25));
}

#endif



}//Namespace Matrix
}//Namespace Engine