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




#pragma region Vector4

template <typename T>
inline Vector4<T>::Vector4()
{
	val[0] = static_cast<T>(0), val[1] = static_cast<T>(0);
	val[2] = static_cast<T>(0), val[3] = static_cast<T>(0);
}


template <typename T>
inline Vector4<T>::Vector4(T w, T x, T y, T z)
{
	val[0] = w, val[1] = x, val[2] = y, val[3] = z;
}


template <typename T>
inline Vector4<T>::Vector4(const Vector4<T>& other)
{
	val[0] = other[0], val[1] = other[1];
	val[2] = other[2], val[3] = other[3];
}


template <typename T>
inline T& Vector4<T>::operator[] (int idx)
{
	return val[idx];
}


template <typename T>
inline const T& Vector4<T>::operator[] (int idx) const
{
	return val[idx];
}


template <typename T>
template <typename U>
inline Vector4<T> Vector4<T>::ConvertType(const Vector4<U>& other)
{
	return Vector4<T>(static_cast<T>(other[0]), static_cast<T>(other[1]), static_cast<T>(other[2]), static_cast<T>(other[3]));
}


#pragma endregion
