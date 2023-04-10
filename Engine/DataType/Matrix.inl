#pragma once
using namespace Engine;


#pragma region Matrix3

template <typename T>
inline Matrix3<T>::Matrix3()
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0][0] = static_cast<T>(0); val[0][1] = static_cast<T>(0); val[0][2] = static_cast<T>(0);
	val[1][0] = static_cast<T>(0); val[1][1] = static_cast<T>(0); val[1][2] = static_cast<T>(0);
	val[2][0] = static_cast<T>(0); val[2][1] = static_cast<T>(0); val[2][2] = static_cast<T>(0);
}


template <typename T>
inline Matrix3<T>::Matrix3(
	T x00, T x01, T x02,
	T x10, T x11, T x12,
	T x20, T x21, T x22)
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0][0] = x00; val[0][1] = x01; val[0][2] = x02;
	val[1][0] = x10; val[1][1] = x11; val[1][2] = x12;
	val[2][0] = x20; val[2][1] = x21; val[2][2] = x22;
}


template <typename T>
inline Matrix3<T>::Matrix3(const Matrix3<T>& other)
{
	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2];
	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2];
	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2];
}

template <typename T>
template <typename U>
inline Matrix3<U> Matrix3<T>::GetInverse() const
{
	Matrix3<T> ori = *this;
	Matrix3<U> res = ori.CovertToType<U>();
	res.Invert();
	return res;
}


template <typename T>
inline void Matrix3<T>::Transpose(void)
{
	T t01 = val[0][1], t02 = val[0][2], t12 = val[1][2];

	val[0][1] = val[1][0]; val[0][2] = val[2][0]; val[1][2] = val[2][1]; 

	val[1][0] = t01; val[2][0] = t02; val[2][1] = t12;
}


template <typename T>
inline Matrix3<T> Matrix3<T>::GetTranspose(void) const
{
	return Matrix3<T>(
		val[0][0], val[1][0], val[2][0],
		val[0][1], val[1][1], val[2][1],
		val[0][2], val[1][2], val[2][2]);
}


template <typename T>
template <typename U>
inline Matrix3<U> Matrix3<T>::CovertToType()
{
	return Matrix3<U>(
		static_cast<U>(val[0][0]), static_cast<U>(val[0][1]), static_cast<U>(val[0][2]),
		static_cast<U>(val[1][0]), static_cast<U>(val[1][1]), static_cast<U>(val[1][2]),
		static_cast<U>(val[2][0]), static_cast<U>(val[2][1]), static_cast<U>(val[2][2]));
}



template <typename T>
inline T* Matrix3<T>::operator[] (int row)
{
	return val[row];
}


template <typename T>
inline const T* Matrix3<T>::operator[] (int row) const
{
	return val[row];
}

template <typename T>
inline Matrix3<T> Matrix3<T>::operator+ (const Matrix3<T>& other) const
{
	return Matrix3<T>(
		val[0][0] + other[0][0], val[0][1] + other[0][1], val[0][2] + other[0][2],
		val[1][0] + other[1][0], val[1][1] + other[1][1], val[1][2] + other[1][2],
		val[2][0] + other[2][0], val[2][1] + other[2][1], val[2][2] + other[2][2]);
}


template <typename T>
inline Matrix3<T> Matrix3<T>::operator- (const Matrix3<T>& other) const
{
	return Matrix3<T>(
		val[0][0] - other[0][0], val[0][1] - other[0][1], val[0][2] - other[0][2],
		val[1][0] - other[1][0], val[1][1] - other[1][1], val[1][2] - other[1][2],
		val[2][0] - other[2][0], val[2][1] - other[2][1], val[2][2] - other[2][2]);
}


template <typename T>
inline Matrix3<T> Matrix3<T>::operator* (T num) const
{
	return Matrix3<T>(
		val[0][0] * num, val[0][1] * num, val[0][2] * num,
		val[1][0] * num, val[1][1] * num, val[1][2] * num,
		val[2][0] * num, val[2][1] * num, val[2][2] * num);
}


template <typename T>
inline Matrix3<T> Matrix3<T>::operator/ (T num) const
{
	return Matrix3<T>(
		val[0][0] / num, val[0][1] / num, val[0][2] / num,
		val[1][0] / num, val[1][1] / num, val[1][2] / num,
		val[2][0] / num, val[2][1] / num, val[2][2] / num);
}


template <typename T>
inline Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& other)
{
	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2];
	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2];
	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2];
	return *this;
}


template <typename T>
inline Matrix3<T> Matrix3<T>::CreateIdentity(void)
{
	Matrix3<double> res = Matrix3<double>(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1);
	return res.CovertToType<T>();
}


template <typename T>
inline Matrix3<T> Matrix3<T>::CreateRotation(double rad)
{
	Matrix3<double> res = Matrix3<double>(
		cos(rad), -sin(rad), 0,
		sin(rad), cos(rad),  0,
		0,		  0,		 1);
	return res.CovertToType<T>();
}


template <typename T>
inline Matrix3<T> Matrix3<T>::CreateTranslation(Vector2<T>& vec)
{
	Matrix3<double> res = Matrix3<double>(
		1, 0, vec[0],
		0, 1, vec[1],
		0, 0, 1);
	return res.CovertToType<T>();
}


template <typename T>
inline Matrix3<T> Matrix3<T>::CreateTranslation(T transX, T transY)
{
	Matrix3<double> res = Matrix3<double>(
		1, 0, transX,
		0, 1, transY,
		0, 0, 1);
	return res.CovertToType<T>();
}


template <typename T>
inline Matrix3<T> Matrix3<T>::CreateScale(Vector2<T>& vec)
{
	Matrix3<double> res = Matrix3<double>(
		vec[0], 0,		0,
		0,		vec[1], 0,
		0,		0,		1);
	return res.CovertToType<T>();
}


template <typename T>
inline Matrix3<T> Matrix3<T>::CreateScale(T scaleX, T scaleY)
{
	Matrix3<double> res = Matrix3<double>(
		scaleX, 0,		0,
		0,		scaleY, 0,
		0,		0,		1);
	return res.CovertToType<T>();
}

#pragma endregion




#pragma region Matrix4

template <typename T>
inline Matrix4<T>::Matrix4()
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0][0] = static_cast<T>(0); val[0][1] = static_cast<T>(0); val[0][2] = static_cast<T>(0); val[0][3] = static_cast<T>(0);
	val[1][0] = static_cast<T>(0); val[1][1] = static_cast<T>(0); val[1][2] = static_cast<T>(0); val[1][3] = static_cast<T>(0);
	val[2][0] = static_cast<T>(0); val[2][1] = static_cast<T>(0); val[2][2] = static_cast<T>(0); val[2][3] = static_cast<T>(0);
	val[3][0] = static_cast<T>(0); val[3][1] = static_cast<T>(0); val[3][2] = static_cast<T>(0); val[3][3] = static_cast<T>(0);
}

template <typename T>
inline Matrix4<T>::Matrix4(
	T x00, T x01, T x02, T x03,
	T x10, T x11, T x12, T x13,
	T x20, T x21, T x22, T x23,
	T x30, T x31, T x32, T x33)
{
	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
	{
		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
	}

	val[0][0] = x00; val[0][1] = x01; val[0][2] = x02; val[0][3] = x03;
	val[1][0] = x10; val[1][1] = x11; val[1][2] = x12; val[1][3] = x13;
	val[2][0] = x20; val[2][1] = x21; val[2][2] = x22; val[2][3] = x23;
	val[3][0] = x30; val[3][1] = x31; val[3][2] = x32; val[3][3] = x33;
}

template <typename T>
inline Matrix4<T>::Matrix4(const Matrix4<T>& other)
{
	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2]; val[0][3] = other[0][3];
	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2]; val[1][3] = other[1][3];
	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2]; val[2][3] = other[2][3];
	val[3][0] = other[3][0]; val[3][1] = other[3][1]; val[3][2] = other[3][2]; val[3][3] = other[3][3];
}

template <typename T>
template <typename U>
inline Matrix4<U> Matrix4<T>::GetInverse() const
{
	Matrix4<T> ori = *this;
	Matrix4<U> res = ori.CovertToType<U>();
	res.Invert();
	return res;
}

template <typename T>
inline void Matrix4<T>::Transpose(void)
{
	T t01 = val[0][1], t02 = val[0][2], t03 = val[0][3],
	  t12 = val[1][2], t13 = val[1][3], t23 = val[2][3];

	val[0][1] = val[1][0]; val[0][2] = val[2][0]; val[0][3] = val[3][0];
	val[1][2] = val[2][1]; val[1][3] = val[3][1]; val[2][3] = val[3][2];

	val[1][0] = t01, val[2][0] = t02, val[3][0] = t03,
	val[2][1] = t12, val[3][1] = t13, val[3][2] = t23;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::GetTranspose(void) const
{
	return Matrix4<T>(
		val[0][0], val[1][0], val[2][0], val[3][0],
		val[0][1], val[1][1], val[2][1], val[3][1],
		val[0][2], val[1][2], val[2][2], val[3][2],
		val[0][3], val[1][3], val[2][3], val[3][3]);
}

template <typename T>
template <typename U>
inline Matrix4<U> Matrix4<T>::CovertToType()
{
	return Matrix4<U>(
		static_cast<U>(val[0][0]), static_cast<U>(val[0][1]), static_cast<U>(val[0][2]), static_cast<U>(val[0][3]),
		static_cast<U>(val[1][0]), static_cast<U>(val[1][1]), static_cast<U>(val[1][2]), static_cast<U>(val[1][3]),
		static_cast<U>(val[2][0]), static_cast<U>(val[2][1]), static_cast<U>(val[2][2]), static_cast<U>(val[2][3]),
		static_cast<U>(val[3][0]), static_cast<U>(val[3][1]), static_cast<U>(val[3][2]), static_cast<U>(val[3][3]));
}

template <typename T>
inline T* Matrix4<T>::operator[] (int row)
{
	return val[row];
}

template <typename T>
inline const T* Matrix4<T>::operator[] (int row) const
{
	return val[row];
}

template <typename T>
inline Matrix4<T> Matrix4<T>::operator+ (const Matrix4<T>& other) const
{
	return Matrix4<T>(
		val[0][0] + other[0][0], val[0][1] + other[0][1], val[0][2] + other[0][2], val[0][3] + other[0][3],
		val[1][0] + other[1][0], val[1][1] + other[1][1], val[1][2] + other[1][2], val[1][3] + other[1][3],
		val[2][0] + other[2][0], val[2][1] + other[2][1], val[2][2] + other[2][2], val[2][3] + other[2][3],
		val[3][0] + other[3][0], val[3][1] + other[3][1], val[3][2] + other[3][2], val[3][3] + other[3][3]);
}

template <typename T>
inline Matrix4<T> Matrix4<T>::operator- (const Matrix4<T>& other) const
{
	return Matrix4<T>(
		val[0][0] - other[0][0], val[0][1] - other[0][1], val[0][2] - other[0][2], val[0][3] - other[0][3],
		val[1][0] - other[1][0], val[1][1] - other[1][1], val[1][2] - other[1][2], val[1][3] - other[1][3],
		val[2][0] - other[2][0], val[2][1] - other[2][1], val[2][2] - other[2][2], val[2][3] - other[2][3],
		val[3][0] - other[3][0], val[3][1] - other[3][1], val[3][2] - other[3][2], val[3][3] - other[3][3]);
}

template <typename T>
inline Matrix4<T> Matrix4<T>::operator* (T num) const
{
	return Matrix4<T>(
		val[0][0] * num, val[0][1] * num, val[0][2] * num, val[0][3] * num,
		val[1][0] * num, val[1][1] * num, val[1][2] * num, val[1][3] * num,
		val[2][0] * num, val[2][1] * num, val[2][2] * num, val[2][3] * num,
		val[3][0] * num, val[3][1] * num, val[3][2] * num, val[3][3] * num);
}

template <typename T>
inline Matrix4<T> Matrix4<T>::operator/ (T num) const
{
	return Matrix4<T>(
		val[0][0] / num, val[0][1] / num, val[0][2] / num, val[0][3] / num,
		val[1][0] / num, val[1][1] / num, val[1][2] / num, val[1][3] / num,
		val[2][0] / num, val[2][1] / num, val[2][2] / num, val[2][3] / num,
		val[3][0] / num, val[3][1] / num, val[3][2] / num, val[3][3] / num);
}

template <typename T>
inline Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& other)
{
	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2]; val[0][3] = other[0][3];
	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2]; val[1][3] = other[1][3];
	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2]; val[2][3] = other[2][3];
	val[3][0] = other[3][0]; val[3][1] = other[3][1]; val[3][2] = other[3][2]; val[3][3] = other[3][3];
	return *this;
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateIdentity(void)
{
	Matrix4<double> res = Matrix4<double>(
							1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1);
	return res.CovertToType<T>();
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateXRotation(double rad)
{
	Matrix4<double> res = Matrix4<double>(
							1, 0, 0, 0,
							0, cos(rad), -sin(rad), 0,
							0, sin(rad), cos(rad), 0,
							0, 0, 0, 1);
	return res.CovertToType<T>();
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateYRotation(double rad)
{
	/* Note that the sin() in the first row is positive and
	 * sin() in the third row is negative. This is because
	 * we are using right-hand coordinate.
	 */
	Matrix4<double> res = Matrix4<double>(
							cos(rad), 0, sin(rad), 0,
							0, 1, 0, 0,
							-sin(rad), 0, cos(rad), 0,
							0, 0, 0, 1);
	return res.CovertToType<T>();
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateZRotation(double rad)
{
	Matrix4<double> res = Matrix4<double>(
							cos(rad), -sin(rad), 0, 0,
							sin(rad), cos(rad), 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1);
	return res.CovertToType<T>();
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateTranslation(Vector3<T>& vec)
{
	Matrix4<double> res = Matrix4<double>(
							1, 0, 0, vec[0],
							0, 1, 0, vec[1],
							0, 0, 1, vec[2],
							0, 0, 0, 1);
	return res.CovertToType<T>();
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateTranslation(T transX, T transY, T transZ)
{
	Matrix4<double> res = Matrix4<double>(
							1, 0, 0, transX,
							0, 1, 0, transY,
							0, 0, 1, transZ,
							0, 0, 0, 1);
	return res.CovertToType<T>();
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateScale(Vector3<T>& vec)
{
	Matrix4<double> res = Matrix4<double>(
							vec[0], 0, 0, 0,
							0, vec[1], 0, 0,
							0, 0, vec[2], 0,
							0, 0, 0, 1);
	return res.CovertToType<T>();
}

template <typename T>
inline Matrix4<T> Matrix4<T>::CreateScale(T scaleX, T scaleY, T scaleZ)
{
	Matrix4<double> res = Matrix4<double>(
							scaleX, 0, 0, 0,
							0, scaleY, 0, 0,
							0, 0, scaleZ, 0,
							0, 0, 0, 1);
	return res.CovertToType<T>();
}


#pragma endregion

