#pragma once
#include "Dependency.h"
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


	/* Calculate the determinant of the 2x2 minor matrix where M(row, col) is the pivot */
	T Det(int row, int col) const;

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result.
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void);
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance.
	 *		  This function can avoid incorrect invert result */
	template <typename U>
	inline Matrix3<U> GetInverse() const;

	/* Transpose this instance */
	inline void Transpose(void);
	/* Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix3<T> GetTranspose(void) const;

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
	inline static Matrix3<T> CreateRotation(double rad);

	/* Translation matrix creator */
	inline static Matrix3<T> CreateTranslation(Vector2<T>& vec);
	inline static Matrix3<T> CreateTranslation(T transX, T transY);

	/* Scale matrix creator */
	inline static Matrix3<T> CreateScale(Vector2<T>& vec);
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
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance.
	 *		  This function can avoid incorrect invert result */
	template <typename U>
	inline Matrix4<U> GetInverse() const;

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
	inline static Matrix4<T> CreateTranslation(Vector3<T>& vec);
	inline static Matrix4<T> CreateTranslation(T transX, T transY, T transZ);

	/* Scale matrix creator */
	inline static Matrix4<T> CreateScale(Vector3<T>& vec);
	inline static Matrix4<T> CreateScale(T scaleX, T scaleY, T scaleZ);

};

#include "Matrix.inl"

}