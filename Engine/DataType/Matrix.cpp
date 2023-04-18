#include "Matrix.h"

using namespace Engine;


#pragma region Matrix3

template <typename T>
T Matrix3<T>::Det(int row, int col) const
{
	/* Calcualte the determinant based on the following formula:
	 * det = a11(a22a33 - a23a32) - a12(a21a33 - a23a31) + a13(a21a32 - a22a31).
	 * Note that needs to ensure positive-negative correctness*/
	T res = static_cast<T>(pow(-1, row + col)) * (val[(row + 1) % 3][(col + 1) % 3] * val[(row + 2) % 3][(col + 2) % 3] -
		val[(row + 1) % 3][(col + 2) % 3] * val[(row + 2) % 3][(col + 1) % 3]);

	return res;
}


template <typename T>
void Matrix3<T>::Invert(void)
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


template <typename T>
Vector3<T> Matrix3<T>::MultiplyLeft(const Vector3<T>& vec) const
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


template <typename T>
Vector3<T> Matrix3<T>::MultiplyRight(const Vector3<T>& vec) const
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


template <typename T>
Matrix3<T> Matrix3<T>::operator* (const Matrix3<T>& other) const
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


template <typename T>
bool Matrix3<T>::operator== (const Matrix3<T>& other) const
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


#pragma endregion




#pragma region Matrix4

template <typename T>
T Matrix4<T>::Det(int row, int col) const
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


template <typename T>
void Matrix4<T>::Invert(void)
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


template <typename T>
Vector4<T> Matrix4<T>::MultiplyLeft(const Vector4<T>& vec) const
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


template <typename T>
Vector4<T> Matrix4<T>::MultiplyRight(const Vector4<T>& vec) const
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


template <typename T>
Matrix4<T> Matrix4<T>::operator* (const Matrix4<T>& other) const
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


template <typename T>
bool Matrix4<T>::operator== (const Matrix4<T>& other) const
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


template <typename T>
void Matrix4<T>::UnitTest()
{
	Matrix4<T> test = Matrix4<T>(
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
	);

	/* Operator Test */
	Matrix4<T> temp1;
	temp1 = test + Matrix4<T>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp1 == Matrix4<T>(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17));
	temp1 = test - Matrix4<T>(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
	assert(temp1 == Matrix4<T>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
	temp1 = test * 2;
	assert(temp1 == Matrix4<T>(2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32));
	temp1 = test * Matrix4<T>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
	assert(temp1 == Matrix4<T>(90, 100, 110, 120, 202, 228, 254, 280, 314, 356, 398, 440, 426, 484, 542, 600));
	assert(2 == test[0][1]);

	/* Math Operation Test */
	Vector4<T> temp2;
	temp2 = test.MultiplyLeft(Vector4<T>(1, 2, 3, 4));
	assert(temp2 == Vector4<T>(90, 100, 110, 120));
	temp2 = test.MultiplyRight(Vector4<T>(1, 2, 3, 4));
	assert(temp2 == Vector4<T>(30, 70, 110, 150));
	temp1 = test.GetTranspose();
	assert(temp1 == Matrix4<T>(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16));
	test = Matrix4<T>(1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, -1, 1, 1, 1);
	temp1 = test.GetInverse();
	assert(temp1 == Matrix4<T>(0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, -0.25, 0.25, 0.25, 0.25));

}

#pragma endregion