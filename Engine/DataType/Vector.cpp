#include "Vector.h"

using namespace Engine;


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
		return w;
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