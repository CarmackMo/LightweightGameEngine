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
	T Det(int row, int col) const
	{
		/* Calcualte the determinant based on the following formula:
		 * det = a11(a22a33 - a23a32) - a12(a21a33 - a23a31) + a13(a21a32 - a22a31).
		 * Note that needs to ensure positive-negative correctness*/
		T res = static_cast<T>(pow(-1, row + col)) * (val[(row + 1) % 3][(col + 1) % 3] * val[(row + 2) % 3][(col + 2) % 3] -
													  val[(row + 1) % 3][(col + 2) % 3] * val[(row + 2) % 3][(col + 1) % 3]);

		return res;
	}

	/* @brief Invert this instance. Invert a matrix with type "int" might have incorrect result.
	 *		  Because the real result of the inverse might be a float point number. */
	void Invert(void)
	{
		/* Inverse of matrix M equals to Adj(M)/Det(M).
		 * Calcualte the determinant of this instance, using first row */
		T det = val[0][0] * Det(0, 0) - val[0][1] * Det(0, 1) + val[0][2] * Det(0, 2);

		/* Calculate the adjugate matrix of this instance, the value at (i,j)
		 * equals to Det(i,j) */
		Matrix3<T> adjugate;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				adjugate[i][j] = static_cast<T>(pow(-1, i + j)) * Det(i, j);
			}
		}

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
	Vector3<T> MultiplyLeft(const Vector3<T>& vec) const
	{
		Vector3<T> res = Vector3<T>();

		/* Iterate all columns in matrix */
		for (int col = 0; col < 3; col++)
		{
			T sum = 0;
			/* Iterator that repeat 4 times */
			for (int i = 0; i < 3; i++)
			{
				sum += vec[i] * val[i][col];
			}
			res[col] = sum;
		}
		return res;
	}
	/* Return M * v */
	Vector3<T> MultiplyRight(const Vector3<T>& vec) const
	{
		Vector3<T> res = Vector3<T>();

		/* Iterator all rows in matrix */
		for (int row = 0; row < 3; row++)
		{
			T sum = 0;
			/* Iterator that repeat 4 times */
			for (int i = 0; i < 3; i++)
			{
				sum += val[row][i] * vec[i];
			}
			res[row] = sum;
		}
		return res;
	}

	inline T* operator[] (int row);
	inline const T* operator[] (int row) const;
	inline Matrix3<T> operator+ (const Matrix3<T>& other) const;
	inline Matrix3<T> operator- (const Matrix3<T>& other) const;
	inline Matrix3<T> operator* (T num) const;
	inline Matrix3<T> operator/ (T num) const;
	inline Matrix3<T>& operator= (const Matrix3<T>& other);
	/* Multiply matrix by matrix, return this instance * other */
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

	bool operator== (const Matrix3<T>& other) const
	{
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				if (val[row][col] != other[row][col])
					return false;
			}
		}
		return true;
	}


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
		 * det = a11(a22a33 - a23a32) - a12(a21a33 - a23a31) + a13(a21a32 - a22a31) */
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

		/* Calculate the adjugate matrix of this instance, the value at (i,j)
		 * equals to Det(i,j) */
		T adjugate[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				adjugate[i][j] = static_cast<T>(pow(-1, i + j)) * Det(i, j);
			}
		}

		/* Update the elements of this instance to make it inversed */
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				val[i][j] = adjugate[i][j] / det;
			}
		}
	}
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
	Vector4<T> MultiplyLeft(const Vector4<T>& vec) const
	{
		Vector4<T> res = Vector4<T>();

		/* Iterate all columns in matrix */
		for (int col = 0; col < 4; col++)
		{
			T sum = 0;
			/* Iterator that repeat 4 times */
			for (int i = 0; i < 4; i++)
			{
				sum += vec[i] * val[i][col];
			}
			res[col] = sum;
		}
		return res;
	}
	/* Return M * v */
	Vector4<T> MultiplyRight(const Vector4<T>& vec) const
	{
		Vector4<T> res = Vector4<T>();

		/* Iterator all rows in matrix */
		for (int row = 0; row < 4; row++)
		{
			T sum = 0;
			/* Iterator that repeat 4 times */
			for (int i = 0; i < 4; i++)
			{
				sum += val[row][i] * vec[i];
			}
			res[row] = sum;
		}
		return res;
	}

	inline T* operator[] (int row);
	inline const T* operator[] (int row) const;
	inline Matrix4<T> operator+ (const Matrix4<T>& other) const;
	inline Matrix4<T> operator- (const Matrix4<T>& other) const;
	inline Matrix4<T> operator* (T num) const;
	inline Matrix4<T> operator/ (T num) const;
	inline Matrix4<T>& operator= (const Matrix4<T>& other);
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

	bool operator== (const Matrix4<T>& other) const
	{
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				if (val[row][col] != other[row][col])
					return false;
			}
		}
		return true;
	}


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