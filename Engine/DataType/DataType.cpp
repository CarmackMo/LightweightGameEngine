#include "DataType.h"

using namespace Engine;

#pragma region Vector2

const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector2 Vector2::Left(-1.0f, 0.0f);
const Vector2 Vector2::Right(1.0f, 0.0f);
const Vector2 Vector2::Up(0.0f, 1.0f);
const Vector2 Vector2::Down(0.0f, -1.0f);

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

Matrix4::Matrix4(
	double x11, double x12, double x13, double x14,
	double x21, double x22, double x23, double x24,
	double x31, double x32, double x33, double x34,
	double x41, double x42, double x43, double x44) :
	m11(x11), m12(x12), m13(x13), m14(x14),
	m21(x21), m22(x22), m23(x23), m24(x24),
	m31(x31), m32(x32), m33(x33), m34(x34),
	m41(x42), m42(x42), m43(x43), m44(x44)
{ }

Matrix4::Matrix4(const Matrix4& other) :
	m11(other.m11), m12(other.m12), m13(other.m13), m14(other.m14),
	m21(other.m21), m22(other.m22), m23(other.m23), m24(other.m24),
	m31(other.m31), m32(other.m32), m33(other.m33), m34(other.m34),
	m41(other.m41), m42(other.m42), m43(other.m43), m44(other.m44)
{ }

Matrix4& Matrix4::operator=(const Matrix4& other)
{
	m11 = other.m11; m12 = other.m12; m13 = other.m13; m14 = other.m14;
	m21 = other.m21; m22 = other.m22; m23 = other.m23; m24 = other.m24;
	m31 = other.m31; m32 = other.m32; m33 = other.m33; m34 = other.m34;
	m41 = other.m41; m42 = other.m42; m43 = other.m43; m44 = other.m44;
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



#pragma endregion




#pragma region GameObject

#pragma endregion
