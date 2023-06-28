#pragma once
#include <type_traits>
#include <utility>
#include "Vector.h"
#include "Mathf.h"


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
	inline Matrix3()
	{
		val[0][0] = static_cast<T>(0); val[0][1] = static_cast<T>(0); val[0][2] = static_cast<T>(0);
		val[1][0] = static_cast<T>(0); val[1][1] = static_cast<T>(0); val[1][2] = static_cast<T>(0);
		val[2][0] = static_cast<T>(0); val[2][1] = static_cast<T>(0); val[2][2] = static_cast<T>(0);
	}
	inline Matrix3(
		T x00, T x01, T x02,
		T x10, T x11, T x12,
		T x20, T x21, T x22)
	{
		val[0][0] = x00; val[0][1] = x01; val[0][2] = x02;
		val[1][0] = x10; val[1][1] = x11; val[1][2] = x12;
		val[2][0] = x20; val[2][1] = x21; val[2][2] = x22;
	}
	inline Matrix3(const Matrix3<T>& other)
	{
		val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2];
		val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2];
		val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2];
	}

	/* Get the specific row / column of the matrix */
	inline Vector3<T> GetRow(int row) const
	{
		return Vector3<T>(val[row][0], val[row][1], val[row][2]);
	}
	inline Vector3<T> GetCol(int col) const
	{
		return Vector3<T>(val[0][col], val[1][col], val[2][col]);
	}

	/* @brief Calculate the determinant of the 2x2 minor matrix using M[row][col] as the pivot.
	 *		  To calculate the actual determinant of a Matrix3 instance, can refer to following formular: 
	 *		  det = M[0][0]*Det(0,0) - M[0][1]*Det(0,1) + M[0][2]*Det(0,2), using first row elements
	 *		  as pivots */
	T Det(int row, int col) const
	{
		T res = static_cast<T>(pow(-1, row + col)) *
			(val[(row + 1) % 3][(col + 1) % 3] * val[(row + 2) % 3][(col + 2) % 3] -
				val[(row + 1) % 3][(col + 2) % 3] * val[(row + 2) % 3][(col + 1) % 3]);

		return res;
	}

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result.
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void)
	{
		/* Inverse of matrix M equals to Adj(M)/Det(M).
		 * Calcualte the determinant of this instance, using first row elements as pivots */
		T det = val[0][0] * Det(0, 0) - val[0][1] * Det(0, 1) + val[0][2] * Det(0, 2);

		/* Calculate the cofactors matrix of this instance first, the value at (i,j)
		 * equals to Det(i,j) */
		Matrix3<T> adjugate;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				adjugate[i][j] = static_cast<T>(pow(-1, i + j)) * Det(i, j);
			}
		}

		/* Calculate the adjugate matrix by transposing the cofactors matrix */
		adjugate.Transpose();

		/* Update the elements of this instance to make it inversed */
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				val[i][j] = adjugate[i][j] / det;
			}
		}
	}
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance. User 
	 *		  needs to sepcify the return type to prevent incorrect result due to inappropriate data 
	 *		  types. (e.g. the inverse of a <int> matrix might has <float> result, if return the result 
	 *		  in its original type, the result will be incorrect) */
	template <typename U>
	inline Matrix3<U> GetInverse() const
	{
		Matrix3<U> res = this->ConvertTo<U>();
		res.Invert();
		return res;
	}


	/* @brief Transpose this instance */
	inline void Transpose(void)
	{
		std::swap(val[0][1], val[1][0]);
		std::swap(val[0][2], val[2][0]);
		std::swap(val[1][2], val[2][1]);

		//T t01 = val[0][1], t02 = val[0][2], t12 = val[1][2];
		//val[0][1] = val[1][0]; val[0][2] = val[2][0]; val[1][2] = val[2][1];
		//val[1][0] = t01; val[2][0] = t02; val[2][1] = t12;
	}
	/* @brief Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix3<T> GetTranspose(void) const
	{
		return Matrix3<T>(
			val[0][0], val[1][0], val[2][0],
			val[0][1], val[1][1], val[2][1],
			val[0][2], val[1][2], val[2][2]);
	}

	/* @brief Convert this instance to a new matrix with type "U", but don't modify this instance */
	template <typename U>
	inline Matrix3<U> ConvertTo() const
	{
		return Matrix3<U>(
			static_cast<U>(val[0][0]), static_cast<U>(val[0][1]), static_cast<U>(val[0][2]),
			static_cast<U>(val[1][0]), static_cast<U>(val[1][1]), static_cast<U>(val[1][2]),
			static_cast<U>(val[2][0]), static_cast<U>(val[2][1]), static_cast<U>(val[2][2]));
	}


	/* Indexing */
	inline T* operator[] (int row)
	{
		return val[row];
	}
	inline const T* operator[] (int row) const
	{
		return val[row];
	}

	inline Matrix3<T> operator+ (const Matrix3<T>& other) const
	{
		return Matrix3<T>(
			val[0][0] + other[0][0], val[0][1] + other[0][1], val[0][2] + other[0][2],
			val[1][0] + other[1][0], val[1][1] + other[1][1], val[1][2] + other[1][2],
			val[2][0] + other[2][0], val[2][1] + other[2][1], val[2][2] + other[2][2]);
	}
	inline Matrix3<T> operator- (const Matrix3<T>& other) const
	{
		return Matrix3<T>(
			val[0][0] - other[0][0], val[0][1] - other[0][1], val[0][2] - other[0][2],
			val[1][0] - other[1][0], val[1][1] - other[1][1], val[1][2] - other[1][2],
			val[2][0] - other[2][0], val[2][1] - other[2][1], val[2][2] - other[2][2]);
	}
	inline Matrix3<T> operator* (T num) const
	{
		return Matrix3<T>(
			val[0][0] * num, val[0][1] * num, val[0][2] * num,
			val[1][0] * num, val[1][1] * num, val[1][2] * num,
			val[2][0] * num, val[2][1] * num, val[2][2] * num);
	}
	inline Matrix3<T> operator/ (T num) const
	{
		return Matrix3<T>(
			val[0][0] / num, val[0][1] / num, val[0][2] / num,
			val[1][0] / num, val[1][1] / num, val[1][2] / num,
			val[2][0] / num, val[2][1] / num, val[2][2] / num);
	}

	/* @brief Multiply matrix by matrix */
	Matrix3<T> operator* (const Matrix3<T>& other) const
	{
		Matrix3<T> res = Matrix3<T>();

		/* Iterate all rows in matrix A */
		for (int rowA = 0; rowA < 3; rowA++)
		{
			/* Iterate all columns in matrix B */
			for (int colB = 0; colB < 3; colB++)
			{
				T sum = 0;
				/* Iterator that repeat 4 times */
				for (int i = 0; i < 3; i++)
				{
					sum += val[rowA][i] * other[i][colB];
				}
				res[rowA][colB] = sum;
			}
		}
		return res;
	}

	inline Matrix3<T>& operator= (const Matrix3<T>& other)
	{
		val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2];
		val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2];
		val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2];
		return *this;
	}

	bool operator== (const Matrix3<T>& other) const
	{
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				if (AreEqual(static_cast<float>(val[row][col]), static_cast<float>(other[row][col])) == false)
					return false;
			}
		}
		return true;
	}


	/* @brief Convert this instance into a 4x4 transformation matrix. The additional row and column 
	 *		  represents translations and homogeneous coordinates respectively. Mathematically,
	 *		  this convertion is known as an affine transformation, allowing the original matrix
	 *		  to be used for 3D math in a homogenous coordinate system. */
	inline Matrix4<T> ToTransform() const
	{
		return Matrix4<T>(
			val[0][0], val[0][1], val[0][2], 0.0f,
			val[1][0], val[1][1], val[1][2], 0.0f,
			val[2][0], val[2][1], val[2][2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	/* @brief Apply this instance as a transform matrix to the input point, return a new vector that
	*		  represents the transformed point. (The point is transformed clockwise). */
	inline Vector2<T> TransformPoint(const Vector2<T>& point) const
	{
		/* Convert the point's coordinates to homogeneous coordinate */
		Vector3<T> Point = *this * Vector3<T>(point[0], point[1], 1.0f);

		/* The 3rd element of the resulting transformed point is the homogeneous coordinate.
		 * The variable "invHomo" stores the inverse of the homogeneous coordinate. The
		 * purpose of dividing the transformed point by "invHomo" is to normalize the coordinates
		 * This normalization step ensures that the transformed point retains its relative
		 * position in 3D space. It effectively brings the point back from homogeneous coordinates
		 * to Cartesian coordinates. */
		float invHomo = 1.0f / Point[2];
		return Vector2<T>(Point[0] * invHomo, Point[1] * invHomo);
	}

	/* @brief Identity matrix creator */
	inline static Matrix3<T> CreateIdentity(void)
	{
		Matrix3<double> res = Matrix3<double>(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1);
		return res.ConvertTo<T>();
	}

	/* @brief Rotate matrix around origin point, rotate counter-clockwise */
	inline static Matrix3<T> CreateXRotation(double rad)
	{
		double sin_ = sin(rad);
		double cos_ = cos(rad);

		Matrix3<double> res = Matrix3<double>(
			1, 0, 0,
			0, cos_, -sin_,
			0, sin_, cos_);
		return res.ConvertTo<T>();
	}
	inline static Matrix3<T> CreateYRotation(double rad)
	{
		double sin_ = sin(rad);
		double cos_ = cos(rad);

		Matrix3<double> res = Matrix3<double>(
			cos_, 0, sin_,
			0, 1, 0,
			-sin_, 0, cos_);
		return res.ConvertTo<T>();
	}
	inline static Matrix3<T> CreateZRotation(double rad)
	{
		double sin_ = sin(rad);
		double cos_ = cos(rad);

		Matrix3<double> res = Matrix3<double>(
			cos_, -sin_, 0,
			sin_, cos_, 0,
			0, 0, 1);
		return res.ConvertTo<T>();
	}

	/* @brief Translation matrix creator */
	inline static Matrix3<T> CreateTranslation(const Vector2<T>& vec)
	{
		Matrix3<double> res = Matrix3<double>(
			1, 0, vec[0],
			0, 1, vec[1],
			0, 0, 1);
		return res.ConvertTo<T>();
	}
	inline static Matrix3<T> CreateTranslation(T transX, T transY)
	{
		Matrix3<double> res = Matrix3<double>(
			1, 0, transX,
			0, 1, transY,
			0, 0, 1);
		return res.ConvertTo<T>();
	}

	/* @brief Scale matrix creator */
	inline static Matrix3<T> CreateScale(const Vector2<T>& vec)
	{
		Matrix3<double> res = Matrix3<double>(
			vec[0], 0, 0,
			0, vec[1], 0,
			0, 0, 1);
		return res.ConvertTo<T>();
	}
	inline static Matrix3<T> CreateScale(T scaleX, T scaleY)
	{
		Matrix3<double> res = Matrix3<double>(
			scaleX, 0, 0,
			0, scaleY, 0,
			0, 0, 1);
		return res.ConvertTo<T>();
	}
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
	inline Matrix4()
	{
		val[0][0] = static_cast<T>(0); val[0][1] = static_cast<T>(0); val[0][2] = static_cast<T>(0); val[0][3] = static_cast<T>(0);
		val[1][0] = static_cast<T>(0); val[1][1] = static_cast<T>(0); val[1][2] = static_cast<T>(0); val[1][3] = static_cast<T>(0);
		val[2][0] = static_cast<T>(0); val[2][1] = static_cast<T>(0); val[2][2] = static_cast<T>(0); val[2][3] = static_cast<T>(0);
		val[3][0] = static_cast<T>(0); val[3][1] = static_cast<T>(0); val[3][2] = static_cast<T>(0); val[3][3] = static_cast<T>(0);
	}
	inline Matrix4(
		T x00, T x01, T x02, T x03,
		T x10, T x11, T x12, T x13,
		T x20, T x21, T x22, T x23,
		T x30, T x31, T x32, T x33)
	{
		val[0][0] = x00; val[0][1] = x01; val[0][2] = x02; val[0][3] = x03;
		val[1][0] = x10; val[1][1] = x11; val[1][2] = x12; val[1][3] = x13;
		val[2][0] = x20; val[2][1] = x21; val[2][2] = x22; val[2][3] = x23;
		val[3][0] = x30; val[3][1] = x31; val[3][2] = x32; val[3][3] = x33;
	}
	inline Matrix4(const Matrix4<T>& other)
	{
		val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2]; val[0][3] = other[0][3];
		val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2]; val[1][3] = other[1][3];
		val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2]; val[2][3] = other[2][3];
		val[3][0] = other[3][0]; val[3][1] = other[3][1]; val[3][2] = other[3][2]; val[3][3] = other[3][3];
	}

	/* @brief Calculate the determinant of the 3x3 minor matrix using M[row][col] as the pivot.
	 *		  To calculate the actual determinant of a Matrix4 instance, can refer to following formular:
	 *		  det = M[0][0]*Det(0,0) - M[0][1]*Det(0,1) + M[0][2]*Det(0,2) - M[0][3]*Det(0,3), using first
	 *		  row elements as pivots */
	T Det(int row, int col) const
	{
		int x = 0, y = 0;
		T subMatrix[3][3];

		/* Extract the 3x3 sub-matrix that is going to calculate the determinant */
		for (int i = 0; i < 4; i++)
		{
			if (i == row) { continue; }
			for (int j = 0; j < 4; j++)
			{
				if (j == col) { continue; }

				subMatrix[x][y] = val[i][j];
				y++;
			}
			y = 0;
			x++;
		}

		/* Calcualte the determinant based on the following formula:
		 * det = M11(M22M33 - M23M32) - M12(M21M33 - M23M31) + M13(M21M32 - M22M31) */
		T res = 0;
		for (int i = 0; i < 3; i++)
		{
			res += subMatrix[0][i] * (subMatrix[1][(i + 1) % 3] * subMatrix[2][(i + 2) % 3] - subMatrix[1][(i + 2) % 3] * subMatrix[2][(i + 1) % 3]);
		}

		return res;
	}

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result. 
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void)
	{
		/* Inverse of matrix M equals to Adj(M)/Det(M).
		 * Calcualte the determinant of this instance, using first row */
		T det = val[0][0] * Det(0, 0) - val[0][1] * Det(0, 1) +
			val[0][2] * Det(0, 2) - val[0][3] * Det(0, 3);

		/* Calculate the cofactors matrix of this instance first, the value at (i,j)
		 * equals to Det(i,j) */
		Matrix4<T> adjugate;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				adjugate[i][j] = static_cast<T>(pow(-1, i + j)) * Det(i, j);
			}
		}

		/* Calculate the adjugate matrix by transposing the cofactors matrix */
		adjugate.Transpose();

		/* Update the elements of this instance to make it inversed */
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				val[i][j] = adjugate[i][j] / det;
			}
		}
	}
	/* @brief Return a matrix that is the inverse of this instance but don't modify this instance. User
	 *		  needs to sepcify the return type to prevent incorrect result due to inappropriate data
	 *		  types. (e.g. the inverse of a <int> matrix might has <float> result, if return the result
	 *		  in its original type, the result will be incorrect) */
	template <typename U>
	inline Matrix4<U> GetInverse() const
	{
		Matrix4<U> res = this->ConvertTo<U>();
		res.Invert();
		return res;
	}

	/* @brief Transpose this instance */
	inline void Transpose(void)
	{
		std::swap(val[0][1], val[1][0]); std::swap(val[0][2], val[2][0]);
		std::swap(val[0][3], val[3][0]); std::swap(val[1][2], val[2][1]);
		std::swap(val[1][3], val[3][1]); std::swap(val[2][3], val[3][2]);

		//T t01 = val[0][1], t02 = val[0][2], t03 = val[0][3],
		//  t12 = val[1][2], t13 = val[1][3], t23 = val[2][3];
		//val[0][1] = val[1][0]; val[0][2] = val[2][0]; val[0][3] = val[3][0];
		//val[1][2] = val[2][1]; val[1][3] = val[3][1]; val[2][3] = val[3][2];
		//val[1][0] = t01, val[2][0] = t02, val[3][0] = t03,
		//val[2][1] = t12, val[3][1] = t13, val[3][2] = t23;
	}
	/* @brief Return a matrix that is the transpose of this instance but don't modify this instance */
	inline Matrix4<T> GetTranspose(void) const
	{
		return Matrix4<T>(
			val[0][0], val[1][0], val[2][0], val[3][0],
			val[0][1], val[1][1], val[2][1], val[3][1],
			val[0][2], val[1][2], val[2][2], val[3][2],
			val[0][3], val[1][3], val[2][3], val[3][3]);
	}

	/* @brief Convert this instance to a new matrix with type "U", but don't modify this instance */
	template <typename U>
	inline Matrix4<U> ConvertTo() const
	{
		return Matrix4<U>(
			static_cast<U>(val[0][0]), static_cast<U>(val[0][1]), static_cast<U>(val[0][2]), static_cast<U>(val[0][3]),
			static_cast<U>(val[1][0]), static_cast<U>(val[1][1]), static_cast<U>(val[1][2]), static_cast<U>(val[1][3]),
			static_cast<U>(val[2][0]), static_cast<U>(val[2][1]), static_cast<U>(val[2][2]), static_cast<U>(val[2][3]),
			static_cast<U>(val[3][0]), static_cast<U>(val[3][1]), static_cast<U>(val[3][2]), static_cast<U>(val[3][3]));
	}

	/* Indexing */
	inline T* operator[] (int row)
	{
		return val[row];
	}
	inline const T* operator[] (int row) const
	{
		return val[row];
	}

	inline Matrix4<T> operator+ (const Matrix4<T>& other) const
	{
		return Matrix4<T>(
			val[0][0] + other[0][0], val[0][1] + other[0][1], val[0][2] + other[0][2], val[0][3] + other[0][3],
			val[1][0] + other[1][0], val[1][1] + other[1][1], val[1][2] + other[1][2], val[1][3] + other[1][3],
			val[2][0] + other[2][0], val[2][1] + other[2][1], val[2][2] + other[2][2], val[2][3] + other[2][3],
			val[3][0] + other[3][0], val[3][1] + other[3][1], val[3][2] + other[3][2], val[3][3] + other[3][3]);
	}
	inline Matrix4<T> operator- (const Matrix4<T>& other) const
	{
		return Matrix4<T>(
			val[0][0] - other[0][0], val[0][1] - other[0][1], val[0][2] - other[0][2], val[0][3] - other[0][3],
			val[1][0] - other[1][0], val[1][1] - other[1][1], val[1][2] - other[1][2], val[1][3] - other[1][3],
			val[2][0] - other[2][0], val[2][1] - other[2][1], val[2][2] - other[2][2], val[2][3] - other[2][3],
			val[3][0] - other[3][0], val[3][1] - other[3][1], val[3][2] - other[3][2], val[3][3] - other[3][3]);
	}
	inline Matrix4<T> operator* (T num) const
	{
		return Matrix4<T>(
			val[0][0] * num, val[0][1] * num, val[0][2] * num, val[0][3] * num,
			val[1][0] * num, val[1][1] * num, val[1][2] * num, val[1][3] * num,
			val[2][0] * num, val[2][1] * num, val[2][2] * num, val[2][3] * num,
			val[3][0] * num, val[3][1] * num, val[3][2] * num, val[3][3] * num);
	}
	inline Matrix4<T> operator/ (T num) const
	{
		return Matrix4<T>(
			val[0][0] / num, val[0][1] / num, val[0][2] / num, val[0][3] / num,
			val[1][0] / num, val[1][1] / num, val[1][2] / num, val[1][3] / num,
			val[2][0] / num, val[2][1] / num, val[2][2] / num, val[2][3] / num,
			val[3][0] / num, val[3][1] / num, val[3][2] / num, val[3][3] / num);
	}

	/* Multiply matrix by matrix, return this instance * other */
	Matrix4<T> operator* (const Matrix4<T>& other) const
	{
		Matrix4<T> res = Matrix4<T>();

		/* Iterate all rows in matrix A */
		for (int rowA = 0; rowA < 4; rowA++)
		{
			/* Iterate all columns in matrix B */
			for (int colB = 0; colB < 4; colB++)
			{
				T sum = 0;
				/* Iterator that repeat 4 times */
				for (int i = 0; i < 4; i++)
				{
					sum += val[rowA][i] * other[i][colB];
				}
				res[rowA][colB] = sum;
			}
		}
		return res;
	}

	inline Matrix4<T>& operator= (const Matrix4<T>& other)
	{
		val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2]; val[0][3] = other[0][3];
		val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2]; val[1][3] = other[1][3];
		val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2]; val[2][3] = other[2][3];
		val[3][0] = other[3][0]; val[3][1] = other[3][1]; val[3][2] = other[3][2]; val[3][3] = other[3][3];
		return *this;
	}

	bool operator== (const Matrix4<T>& other) const
	{
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				if (AreEqual(static_cast<float>(val[row][col]), static_cast<float>(other[row][col])) == false)
					return false;
			}
		}
		return true;
	}


	/* @brief Return a matrix that is the inverse of a transformation matrix. It "undoes" the original 
	 *		  transformation by taking the transpose of the rotation and applying the inverse translation.
	 *		  (ASSUME this matrix has axis rotation and translation only). */
	inline Matrix4<T> GetInverseRotTrans() const
	{
		/* Constructs a new Matrix4 instance where the top-left 3x3 portion contains the transpose of the original matrix's
		 * rotation component. The elements in the 4th column are the negated dot products of each column of the rotation
		 * component with the translation component. The elements in the bottom row are set to 0, except for the last element,
		 * which is set to 1 to maintain the homogeneous coordinate
		 * By obtaining the inverse of a transformation matrix, users can use it to transform points in the opposite direction,
		 * enabling operations such as un-rotating or un-translating coordinates.*/
		return Matrix4<T>(
			val[0][0], val[1][0], val[2][0], -((val[0][0] * val[0][3]) + (val[1][0] * val[1][3]) + (val[2][0] * val[2][3])),
			val[0][1], val[1][1], val[2][1], -((val[0][1] * val[0][3]) + (val[1][1] * val[1][3]) + (val[2][1] * val[2][3])),
			val[0][2], val[1][2], val[2][2], -((val[0][2] * val[0][3]) + (val[1][2] * val[1][3]) + (val[2][2] * val[2][3])),
			0.0f, 0.0f, 0.0f, 1.0f);
	}

	/* @brief Apply this instance as a transform matrix to the input point, return a new vector that
	 *		  represents the transformed point. (The point is transformed clockwise). */
	inline Vector3<T> TransformPoint(const Vector3<T>& point) const
	{
		/* Convert the point's coordinates to homogeneous coordinate */
		Vector4<T> Point = *this * Vector4<T>(point[0], point[1], point[2], 1.0f);

		/* The 4th element of the resulting transformed point is the homogeneous coordinate.
		 * The variable "invHomo" stores the inverse of the homogeneous coordinate. The
		 * purpose of dividing the transformed point by "invHomo" is to normalize the coordinates
		 * This normalization step ensures that the transformed point retains its relative
		 * position in 3D space. It effectively brings the point back from homogeneous coordinates
		 * to Cartesian coordinates. */
		float invHomo = 1.0f / Point[3];
		return Vector3<T>(Point[0] * invHomo, Point[1] * invHomo, Point[2] * invHomo);
	}

	/* Identity matrix creator */
	inline static Matrix4<T> CreateIdentity(void)
	{
		Matrix4<double> res = Matrix4<double>(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}

	/* @brief Rotate matrix around X-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateXRotation(double rad)
	{
		double cos_ = cos(rad);
		double sin_ = sin(rad);

		Matrix4<double> res = Matrix4<double>(
			1, 0, 0, 0,
			0, cos_, -sin_, 0,
			0, sin_, cos_, 0,
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}
	/* @brief Rotate matrix around Y-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateYRotation(double rad)
	{
		/* Note that the sin() in the first row is positive and
		 * sin() in the third row is negative. This is because
		 * we are using right-hand coordinate. */
		double cos_ = cos(rad);
		double sin_ = sin(rad);

		Matrix4<double> res = Matrix4<double>(
			cos_, 0, sin_, 0,
			0, 1, 0, 0,
			-sin_, 0, cos_, 0,
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}
	/* @brief Rotate matrix around Z-axis, rotate counter-clockwise */
	inline static Matrix4<T> CreateZRotation(double rad)
	{
		double cos_ = cos(rad);
		double sin_ = sin(rad);

		Matrix4<double> res = Matrix4<double>(
			cos_, -sin_, 0, 0,
			sin_, cos_, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}

	/* Translation matrix creator */
	inline static Matrix4<T> CreateTranslation(const Vector3<T>& vec)
	{
		Matrix4<double> res = Matrix4<double>(
			1, 0, 0, vec[0],
			0, 1, 0, vec[1],
			0, 0, 1, vec[2],
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}
	inline static Matrix4<T> CreateTranslation(T transX, T transY, T transZ)
	{
		Matrix4<double> res = Matrix4<double>(
			1, 0, 0, transX,
			0, 1, 0, transY,
			0, 0, 1, transZ,
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}

	/* Scale matrix creator */
	inline static Matrix4<T> CreateScale(const Vector3<T>& vec)
	{
		Matrix4<double> res = Matrix4<double>(
			vec[0], 0, 0, 0,
			0, vec[1], 0, 0,
			0, 0, vec[2], 0,
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}
	inline static Matrix4<T> CreateScale(T scaleX, T scaleY, T scaleZ)
	{
		Matrix4<double> res = Matrix4<double>(
			scaleX, 0, 0, 0,
			0, scaleY, 0, 0,
			0, 0, scaleZ, 0,
			0, 0, 0, 1);
		return res.ConvertTo<T>();
	}

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
	Matrix4<float> case0 = Matrix4<float>(
		1.1f, 0.0f, 0.0f, 0.0f,
		0.0f, 2.2f, 2.3f, 0.0f,
		0.0f, 3.2f, 3.3f, 0.0f,
		0.0f, 0.0f, 0.0f, 4.4f);
	Matrix4<int> case1 = Matrix4<int>(
		1, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 3, 0,
		0, 0, 0, 4);
	Vector4<float> case2 = Vector4<float>(1.0f, 2.0f, 3.0f, 4.0f);

	Matrix4<float> temp0;
	Matrix4<int> temp1;
	Vector4<float> temp2;


	assert(AreEqual(case0.Det(1, 1), 15.972f));

	temp0 = case0;
	temp0.Invert();
	assert(temp0 == Matrix4<float>(.9090909f, 0, 0, 0, 0, -32.999961f, 22.999971f, 0, 0, 31.999961f, -21.999975f, 0, 0, 0, 0, .22727272f));
	temp0 = case0.GetInverse<float>();
	assert(temp0 == Matrix4<float>(.9090909f, 0, 0, 0, 0, -32.999961f, 22.999971f, 0, 0, 31.999961f, -21.999975f, 0, 0, 0, 0, .22727272f));
	temp1 = case1;
	temp1.Invert();
	assert(temp1 == Matrix4<int>(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
	temp0 = case1.GetInverse<float>();
	assert(temp0 == Matrix4<float>(1, 0, 0, 0, 0, .5f, 0, 0, 0, 0, .33333333f, 0, 0, 0, 0, .25f));

	temp0 = case0;
	temp0.Transpose();
	assert(temp0 == Matrix4<float>(1.1f, 0.0f, 0.0f, 0.0f, 0.0f, 2.2f, 3.2f, 0.0f, 0.0f, 2.3f, 3.3f, 0.0f, 0.0f, 0.0f, 0.0f, 4.4f));
	temp0 = case0.GetTranspose();
	assert(temp0 == Matrix4<float>(1.1f, 0.0f, 0.0f, 0.0f, 0.0f, 2.2f, 3.2f, 0.0f, 0.0f, 2.3f, 3.3f, 0.0f, 0.0f, 0.0f, 0.0f, 4.4f));

	temp0 = case0 + Matrix4<float>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp0 == Matrix4<float>(2.1f, 1, 1, 1, 1, 3.2f, 3.3f, 1, 1, 4.2f, 4.3f, 1, 1, 1, 1, 5.4f));
	temp0 = temp0 - Matrix4<float>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp0 == Matrix4<float>(1.1f, 0.0f, 0.0f, 0.0f, 0.0f, 2.2f, 2.3f, 0.0f, 0.0f, 3.2f, 3.3f, 0.0f, 0.0f, 0.0f, 0.0f, 4.4f));
	temp0 = temp0 * 2;
	assert(temp0 == Matrix4<float>(2.2f, 0.0f, 0.0f, 0.0f, 0.0f, 4.4f, 4.6f, 0.0f, 0.0f, 6.4f, 6.6f, 0.0f, 0.0f, 0.0f, 0.0f, 8.8f));
	temp0 = temp0 / 2;
	assert(temp0 == Matrix4<float>(1.1f, 0.0f, 0.0f, 0.0f, 0.0f, 2.2f, 2.3f, 0.0f, 0.0f, 3.2f, 3.3f, 0.0f, 0.0f, 0.0f, 0.0f, 4.4f));
	temp0 = case0 * case0;
	assert(temp0 == Matrix4<float>(1.21f, 0, 0, 0, 0, 12.2f, 12.65f, 0, 0, 17.6f, 18.25f, 0, 0, 0, 0, 19.36f));

	temp2 = case0 * case2;
	assert(temp2 == Vector4<float>(1.1f, 11.3f, 16.3f, 17.6f));
	temp2 = case2 * case0;
	assert(temp2 == Vector4<float>(1.1f, 14, 14.5f, 17.6f));
}

#endif



}//Namespace Matrix
}//Namespace Engine