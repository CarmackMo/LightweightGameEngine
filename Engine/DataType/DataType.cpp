#include "DataType.h"

using namespace Engine;

#pragma region Vector2

const Vector2 Vector2::Zero(0, 0);
const Vector2 Vector2::Left(-1, 0);
const Vector2 Vector2::Right(1, 0);
const Vector2 Vector2::Up(0, 1);
const Vector2 Vector2::Down(0, -1);

#pragma endregion


#pragma region Vector3

const Vector3 Vector3::Zero(0, 0, 0);
const Vector3 Vector3::Left(-1, 0, 0);
const Vector3 Vector3::Right(1, 0, 0);
const Vector3 Vector3::Up(0, 1, 0);
const Vector3 Vector3::Down(0, -1, 0);
const Vector3 Vector3::Front(0, 0, 1);
const Vector3 Vector3::Back(0, 0, -1);

#pragma endregion



#pragma region Vector4

Vector4::Vector4() : w(0), x(0), y(0), z(0)
{ }

Vector4::Vector4(double w, double x, double y, double z) :
	w(w), x(x), y(y), z(z)
{ }

Vector4::Vector4(const Vector4& other) :
	w(other.w), x(other.x), y(other.y), z(other.z)
{ }

double Vector4::At(int idx) const
{
	switch (idx)
	{
	case 0:
		return w;
	case 1:
		return x;
	case 2:
		return y;
	case 3:
		return z;
	default:
		break;
	}
}

void Vector4::Set(int idx, double val)
{
	switch (idx)
	{
	case 0:
		w = val;
		break;
	case 1:
		x = val;
		break;
	case 2:
		y = val;
		break;
	case 3:
		z = val;
		break;
	default:
		break;
	}
}

#pragma endregion




#pragma region Matrix4

Matrix4::Matrix4()
{
	val[0][0] = 0; val[0][1] = 0; val[0][2] = 0; val[0][3] = 0;
	val[1][0] = 0; val[1][1] = 0; val[1][2] = 0; val[1][3] = 0;
	val[2][0] = 0; val[2][1] = 0; val[2][2] = 0; val[2][3] = 0;
	val[3][0] = 0; val[3][1] = 0; val[3][2] = 0; val[3][3] = 0;
}

Matrix4::Matrix4(
	double x00, double x01, double x02, double x03,
	double x10, double x11, double x12, double x13,
	double x20, double x21, double x22, double x23,
	double x30, double x31, double x32, double x33)
{
	val[0][0] = x00; val[0][1] = x01; val[0][2] = x02; val[0][3] = x03;
	val[1][0] = x10; val[1][1] = x11; val[1][2] = x12; val[1][3] = x13;
	val[2][0] = x20; val[2][1] = x21; val[2][2] = x22; val[2][3] = x23;
	val[3][0] = x30; val[3][1] = x31; val[3][2] = x32; val[3][3] = x33;
}

Matrix4::Matrix4(const Matrix4& other)
{ 
	val[0][0] = other.At(0,0); val[0][1] = other.At(0,1); val[0][2] = other.At(0,2); val[0][3] = other.At(0,3);
	val[1][0] = other.At(1,0); val[1][1] = other.At(1,1); val[1][2] = other.At(1,2); val[1][3] = other.At(1,3);
	val[2][0] = other.At(2,0); val[2][1] = other.At(2,1); val[2][2] = other.At(2,2); val[2][3] = other.At(2,3);
	val[3][0] = other.At(3,0); val[3][1] = other.At(3,1); val[3][2] = other.At(3,2); val[3][3] = other.At(3,3);
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
	val[0][0] = other.At(0, 0); val[0][1] = other.At(0, 1); val[0][2] = other.At(0, 2); val[0][3] = other.At(0, 3);
	val[1][0] = other.At(1, 0); val[1][1] = other.At(1, 1); val[1][2] = other.At(1, 2); val[1][3] = other.At(1, 3);
	val[2][0] = other.At(2, 0); val[2][1] = other.At(2, 1); val[2][2] = other.At(2, 2); val[2][3] = other.At(2, 3);
	val[3][0] = other.At(3, 0); val[3][1] = other.At(3, 1); val[3][2] = other.At(3, 2); val[3][3] = other.At(3, 3);
	return *this;
}

Matrix4 Matrix4::CreateIdentity(void)
{
	return Matrix4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateXRotation(double rad)
{
	return Matrix4(
		1, 0,		 0,			0,
		0, cos(rad), -sin(rad), 0,
		0, sin(rad), cos(rad),  0,
		0, 0,		 0,			1);
}

Matrix4 Matrix4::CreateYRotation(double rad)
{
	/* Note that the sin() in the first row is positive and 
	 * sin() in the third row is negative. This is because
	 * we are using right-hand coordinate. 
	 */
	return Matrix4(
		cos(rad),  0, sin(rad), 0,
		0,		   1, 0,		0,
		-sin(rad), 0, cos(rad), 0,
		0,		   0, 0,		1);
}

Matrix4 Matrix4::CreateZRotation(double rad)
{
	return Matrix4(
		cos(rad), -sin(rad), 0, 0,
		sin(rad), cos(rad),  0, 0,
		0,		  0,		 1, 0,
		0,		  0,		 0, 1);
}

Matrix4 Matrix4::CreateTranslation(Vector3& vec)
{
	return Matrix4(
		1, 0, 0, vec.x,
		0, 1, 0, vec.y,
		0, 0, 1, vec.z,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateTranslation(double transX, double transY, double transZ)
{
	return Matrix4(
		1, 0, 0, transX,
		0, 1, 0, transY,
		0, 0, 1, transZ,
		0, 0, 0, 1);
}

Matrix4 Matrix4::CreateScale(Vector3& vec)
{
	return Matrix4(
		vec.x, 0,	  0,	 0,
		0,	   vec.y, 0,	 0,
		0,	   0,	  vec.z, 0,
		0,	   0,	  0,	 1);
}

Matrix4 Matrix4::CreateScale(double scaleX, double scaleY, double scaleZ)
{
	return Matrix4(
		scaleX, 0,		0,		0,
		0,		scaleY, 0,		0,
		0,		0,		scaleZ, 0,
		0,		0,		0,		1);
}

double Matrix4::At(int row, int col) const
{
	return val[row][col];
}

void Matrix4::Set(int row, int col, double data)
{
	val[row][col] = data;
}

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

Matrix4 Matrix4::GetInverse(void) const
{
	Matrix4 temp = *this;
	temp.Invert();
	return temp;
}

void Matrix4::Transpose(void)
{
	double
		t01 = val[0][1], t02 = val[0][2], t03 = val[0][3],
		t12 = val[1][2], t13 = val[1][3], t23 = val[2][3];

	val[0][1] = val[1][0]; val[0][2] = val[2][0]; val[0][3] = val[3][0];
	val[1][2] = val[2][1]; val[1][3] = val[3][1]; val[2][3] = val[3][2];

	val[0][1] = t01, val[0][2] = t02, val[0][3] = t03,
	val[1][2] = t12, val[1][3] = t13, val[2][3] = t23;
}


Matrix4 Matrix4::GetTranspose(void) const
{
	return Matrix4(
		val[0][0], val[1][0], val[2][0], val[3][0],
		val[0][1], val[1][1], val[2][1], val[3][1],
		val[0][2], val[1][2], val[2][2], val[3][2],
		val[0][3], val[1][3], val[2][3], val[3][3]);
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




#pragma region GameObject

#pragma endregion
