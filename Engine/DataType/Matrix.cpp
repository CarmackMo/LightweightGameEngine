#include "Matrix.h"

using namespace Engine;


#pragma region Matrix4

void Matrix4::Invert(void)
{
	/* Inverse of matrix M equals to Adj(M)/Det(M).
	 * Calcualte the determinant of this instance, using first row */
	double det = val[0][0] * Det(0, 0) - val[0][1] * Det(0, 1) +
		val[0][2] * Det(0, 2) - val[0][3] * Det(0, 3);

	/* Calculate the adjugate matrix of this instance, the value at (i,j)
	 * equals to Det(i,j) */
	double adjugate[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			adjugate[i][j] = pow(-1, i + j) * Det(i, j);
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


double Matrix4::Det(int row, int col) const
{
	int x = 0, y = 0;
	double subMatrix[3][3];

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
	double res = 0;
	for (int i = 0; i < 3; i++)
	{
		res += subMatrix[0][i] * (subMatrix[1][(i + 1) % 3] * subMatrix[2][(i + 2) % 3] - subMatrix[1][(i + 2) % 3] * subMatrix[2][(i + 1) % 3]);
	}

	return res;
}


Vector4 Matrix4::MultiplyLeft(const Vector4& vec) const
{
	Vector4 res = Vector4();

	/* Iterate all columns in matrix */
	for (int col = 0; col < 4; col++)
	{
		double sum = 0;
		/* Iterator that repeat 4 times */
		for (int i = 0; i < 4; i++)
		{
			sum += vec.At(i) * val[i][col];
		}
		res.Set(col, sum);
	}
	return res;
}


Vector4 Matrix4::MultiplyRight(const Vector4& vec) const
{
	Vector4 res = Vector4();

	/* Iterator all rows in matrix */
	for (int row = 0; row < 4; row++)
	{
		double sum = 0;
		/* Iterator that repeat 4 times */
		for (int i = 0; i < 4; i++)
		{
			sum += val[row][i] * vec.At(i);
		}
		res.Set(row, sum);
	}
	return res;
}


Matrix4 Matrix4::operator* (const Matrix4& other) const
{
	Matrix4 res = Matrix4();

	/* Iterate all rows in matrix A */
	for (int rowA = 0; rowA < 4; rowA++)
	{
		/* Iterate all columns in matrix B */
		for (int colB = 0; colB < 4; colB++)
		{
			double sum = 0;
			/* Iterator that repeat 4 times */
			for (int i = 0; i < 4; i++)
			{
				sum += val[rowA][i] * other.val[i][colB];
			}
			res.Set(rowA, colB, sum);
		}
	}
	return res;
}


bool Matrix4::operator== (const Matrix4& other) const
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (val[row][col] != other.At(row, col))
				return false;
		}
	}
	return true;
}






#pragma endregion