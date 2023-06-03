#pragma once
#include <type_traits>
#include "Debugger.h"
#include "Vector.h"
#include "Mathf.h"

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

	/* @brief Calculate the determinant of the 2x2 minor matrix using M[row][col] as the pivot.
	 *		  To calculate the actual determinant of a Matrix3 instance, can refer to following formular: 
	 *		  det = M[0][0]*Det(0,0) - M[0][1]*Det(0,1) + M[0][2]*Det(0,2), using first row elements
	 *		  as pivots */
	T Det(int row, int col) const;

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result.
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void);
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance. User 
	 *		  needs to sepcify the return type to prevent incorrect result due to inappropriate data 
	 *		  types. (e.g. the inverse of a <int> matrix might has <float> result, if return the result 
	 *		  in its original type, the result will be incorrect) */
	template <typename U>
	inline Matrix3<U> GetInverse() const;
	///* @brief Return a matrix that is the inverse of this instance but don't modify this instance. */
	//inline Matrix3<T> GetInverse() const;

	/* @brief Transpose this instance */
	inline void Transpose(void);
	/* @brief Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix3<T> GetTranspose(void) const;

	/* @brief Convert this instance to a new matrix with type "U", but don't modify this instance */
	template <typename U>
	inline Matrix3<U> ConvertToType() const;

	///* Multiply vector by matrix */
	///* @brief Return vec * Mtx */
	//Vector3<T> MultiplyLeft(const Vector3<T>& vec) const;
	///* @brief Return Mtx * vec */
	//Vector3<T> MultiplyRight(const Vector3<T>& vec) const;

	inline T* operator[] (int row);
	inline const T* operator[] (int row) const;
	inline Matrix3<T> operator+ (const Matrix3<T>& other) const;
	inline Matrix3<T> operator- (const Matrix3<T>& other) const;
	inline Matrix3<T> operator* (T num) const;
	inline Matrix3<T> operator/ (T num) const;
	inline Matrix3<T>& operator= (const Matrix3<T>& other);
	/* @brief Multiply matrix by matrix */
	Matrix3<T> operator* (const Matrix3<T>& other) const;

	bool operator== (const Matrix3<T>& other) const;


	/* @brief Convert this instance into a 4x4 transformation matrix. The additional row and column 
	 *		  represents translations and homogeneous coordinates respectively. Mathematically,
	 *		  this convertion is known as an affine transformation, allowing the original matrix
	 *		  to be used for 3D math in a homogenous coordinate system. */
	inline Matrix4<T> ToTransform() const;

	/* @brief Apply this instance as a transform matrix to the input point, return a new vector that
	*		  represents the transformed point. (The point is transformed clockwise). */
	inline Vector2<T> TransformPoint(const Vector2<T>& point) const;

	/* @brief Identity matrix creator */
	inline static Matrix3<T> CreateIdentity(void);

	/* @brief Rotate matrix around origin point, rotate counter-clockwise */
	inline static Matrix3<T> CreateXRotation(double rad);
	inline static Matrix3<T> CreateYRotation(double rad);
	inline static Matrix3<T> CreateZRotation(double rad);

	/* @brief Translation matrix creator */
	inline static Matrix3<T> CreateTranslation(const Vector2<T>& vec);
	inline static Matrix3<T> CreateTranslation(T transX, T transY);

	/* @brief Scale matrix creator */
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

	/* @brief Calculate the determinant of the 3x3 minor matrix using M[row][col] as the pivot.
	 *		  To calculate the actual determinant of a Matrix4 instance, can refer to following formular:
	 *		  det = M[0][0]*Det(0,0) - M[0][1]*Det(0,1) + M[0][2]*Det(0,2) - M[0][3]*Det(0,3), using first
	 *		  row elements as pivots */
	T Det(int row, int col) const;

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result. 
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void);
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance. User
	 *		  needs to sepcify the return type to prevent incorrect result due to inappropriate data
	 *		  types. (e.g. the inverse of a <int> matrix might has <float> result, if return the result
	 *		  in its original type, the result will be incorrect) */
	template <typename U>
	inline Matrix4<U> GetInverse() const;
	///* @brief Return a matrix that is the inverse of this instance but don't modify this instance. */
	//inline Matrix4<T> GetInverse() const;

	/* @brief Transpose this instance */
	inline void Transpose(void);
	/* @brief Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix4<T> GetTranspose(void) const;

	/* @brief Convert this instance to a new matrix with type "U", but don't modify this instance */
	template <typename U>
	inline Matrix4<U> CovertToType() const;

	///* Multiply vector by matrix */
	///* Return v * M */
	//Vector4<T> MultiplyLeft(const Vector4<T>& vec) const;
	///* Return M * v */
	//Vector4<T> MultiplyRight(const Vector4<T>& vec) const;

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


	/* @brief Return a matrix that is the inverse of a transformation matrix. It "undoes" the original 
	 *		  transformation by taking the transpose of the rotation and applying the inverse translation.
	 *		  (ASSUME this matrix has axis rotation and translation only). */
	inline Matrix4<T> GetInverseRotTrans() const;

	/* @brief Apply this instance as a transform matrix to the input point, return a new vector that
	 *		  represents the transformed point. (The point is transformed clockwise). */
	inline Vector3<T> TransformPoint(const Vector3<T>& point) const;

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


/********************************* Global APIs **************************************/
/* Following global APIs are implemented as function templates. Note that template
 * is not a class or a function, it is a "pattern" that the compiler uses to generate
 * a family of classes or functions. To use function templates, both the template
 * declaration and definition must be visible to the compiler at the point of instantiation.
 * As a result, template functions are typically declared and implemented in the same file.
 */

/* @brief Calculate vec = Mtx * vec; (i.e. column vector) */
template <typename T>
inline Vector3<T> operator* (const Matrix3<T>& mtx, const Vector3<T>& vec)
{
	Vector3<T> res = Vector3<T>();

	/* Iterator all rows in matrix */
	for (int row = 0; row < 3; row++)
	{
		T sum = 0;
		/* Iterator that repeat 3 times */
		for (int i = 0; i < 3; i++)
		{
			sum += mtx[row][i] * vec[i];
		}
		res[row] = sum;
	}
	return res;
}


/* @brief Calculate vec = vec * Mtx; (i.e. row vector) */
template <typename T>
inline Vector3<T> operator* (const Vector3<T>& vec, const Matrix3<T>& mtx)
{
	Vector3<T> res = Vector3<T>();

	/* Iterate all columns in matrix */
	for (int col = 0; col < 3; col++)
	{
		T sum = 0;
		/* Iterator that repeat 3 times */
		for (int i = 0; i < 3; i++)
		{
			sum += vec[i] * mtx[i][col];
		}
		res[col] = sum;
	}
	return res;
}


/* @brief Calculate vec = Mtx * vec; (i.e. column vector) */
template <typename T>
inline Vector4<T> operator* (const Matrix4<T>& mtx, const Vector4<T>& vec)
{
	Vector4<T> res = Vector4<T>();

	/* Iterator all rows in matrix */
	for (int row = 0; row < 4; row++)
	{
		T sum = 0;
		/* Iterator that repeat 4 times */
		for (int i = 0; i < 4; i++)
		{
			sum += mtx[row][i] * vec[i];
		}
		res[row] = sum;
	}
	return res;
}


/* @brief Calculate vec = vec * Mtx; (i.e. row vector) */
template <typename T>
inline Vector4<T> operator* (const Vector4<T>& vec, const Matrix4<T>& mtx)
{
	Vector4<T> res = Vector4<T>();

	/* Iterate all columns in matrix */
	for (int col = 0; col < 4; col++)
	{
		T sum = 0;
		/* Iterator that repeat 4 times */
		for (int i = 0; i < 4; i++)
		{
			sum += vec[i] * mtx[i][col];
		}
		res[col] = sum;
	}
	return res;
}



namespace Matrix
{
/********************************* Unit tests **************************************/

#if defined(_DEBUG)

#include <cassert>

inline void Matrix3UnitTest()
{
	Matrix3<float> case0 = Matrix3<float>(
		1.1f, 0.0f, 1.3f,
		0.0f, 2.2f, 0.0f,
		3.1f, 0.0f, 3.3f);
	Matrix3<int> case1 = Matrix3<int>(
		1, 0, 0,
		0, 2, 0,
		0, 0, 3);
	Vector3<float> case2 = Vector3<float>(1.0f, 2.0f, 3.0f);

	Matrix3<float> temp0;
	Matrix3<int> temp1;
	Vector3<float> temp2;

	assert(AreEqual(case0.Det(0, 1), 0.0f));

	temp0 = case0;
	temp0.Invert();
	assert(temp0 == Matrix3<float>(-8.25f, 0, 3.25f, 0, .45454545f, 0, 7.75f, 0, -2.75f));
	temp0 = case0.GetInverse<float>();
	assert(temp0 == Matrix3<float>(-8.25f, 0, 3.25f, 0, .45454545f, 0, 7.75f, 0, -2.75f));
	temp1 = case1;
	temp1.Invert();
	assert(temp1 == Matrix3<int>(1, 0, 0, 0, 0, 0, 0, 0, 0));
	temp0 = case1.GetInverse<float>();
	assert(temp0 == Matrix3<float>(1.0f, 0, 0, 0, .5f, 0, 0, 0, .3333333f));

	temp0 = case0;
	temp0.Transpose();
	assert(temp0 == Matrix3<float>(1.1f, 0.0f, 3.1f, 0.0f, 2.2f, 0.0f, 1.3f, 0.0f, 3.3f));
	temp0 = case0.GetTranspose();
	assert(temp0 == Matrix3<float>(1.1f, 0.0f, 3.1f, 0.0f, 2.2f, 0.0f, 1.3f, 0.0f, 3.3f));

	temp0 = case0 + Matrix3<float>(1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp0 == Matrix3<float>(2.1f, 1, 2.3f, 1, 3.2f, 1, 4.1f, 1, 4.3f));
	temp0 = temp0 - Matrix3<float>(1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp0 == Matrix3<float>(1.1f, 0.0f, 1.3f, 0.0f, 2.2f, 0.0f, 3.1f, 0.0f, 3.3f));
	temp0 = temp0 * 2;
	assert(temp0 == Matrix3<float>(2.2f, 0.0f, 2.6f, 0.0f, 4.4f, 0.0f, 6.2f, 0.0f, 6.6f));
	temp0 = temp0 / 2;
	assert(temp0 == Matrix3<float>(1.1f, 0.0f, 1.3f, 0.0f, 2.2f, 0.0f, 3.1f, 0.0f, 3.3f));
	temp0 = case0 * case0;
	assert(temp0 == Matrix3<float>(5.24f, 0.0f, 5.72f, 0.0f, 4.84f, 0.0f, 13.64f, 0.0f, 14.92f));

	temp2 = case0 * case2;
	assert(temp2 == Vector3<float>(5.0f, 4.4f, 13.0f));
	temp2 = case2 * case0;
	assert(temp2 == Vector3<float>(10.4f, 4.4f, 11.2f));

}

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
	temp2 = Vector4<double>(1, 2, 3, 4) * sample;
	assert(temp2 == Vector4<double>(90, 100, 110, 120));
	temp2 = sample * Vector4<double>(1, 2, 3, 4);
	assert(temp2 == Vector4<double>(30, 70, 110, 150));
	temp1 = sample.GetTranspose();
	assert(temp1 == Matrix4<double>(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16));
	sample = Matrix4<double>(1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, 1);
	temp1 = sample.GetInverse<double>();
	assert(temp1 == Matrix4<double>(0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, 0.25));


}

#endif



}//Namespace Matrix
}//Namespace Engine