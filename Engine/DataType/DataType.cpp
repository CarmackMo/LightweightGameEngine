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

#pragma endregion




#pragma region GameObject

#pragma endregion
