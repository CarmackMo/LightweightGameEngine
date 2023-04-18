#pragma once



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
inline Vector2<T> Vector2<T>::ConvertType(const Vector2<U>& other)
{
	return Vector2<T>(static_cast<T>(other[0]), static_cast<T>(other[1]));
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
inline Vector3<T> Vector3<T>::ConvertType(const Vector3<U>& other)
{
	return Vector3<T>(static_cast<T>(other[0]), static_cast<T>(other[1]), static_cast<T>(other[2]));
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

#pragma endregion
