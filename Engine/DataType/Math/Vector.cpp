#include "Vector.h"

using namespace Engine;

#pragma region Vector2

/* Constructor */
template <typename T>
inline Vector2<T>::Vector2()
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create vector2 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0] = static_cast<T>(0); val[1] = static_cast<T>(0);
}


template <typename T>
inline Vector2<T>::Vector2(T x, T y)
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create vector2 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0] = x; val[1] = y;
}


template <typename T>
inline Vector2<T>::Vector2(const Vector2<T>& other)
{
	val[0] = other[0]; val[1] = other[1];
}


template <typename T>
template <typename U>
inline Vector2<U> Vector2<T>::ConvertTo()
{
	return Vector2<U>(static_cast<U>(val[0]), static_cast<U>(val[1]));
}


template <typename T>
inline float Vector2<T>::Length() const
{
	float lengthSq = static_cast<float>(val[0] * val[0] + val[1] * val[1]);
	return sqrt(lengthSq);
}


template <typename T>
inline void Vector2<T>::Norm()
{
	float len = Length();

	if (!IsZero(len))
	{
		val[0] = static_cast<T>(val[0] / len);
		val[1] = static_cast<T>(val[1] / len);
	}
}


template <typename T>
inline Vector2<float> Vector2<T>::GetNorm() const
{
	float len = Length();

	if (IsZero(len))
		return Zero;
	else
		return Vector2<float>(val[0] / len, val[1] / len);
}


/* Self modifying operators */
template <typename T>
inline void Vector2<T>::operator+= (const Vector2<T>& other)
{
	val[0] += other[0]; val[1] += other[1];
}


template <typename T>
inline void Vector2<T>::operator-= (const Vector2<T>& other)
{
	val[0] -= other[0]; val[1] -= other[1];
}


template <typename T>
inline void Vector2<T>::operator*= (const Vector2<T>& other)
{
	val[0] *= other[0]; val[1] *= other[1];
}


template <typename T>
inline void Vector2<T>::operator*= (T num)
{
	val[0] *= num; val[1] *= num;
}


template <typename T>
inline void Vector2<T>::operator/= (const Vector2<T>& other)
{
	val[0] /= other[0]; val[1] /= other[1];
}


template <typename T>
inline void Vector2<T>::operator/= (T num)
{
	val[0] /= num; val[1] /= num;
}


/* Modifying operators */
template <typename T>
inline Vector2<T> Vector2<T>::operator+ (const Vector2<T>& other) const
{
	return Vector2<T>(val[0] + other[0], val[1] + other[1]);
}


template <typename T>
inline Vector2<T> Vector2<T>::operator- (const Vector2<T>& other) const
{
	return Vector2<T>(val[0] - other[0], val[1] - other[1]);
}


template <typename T>
inline Vector2<T> Vector2<T>::operator* (const Vector2<T>& other) const
{
	return Vector2<T>(val[0] * other[0], val[1] * other[1]);
}


template <typename T>
inline Vector2<T> Vector2<T>::operator* (T num) const
{
	return Vector2<T>(val[0] * num, val[1] * num);
}


template <typename T>
inline Vector2<T> Vector2<T>::operator/ (const Vector2<T>& other) const
{
	return Vector2<T>(val[0] / other[0], val[1] / other[1]);
}


template <typename T>
inline Vector2<T> Vector2<T>::operator/ (T num) const
{
	return Vector2<T>(val[0] / num, val[1] / num);
}


/* Assignment operators */
template <typename T>
inline Vector2<T>& Vector2<T>::operator= (const Vector2<T>& other)
{
	val[0] = other[0]; val[1] = other[1];
	return *this;
}


/* Comparison operators */
template <typename T>
inline bool Vector2<T>::operator== (const Vector2<T>& other) const
{
	return (val[0] == other[0]) && (val[1] == other[1]);
}


template <typename T>
inline bool Vector2<T>::operator!= (const Vector2<T>& other) const
{
	return (val[0] != other[0]) || (val[1] != other[1]);
}


/* Negate */
template <typename T>
inline Vector2<T> Vector2<T>::operator- (void) const
{
	return Vector2<T>(-val[0], -val[1]);
}


/* Indexing */
template <typename T>
inline T& Vector2<T>::operator[] (int idx)
{
	return val[idx];
}


template <typename T>
inline const T& Vector2<T>::operator[] (int idx) const
{
	return val[idx];
}


/* Explicit implementation of static constant instances for each type */
const Vector2<int> Vector2<int>::Zero(0, 0);
const Vector2<int> Vector2<int>::Left(-1, 0);
const Vector2<int> Vector2<int>::Right(1, 0);
const Vector2<int> Vector2<int>::Up(0, 1);
const Vector2<int> Vector2<int>::Down(0, -1);

const Vector2<short> Vector2<short>::Zero(0, 0);
const Vector2<short> Vector2<short>::Left(-1, 0);
const Vector2<short> Vector2<short>::Right(1, 0);
const Vector2<short> Vector2<short>::Up(0, 1);
const Vector2<short> Vector2<short>::Down(0, -1);

const Vector2<long> Vector2<long>::Zero(0, 0);
const Vector2<long> Vector2<long>::Left(-1, 0);
const Vector2<long> Vector2<long>::Right(1, 0);
const Vector2<long> Vector2<long>::Up(0, 1);
const Vector2<long> Vector2<long>::Down(0, -1);

const Vector2<float> Vector2<float>::Zero(0.0, 0.0);
const Vector2<float> Vector2<float>::Left(-1.0, 0.0);
const Vector2<float> Vector2<float>::Right(1.0, 0.0);
const Vector2<float> Vector2<float>::Up(0.0, 1.0);
const Vector2<float> Vector2<float>::Down(0.0, -1.0);

const Vector2<double> Vector2<double>::Zero(0, 0);
const Vector2<double> Vector2<double>::Left(-1, 0);
const Vector2<double> Vector2<double>::Right(1, 0);
const Vector2<double> Vector2<double>::Up(0, 1);
const Vector2<double> Vector2<double>::Down(0, -1);

#pragma endregion



#pragma region Vector3

/* Constructor */
template <typename T>
inline Vector3<T>::Vector3()
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create vector3 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0] = static_cast<T>(0); val[1] = static_cast<T>(0); val[2] = static_cast<T>(0);
}


template <typename T>
inline Vector3<T>::Vector3(T x, T y, T z)
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create vector3 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0] = x; val[1] = y; val[2] = z;
}


template <typename T>
inline Vector3<T>::Vector3(const Vector3<T>& other)
{
	val[0] = other[0]; val[1] = other[1]; val[2] = other[2];
}


template <typename T>
template <typename U>
inline Vector3<U> Vector3<T>::CovertTo()
{
	return Vector3<U>(
		static_cast<U>(val[0]),
		static_cast<U>(val[1]),
		static_cast<U>(val[2]));
}


template <typename T>
inline T Vector3<T>::Dot(const Vector3<T>& other) const
{
	return val[0] * other[0] + val[1] * other[1] + val[2] * other[2];
}


template <typename T>
inline Vector3<T> Vector3<T>::Cross(const Vector3<T>& other) const
{
	return Vector3<T>(
		val[1] * other[2] - val[2] * other[1],
		val[2] * other[0] - val[0] * other[2],
		val[0] * other[1] - val[1] * other[0]);
}


template <typename T>
inline float Vector3<T>::Length() const
{
	float lengthSq = static_cast<float>(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]);
	return sqrt(lengthSq);
}


template <typename T>
inline void Vector3<T>::Norm()
{
	float len = Length();

	if (!IsZero(len))
	{
		val[0] = static_cast<T>(val[0] / len);
		val[1] = static_cast<T>(val[1] / len);
		val[2] = static_cast<T>(val[2] / len);
	}
}


template <typename T>
inline Vector3<float> Vector3<T>::GetNorm() const
{
	float len = Length();

	if (IsZero(len))
		return Zero;
	else
		return Vector3<float>(val[0] / len, val[1] / len, val[2] / len);
}


/* Self modifying operators */
template <typename T>
inline void Vector3<T>::operator+= (const Vector3<T>& other)
{
	val[0] += other[0]; val[1] += other[1]; val[2] += other[2];
}


template <typename T>
inline void Vector3<T>::operator-= (const Vector3<T>& other)
{
	val[0] -= other[0]; val[1] -= other[1]; val[2] -= other[2];
}


template <typename T>
inline void Vector3<T>::operator*= (Vector3<T> const& other)
{
	val[0] *= other[0]; val[1] *= other[1]; val[2] *= other[2];
}


template <typename T>
inline void Vector3<T>::operator*= (T num)
{
	val[0] *= num; val[1] *= num; val[2] *= num;
}


template <typename T>
inline void Vector3<T>::operator/= (const Vector3<T>& other)
{
	val[0] /= other[0]; val[1] /= other[1]; val[2] /= other[2];
}


template <typename T>
inline void Vector3<T>::operator/= (T num)
{
	val[0] /= num; val[1] /= num; val[2] /= num;
}


/* Modifying operators */
template <typename T>
inline Vector3<T> Vector3<T>::operator+ (const Vector3<T>& other) const
{
	return Vector3<T>(val[0] + other[0], val[1] + other[1], val[2] + other[2]);
}


template <typename T>
inline Vector3<T> Vector3<T>::operator- (const Vector3<T>& other) const
{
	return Vector3<T>(val[0] - other[0], val[1] - other[1], val[2] - other[2]);
}


template <typename T>
inline Vector3<T> Vector3<T>::operator* (const Vector3<T>& other) const
{
	return Vector3<T>(val[0] * other[0], val[1] * other[1], val[2] * other[2]);
}


template <typename T>
inline Vector3<T> Vector3<T>::operator* (T num) const
{
	return Vector3(val[0] * num, val[1] * num, val[2] * num);
}


template <typename T>
inline Vector3<T> Vector3<T>::operator/ (const Vector3<T>& other) const
{
	return Vector3<T>(val[0] / other[0], val[1] / other[1], val[2] / other[2]);
}


template <typename T>
inline Vector3<T> Vector3<T>::operator/ (T num) const
{
	return Vector3<T>(val[0] / num, val[1] / num, val[2] / num);
}


/* Assignment operators */
template <typename T>
inline Vector3<T>& Vector3<T>::operator= (const Vector3<T>& other)
{
	val[0] = other[0]; val[1] = other[1]; val[2] = other[2];
	return *this;
}


/* Comparison operators */
template <typename T>
inline bool Vector3<T>::operator== (const Vector3<T>& other) const
{
	return (val[0] == other[0]) && (val[1] == other[1]) && (val[2] == other[2]);
}


template <typename T>
inline bool Vector3<T>::operator!= (const Vector3<T>& other) const
{
	return (val[0] != other[0]) || (val[1] != other[1]) || (val[2] != other[2]);
}


/* Negate */
template <typename T>
inline Vector3<T> Vector3<T>::operator- (void) const
{
	return Vector3<T>(-val[0], -val[1], -val[2]);
}


/* Indexing */
template <typename T>
inline T& Vector3<T>::operator[] (int idx)
{
	return val[idx];
}


template <typename T>
inline const T& Vector3<T>::operator[] (int idx) const
{
	return val[idx];
}


/* Explicit implementation of static constant instances for each type */
const Vector3<int> Vector3<int>::Zero(0, 0, 0);
const Vector3<int> Vector3<int>::Left(-1, 0, 0);
const Vector3<int> Vector3<int>::Right(1, 0, 0);
const Vector3<int> Vector3<int>::Up(0, 1, 0);
const Vector3<int> Vector3<int>::Down(0, -1, 0);
const Vector3<int> Vector3<int>::Front(0, 0, 1);
const Vector3<int> Vector3<int>::Back(0, 0, -1);

const Vector3<short> Vector3<short>::Zero(0, 0, 0);
const Vector3<short> Vector3<short>::Left(-1, 0, 0);
const Vector3<short> Vector3<short>::Right(1, 0, 0);
const Vector3<short> Vector3<short>::Up(0, 1, 0);
const Vector3<short> Vector3<short>::Down(0, -1, 0);
const Vector3<short> Vector3<short>::Front(0, 0, 1);
const Vector3<short> Vector3<short>::Back(0, 0, -1);

const Vector3<long> Vector3<long>::Zero(0, 0, 0);
const Vector3<long> Vector3<long>::Left(-1, 0, 0);
const Vector3<long> Vector3<long>::Right(1, 0, 0);
const Vector3<long> Vector3<long>::Up(0, 1, 0);
const Vector3<long> Vector3<long>::Down(0, -1, 0);
const Vector3<long> Vector3<long>::Front(0, 0, 1);
const Vector3<long> Vector3<long>::Back(0, 0, -1);

const Vector3<float> Vector3<float>::Zero(0.0, 0.0, 0.0);
const Vector3<float> Vector3<float>::Left(-1.0, 0.0, 0.0);
const Vector3<float> Vector3<float>::Right(1.0, 0.0, 0.0);
const Vector3<float> Vector3<float>::Up(0.0, 1.0, 0.0);
const Vector3<float> Vector3<float>::Down(0.0, -1.0, 0.0);
const Vector3<float> Vector3<float>::Front(0.0, 0.0, 1.0);
const Vector3<float> Vector3<float>::Back(0.0, 0.0, -1.0);

const Vector3<double> Vector3<double>::Zero(0, 0, 0);
const Vector3<double> Vector3<double>::Left(-1, 0, 0);
const Vector3<double> Vector3<double>::Right(1, 0, 0);
const Vector3<double> Vector3<double>::Up(0, 1, 0);
const Vector3<double> Vector3<double>::Down(0, -1, 0);
const Vector3<double> Vector3<double>::Front(0, 0, 1);
const Vector3<double> Vector3<double>::Back(0, 0, -1);

#pragma endregion



#pragma region Vector4


template <typename T>
inline Vector4<T>::Vector4()
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create vector4 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0] = static_cast<T>(0), val[1] = static_cast<T>(0);
	val[2] = static_cast<T>(0), val[3] = static_cast<T>(0);
}


template <typename T>
inline Vector4<T>::Vector4(T w, T x, T y, T z)
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create vector4 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0] = w, val[1] = x, val[2] = y, val[3] = z;
}


template <typename T>
inline Vector4<T>::Vector4(const Vector4<T>& other)
{
	val[0] = other[0], val[1] = other[1];
	val[2] = other[2], val[3] = other[3];
}


template <typename T>
template <typename U>
inline Vector4<T> Vector4<T>::ConvertType(const Vector4<U>& other)
{
	return Vector4<T>(static_cast<T>(other[0]), static_cast<T>(other[1]), static_cast<T>(other[2]), static_cast<T>(other[3]));
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
inline bool Vector4<T>::operator== (const Vector4<T>& other) const
{
	return (val[0] == other[0]) && (val[1] == other[1]) && (val[2] == other[2]) && (val[3] == other[3]);
}


template <typename T>
inline Vector4<T> Vector4<T>::operator* (const Vector4<T>& other) const
{
	return Vector4<T>(val[0] * other[0], val[1] * other[1], val[2] * other[2], val[3] * other[3]);
}


template <typename T>
inline Vector4<T> Vector4<T>::operator* (T num) const
{
	return Vector4(val[0] * num, val[1] * num, val[2] * num, val[3] * num);
}


#pragma endregion





