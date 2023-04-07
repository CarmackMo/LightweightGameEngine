#pragma once

using namespace Engine;


#pragma region Vector3

/* Constructor */
inline Vector3::Vector3() :
	x(0), y(0), z(0)
{ }
inline Vector3::Vector3(double x, double y, double z) :
	x(x), y(y), z(z)
{ }
inline Vector3::Vector3(const Vector3& other) :
	x(other.x), y(other.y), z(other.z)
{ }

/* Self modifying operators */
inline void Vector3::operator+= (const Vector3& vec)
{
	x += vec.x; y += vec.y; z += vec.z;
}
inline void Vector3::operator-= (const Vector3& vec)
{
	x -= vec.x; y -= vec.y; x -= vec.z;
}

inline void Vector3::operator*= (Vector3 const& vec)
{
	x *= vec.x; y *= vec.y; z *= vec.z;
}
inline void Vector3::operator*= (double num)
{
	x *= num; y *= num; z *= num;
}

inline void Vector3::operator/= (const Vector3& vec)
{
	x /= vec.x; y /= vec.y; z /= vec.z;
}
inline void Vector3::operator/= (double num)
{
	x /= num; y /= num; z /= num;
}

/* Modifying operators */
inline Vector3 Vector3::operator+ (const Vector3& vec) const
{
	return Vector3(x + vec.x, y + vec.y, z + vec.z);
}
inline Vector3 Vector3::operator- (const Vector3& vec) const
{
	return Vector3(x - vec.x, y - vec.y, z - vec.z);
}

inline Vector3 Vector3::operator* (const Vector3& vec) const
{
	return Vector3(x * vec.x, y * vec.y, z * vec.z);
}
inline Vector3 Vector3::operator* (double num) const
{
	return Vector3(x * num, y * num, z * num);
}
inline Vector3 operator* (double left, Vector3 right)
{
	return Vector3(left * right.x, left * right.y, left * right.z);
}

inline Vector3 Vector3::operator/ (const Vector3& vec) const
{
	return Vector3(x / vec.x, y / vec.y, z / vec.z);
}
inline Vector3 Vector3::operator/ (double num) const
{
	return Vector3(x / num, y / num, z / num);
}

/* Assignment operators */
inline Vector3& Vector3::operator= (const Vector3& vec)
{
	x = vec.x; y = vec.y, z = vec.z;
	return *this;
}

/* Comparison operators */
inline bool Vector3::operator== (const Vector3& vec) const
{
	return (x == vec.x) && (y == vec.y) && (z == vec.z);
}

inline bool Vector3::operator!= (const Vector3& vec) const
{
	return (x != vec.x) || (y != vec.y) || (z != vec.z);
}

/* Negate */
inline Vector3 Vector3::operator- (void) const
{
	return Vector3(-x, -y, -z);
}


#pragma endregion