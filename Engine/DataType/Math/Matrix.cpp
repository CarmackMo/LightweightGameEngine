//#include "Matrix.h"
//#include "Debugger.h"




//namespace Engine
//{
//using namespace Engine::Debugger;


#pragma region Matrix3

//template <typename T>
//inline Matrix3<T>::Matrix3()
//{
//	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
//	{
//		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
//	}
//
//	val[0][0] = static_cast<T>(0); val[0][1] = static_cast<T>(0); val[0][2] = static_cast<T>(0);
//	val[1][0] = static_cast<T>(0); val[1][1] = static_cast<T>(0); val[1][2] = static_cast<T>(0);
//	val[2][0] = static_cast<T>(0); val[2][1] = static_cast<T>(0); val[2][2] = static_cast<T>(0);
//}
//
//
//template <typename T>
//inline Matrix3<T>::Matrix3(
//	T x00, T x01, T x02,
//	T x10, T x11, T x12,
//	T x20, T x21, T x22)
//{
//	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
//	{
//		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
//	}
//
//	val[0][0] = x00; val[0][1] = x01; val[0][2] = x02;
//	val[1][0] = x10; val[1][1] = x11; val[1][2] = x12;
//	val[2][0] = x20; val[2][1] = x21; val[2][2] = x22;
//}
//
//
//template <typename T>
//inline Matrix3<T>::Matrix3(const Matrix3<T>& other)
//{
//	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2];
//	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2];
//	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2];
//}

//
//template <typename T>
//inline Vector3<T> Matrix3<T>::GetRow(int row) const
//{
//	return Vector3<T>(val[row][0], val[row][1], val[row][2]);
//}
//
//
//template <typename T>
//inline Vector3<T> Matrix3<T>::GetCol(int col) const
//{
//	return Vector3<T>(val[0][col], val[1][col], val[2][col]);
//}


//template <typename T>
//T Matrix3<T>::Det(int row, int col) const
//{
//	T res = static_cast<T>(pow(-1, row + col)) * 
//		(val[(row + 1) % 3][(col + 1) % 3] * val[(row + 2) % 3][(col + 2) % 3] -
//		 val[(row + 1) % 3][(col + 2) % 3] * val[(row + 2) % 3][(col + 1) % 3]);
//
//	return res;
//}
//
//
//template <typename T>
//void Matrix3<T>::Invert(void)
//{
//	/* Inverse of matrix M equals to Adj(M)/Det(M).
//	 * Calcualte the determinant of this instance, using first row elements as pivots */
//	T det = val[0][0] * Det(0, 0) - val[0][1] * Det(0, 1) + val[0][2] * Det(0, 2);
//
//	/* Calculate the cofactors matrix of this instance first, the value at (i,j)
//	 * equals to Det(i,j) */
//	Matrix3<T> adjugate;
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			adjugate[i][j] = static_cast<T>(pow(-1, i + j)) * Det(i, j);
//		}
//	}
//
//	/* Calculate the adjugate matrix by transposing the cofactors matrix */
//	adjugate.Transpose();
//
//	/* Update the elements of this instance to make it inversed */
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			val[i][j] = adjugate[i][j] / det;
//		}
//	}
//}
//
//
//template <typename T>
//template <typename U>
//inline Matrix3<U> Matrix3<T>::GetInverse() const
//{
//	Matrix3<U> res = this->ConvertTo<U>();
//	res.Invert();
//	return res;
//}


//template <typename T>
//inline void Matrix3<T>::Transpose(void)
//{
//	std::swap(val[0][1], val[1][0]);
//	std::swap(val[0][2], val[2][0]);
//	std::swap(val[1][2], val[2][1]);
//
//	//T t01 = val[0][1], t02 = val[0][2], t12 = val[1][2];
//
//	//val[0][1] = val[1][0]; val[0][2] = val[2][0]; val[1][2] = val[2][1];
//
//	//val[1][0] = t01; val[2][0] = t02; val[2][1] = t12;
//}


//template <typename T>
//inline Matrix3<T> Matrix3<T>::GetTranspose(void) const
//{
//	return Matrix3<T>(
//		val[0][0], val[1][0], val[2][0],
//		val[0][1], val[1][1], val[2][1],
//		val[0][2], val[1][2], val[2][2]);
//}


//template <typename T>
//template <typename U>
//inline Matrix3<U> Matrix3<T>::ConvertTo() const
//{
//	return Matrix3<U>(
//		static_cast<U>(val[0][0]), static_cast<U>(val[0][1]), static_cast<U>(val[0][2]),
//		static_cast<U>(val[1][0]), static_cast<U>(val[1][1]), static_cast<U>(val[1][2]),
//		static_cast<U>(val[2][0]), static_cast<U>(val[2][1]), static_cast<U>(val[2][2]));
//}


//template <typename T>
//inline T* Matrix3<T>::operator[] (int row)
//{
//	return val[row];
//}
//
//
//template <typename T>
//inline const T* Matrix3<T>::operator[] (int row) const
//{
//	return val[row];
//}


//template <typename T>
//inline Matrix3<T> Matrix3<T>::operator+ (const Matrix3<T>& other) const
//{
//	return Matrix3<T>(
//		val[0][0] + other[0][0], val[0][1] + other[0][1], val[0][2] + other[0][2],
//		val[1][0] + other[1][0], val[1][1] + other[1][1], val[1][2] + other[1][2],
//		val[2][0] + other[2][0], val[2][1] + other[2][1], val[2][2] + other[2][2]);
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::operator- (const Matrix3<T>& other) const
//{
//	return Matrix3<T>(
//		val[0][0] - other[0][0], val[0][1] - other[0][1], val[0][2] - other[0][2],
//		val[1][0] - other[1][0], val[1][1] - other[1][1], val[1][2] - other[1][2],
//		val[2][0] - other[2][0], val[2][1] - other[2][1], val[2][2] - other[2][2]);
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::operator* (T num) const
//{
//	return Matrix3<T>(
//		val[0][0] * num, val[0][1] * num, val[0][2] * num,
//		val[1][0] * num, val[1][1] * num, val[1][2] * num,
//		val[2][0] * num, val[2][1] * num, val[2][2] * num);
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::operator/ (T num) const
//{
//	return Matrix3<T>(
//		val[0][0] / num, val[0][1] / num, val[0][2] / num,
//		val[1][0] / num, val[1][1] / num, val[1][2] / num,
//		val[2][0] / num, val[2][1] / num, val[2][2] / num);
//}

//
//template <typename T>
//inline Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& other)
//{
//	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2];
//	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2];
//	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2];
//	return *this;
//}
//
//
//template <typename T>
//Matrix3<T> Matrix3<T>::operator* (const Matrix3<T>& other) const
//{
//	Matrix3<T> res = Matrix3<T>();
//
//	/* Iterate all rows in matrix A */
//	for (int rowA = 0; rowA < 3; rowA++)
//	{
//		/* Iterate all columns in matrix B */
//		for (int colB = 0; colB < 3; colB++)
//		{
//			T sum = 0;
//			/* Iterator that repeat 4 times */
//			for (int i = 0; i < 3; i++)
//			{
//				sum += val[rowA][i] * other[i][colB];
//			}
//			res[rowA][colB] = sum;
//		}
//	}
//	return res;
//}
//
//
//template <typename T>
//bool Matrix3<T>::operator== (const Matrix3<T>& other) const
//{
//	for (int row = 0; row < 3; row++)
//	{
//		for (int col = 0; col < 3; col++)
//		{
//			if (AreEqual(static_cast<float>(val[row][col]), static_cast<float>(other[row][col])) == false)
//				return false;
//		}
//	}
//	return true;
//}


//template <typename T>
//inline Matrix4<T> Matrix3<T>::ToTransform() const
//{
//	return Matrix4<T>(
//		val[0][0], val[0][1], val[0][2], 0.0f,
//		val[1][0], val[1][1], val[1][2], 0.0f,
//		val[2][0], val[2][1], val[2][2], 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f);
//}
//
//
//template <typename T>
//inline Vector2<T> Matrix3<T>::TransformPoint(const Vector2<T>& point) const
//{
//	/* Convert the point's coordinates to homogeneous coordinate */
//	Vector3<T> Point = *this * Vector3<T>(point[0], point[1], 1.0f);
//
//	/* The 3rd element of the resulting transformed point is the homogeneous coordinate.
//	 * The variable "invHomo" stores the inverse of the homogeneous coordinate. The
//	 * purpose of dividing the transformed point by "invHomo" is to normalize the coordinates
//	 * This normalization step ensures that the transformed point retains its relative
//	 * position in 3D space. It effectively brings the point back from homogeneous coordinates
//	 * to Cartesian coordinates. */
//	float invHomo = 1.0f / Point[2];
//	return Vector2<T>(Point[0] * invHomo, Point[1] * invHomo);
//}


//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateIdentity(void)
//{
//	Matrix3<double> res = Matrix3<double>(
//		1, 0, 0,
//		0, 1, 0,
//		0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateXRotation(double rad)
//{
//	double sin_ = sin(rad);
//	double cos_ = cos(rad);
//
//	Matrix3<double> res = Matrix3<double>(
//		1, 0, 0,
//		0, cos_, -sin_,
//		0, sin_, cos_);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateYRotation(double rad)
//{
//	double sin_ = sin(rad);
//	double cos_ = cos(rad);
//
//	Matrix3<double> res = Matrix3<double>(
//		cos_, 0, sin_,
//		0, 1, 0,
//		-sin_, 0, cos_);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateZRotation(double rad)
//{
//	double sin_ = sin(rad);
//	double cos_ = cos(rad);
//
//	Matrix3<double> res = Matrix3<double>(
//		cos_, -sin_, 0,
//		sin_, cos_, 0,
//		0, 0, 1);
//	return res.ConvertTo<T>();
//}


//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateTranslation(const Vector2<T>& vec)
//{
//	Matrix3<double> res = Matrix3<double>(
//		1, 0, vec[0],
//		0, 1, vec[1],
//		0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateTranslation(T transX, T transY)
//{
//	Matrix3<double> res = Matrix3<double>(
//		1, 0, transX,
//		0, 1, transY,
//		0, 0, 1);
//	return res.ConvertTo<T>();
//}

//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateScale(const Vector2<T>& vec)
//{
//	Matrix3<double> res = Matrix3<double>(
//		vec[0], 0, 0,
//		0, vec[1], 0,
//		0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix3<T> Matrix3<T>::CreateScale(T scaleX, T scaleY)
//{
//	Matrix3<double> res = Matrix3<double>(
//		scaleX, 0, 0,
//		0, scaleY, 0,
//		0, 0, 1);
//	return res.ConvertTo<T>();
//}


#pragma endregion




#pragma region Matrix4


//template <typename T>
//inline Matrix4<T>::Matrix4()
//{
//	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
//	{
//		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
//	}
//
//	val[0][0] = static_cast<T>(0); val[0][1] = static_cast<T>(0); val[0][2] = static_cast<T>(0); val[0][3] = static_cast<T>(0);
//	val[1][0] = static_cast<T>(0); val[1][1] = static_cast<T>(0); val[1][2] = static_cast<T>(0); val[1][3] = static_cast<T>(0);
//	val[2][0] = static_cast<T>(0); val[2][1] = static_cast<T>(0); val[2][2] = static_cast<T>(0); val[2][3] = static_cast<T>(0);
//	val[3][0] = static_cast<T>(0); val[3][1] = static_cast<T>(0); val[3][2] = static_cast<T>(0); val[3][3] = static_cast<T>(0);
//}
//
//
//template <typename T>
//inline Matrix4<T>::Matrix4(
//	T x00, T x01, T x02, T x03,
//	T x10, T x11, T x12, T x13,
//	T x20, T x21, T x22, T x23,
//	T x30, T x31, T x32, T x33)
//{
//	if (typeid(T) == typeid(char) || typeid(T) == typeid(bool) || typeid(T) == typeid(void))
//	{
//		DEBUG_PRINT("WARNING: Create matrix4 class with type '%s' is not allow! \n", typeid(T).name());
//	}
//
//	val[0][0] = x00; val[0][1] = x01; val[0][2] = x02; val[0][3] = x03;
//	val[1][0] = x10; val[1][1] = x11; val[1][2] = x12; val[1][3] = x13;
//	val[2][0] = x20; val[2][1] = x21; val[2][2] = x22; val[2][3] = x23;
//	val[3][0] = x30; val[3][1] = x31; val[3][2] = x32; val[3][3] = x33;
//}
//
//
//template <typename T>
//inline Matrix4<T>::Matrix4(const Matrix4<T>& other)
//{
//	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2]; val[0][3] = other[0][3];
//	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2]; val[1][3] = other[1][3];
//	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2]; val[2][3] = other[2][3];
//	val[3][0] = other[3][0]; val[3][1] = other[3][1]; val[3][2] = other[3][2]; val[3][3] = other[3][3];
//}


//template <typename T>
//T Matrix4<T>::Det(int row, int col) const
//{
//	int x = 0, y = 0;
//	T subMatrix[3][3];
//
//	/* Extract the 3x3 sub-matrix that is going to calculate the determinant */
//	for (int i = 0; i < 4; i++)
//	{
//		if (i == row) { continue; }
//		for (int j = 0; j < 4; j++)
//		{
//			if (j == col) { continue; }
//
//			subMatrix[x][y] = val[i][j];
//			y++;
//		}
//		y = 0;
//		x++;
//	}
//
//	/* Calcualte the determinant based on the following formula:
//	 * det = M11(M22M33 - M23M32) - M12(M21M33 - M23M31) + M13(M21M32 - M22M31) */
//	T res = 0;
//	for (int i = 0; i < 3; i++)
//	{
//		res += subMatrix[0][i] * (subMatrix[1][(i + 1) % 3] * subMatrix[2][(i + 2) % 3] - subMatrix[1][(i + 2) % 3] * subMatrix[2][(i + 1) % 3]);
//	}
//
//	return res;
//}
//
//
//template <typename T>
//void Matrix4<T>::Invert(void)
//{
//	/* Inverse of matrix M equals to Adj(M)/Det(M).
//	 * Calcualte the determinant of this instance, using first row */
//	T det = val[0][0] * Det(0, 0) - val[0][1] * Det(0, 1) +
//		val[0][2] * Det(0, 2) - val[0][3] * Det(0, 3);
//
//	/* Calculate the cofactors matrix of this instance first, the value at (i,j)
//	 * equals to Det(i,j) */
//	Matrix4<T> adjugate;
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			adjugate[i][j] = static_cast<T>(pow(-1, i + j)) * Det(i, j);
//		}
//	}
//
//	/* Calculate the adjugate matrix by transposing the cofactors matrix */
//	adjugate.Transpose();
//
//	/* Update the elements of this instance to make it inversed */
//	for (int i = 0; i < 4; i++)
//	{
//		for (int j = 0; j < 4; j++)
//		{
//			val[i][j] = adjugate[i][j] / det;
//		}
//	}
//}
//
//
//template <typename T>
//template <typename U>
//inline Matrix4<U> Matrix4<T>::GetInverse() const
//{
//	Matrix4<U> res = this->ConvertTo<U>();
//	res.Invert();
//	return res;
//}


//template <typename T>
//inline void Matrix4<T>::Transpose(void)
//{
//	std::swap(val[0][1], val[1][0]); std::swap(val[0][2], val[2][0]);
//	std::swap(val[0][3], val[3][0]); std::swap(val[1][2], val[2][1]);
//	std::swap(val[1][3], val[3][1]); std::swap(val[2][3], val[3][2]);
//
//	//T t01 = val[0][1], t02 = val[0][2], t03 = val[0][3],
//	//  t12 = val[1][2], t13 = val[1][3], t23 = val[2][3];
//	//val[0][1] = val[1][0]; val[0][2] = val[2][0]; val[0][3] = val[3][0];
//	//val[1][2] = val[2][1]; val[1][3] = val[3][1]; val[2][3] = val[3][2];
//	//val[1][0] = t01, val[2][0] = t02, val[3][0] = t03,
//	//val[2][1] = t12, val[3][1] = t13, val[3][2] = t23;
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::GetTranspose(void) const
//{
//	return Matrix4<T>(
//		val[0][0], val[1][0], val[2][0], val[3][0],
//		val[0][1], val[1][1], val[2][1], val[3][1],
//		val[0][2], val[1][2], val[2][2], val[3][2],
//		val[0][3], val[1][3], val[2][3], val[3][3]);
//}


//template <typename T>
//template <typename U>
//inline Matrix4<U> Matrix4<T>::ConvertTo() const
//{
//	return Matrix4<U>(
//		static_cast<U>(val[0][0]), static_cast<U>(val[0][1]), static_cast<U>(val[0][2]), static_cast<U>(val[0][3]),
//		static_cast<U>(val[1][0]), static_cast<U>(val[1][1]), static_cast<U>(val[1][2]), static_cast<U>(val[1][3]),
//		static_cast<U>(val[2][0]), static_cast<U>(val[2][1]), static_cast<U>(val[2][2]), static_cast<U>(val[2][3]),
//		static_cast<U>(val[3][0]), static_cast<U>(val[3][1]), static_cast<U>(val[3][2]), static_cast<U>(val[3][3]));
//}
//
//
//template <typename T>
//inline T* Matrix4<T>::operator[] (int row)
//{
//	return val[row];
//}
//
//
//template <typename T>
//inline const T* Matrix4<T>::operator[] (int row) const
//{
//	return val[row];
//}


//template <typename T>
//inline Matrix4<T> Matrix4<T>::operator+ (const Matrix4<T>& other) const
//{
//	return Matrix4<T>(
//		val[0][0] + other[0][0], val[0][1] + other[0][1], val[0][2] + other[0][2], val[0][3] + other[0][3],
//		val[1][0] + other[1][0], val[1][1] + other[1][1], val[1][2] + other[1][2], val[1][3] + other[1][3],
//		val[2][0] + other[2][0], val[2][1] + other[2][1], val[2][2] + other[2][2], val[2][3] + other[2][3],
//		val[3][0] + other[3][0], val[3][1] + other[3][1], val[3][2] + other[3][2], val[3][3] + other[3][3]);
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::operator- (const Matrix4<T>& other) const
//{
//	return Matrix4<T>(
//		val[0][0] - other[0][0], val[0][1] - other[0][1], val[0][2] - other[0][2], val[0][3] - other[0][3],
//		val[1][0] - other[1][0], val[1][1] - other[1][1], val[1][2] - other[1][2], val[1][3] - other[1][3],
//		val[2][0] - other[2][0], val[2][1] - other[2][1], val[2][2] - other[2][2], val[2][3] - other[2][3],
//		val[3][0] - other[3][0], val[3][1] - other[3][1], val[3][2] - other[3][2], val[3][3] - other[3][3]);
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::operator* (T num) const
//{
//	return Matrix4<T>(
//		val[0][0] * num, val[0][1] * num, val[0][2] * num, val[0][3] * num,
//		val[1][0] * num, val[1][1] * num, val[1][2] * num, val[1][3] * num,
//		val[2][0] * num, val[2][1] * num, val[2][2] * num, val[2][3] * num,
//		val[3][0] * num, val[3][1] * num, val[3][2] * num, val[3][3] * num);
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::operator/ (T num) const
//{
//	return Matrix4<T>(
//		val[0][0] / num, val[0][1] / num, val[0][2] / num, val[0][3] / num,
//		val[1][0] / num, val[1][1] / num, val[1][2] / num, val[1][3] / num,
//		val[2][0] / num, val[2][1] / num, val[2][2] / num, val[2][3] / num,
//		val[3][0] / num, val[3][1] / num, val[3][2] / num, val[3][3] / num);
//}


//template <typename T>
//inline Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& other)
//{
//	val[0][0] = other[0][0]; val[0][1] = other[0][1]; val[0][2] = other[0][2]; val[0][3] = other[0][3];
//	val[1][0] = other[1][0]; val[1][1] = other[1][1]; val[1][2] = other[1][2]; val[1][3] = other[1][3];
//	val[2][0] = other[2][0]; val[2][1] = other[2][1]; val[2][2] = other[2][2]; val[2][3] = other[2][3];
//	val[3][0] = other[3][0]; val[3][1] = other[3][1]; val[3][2] = other[3][2]; val[3][3] = other[3][3];
//	return *this;
//}
//
//
//template <typename T>
//Matrix4<T> Matrix4<T>::operator* (const Matrix4<T>& other) const
//{
//	Matrix4<T> res = Matrix4<T>();
//
//	/* Iterate all rows in matrix A */
//	for (int rowA = 0; rowA < 4; rowA++)
//	{
//		/* Iterate all columns in matrix B */
//		for (int colB = 0; colB < 4; colB++)
//		{
//			T sum = 0;
//			/* Iterator that repeat 4 times */
//			for (int i = 0; i < 4; i++)
//			{
//				sum += val[rowA][i] * other[i][colB];
//			}
//			res[rowA][colB] = sum;
//		}
//	}
//	return res;
//}


//template <typename T>
//bool Matrix4<T>::operator== (const Matrix4<T>& other) const
//{
//	for (int row = 0; row < 4; row++)
//	{
//		for (int col = 0; col < 4; col++)
//		{
//			if (AreEqual(static_cast<float>(val[row][col]), static_cast<float>(other[row][col])) == false)
//				return false;
//		}
//	}
//	return true;
//}


//template <typename T>
//inline Matrix4<T> Matrix4<T>::GetInverseRotTrans() const
//{
//	/* Constructs a new Matrix4 instance where the top-left 3x3 portion contains the transpose of the original matrix's 
//	 * rotation component. The elements in the 4th column are the negated dot products of each column of the rotation 
//	 * component with the translation component. The elements in the bottom row are set to 0, except for the last element, 
//	 * which is set to 1 to maintain the homogeneous coordinate 
//	 * By obtaining the inverse of a transformation matrix, users can use it to transform points in the opposite direction, 
//	 * enabling operations such as un-rotating or un-translating coordinates.*/
//	return Matrix4<T>(
//		val[0][0], val[1][0], val[2][0], -((val[0][0] * val[0][3]) + (val[1][0] * val[1][3]) + (val[2][0] * val[2][3])),
//		val[0][1], val[1][1], val[2][1], -((val[0][1] * val[0][3]) + (val[1][1] * val[1][3]) + (val[2][1] * val[2][3])),
//		val[0][2], val[1][2], val[2][2], -((val[0][2] * val[0][3]) + (val[1][2] * val[1][3]) + (val[2][2] * val[2][3])),
//		0.0f, 0.0f, 0.0f, 1.0f);
//}
//
//
//template <typename T>
//inline Vector3<T> Matrix4<T>::TransformPoint(const Vector3<T>& point) const
//{
//	/* Convert the point's coordinates to homogeneous coordinate */
//	Vector4<T> Point = *this * Vector4<T>(point[0], point[1], point[2], 1.0f);
//
//	/* The 4th element of the resulting transformed point is the homogeneous coordinate. 
//	 * The variable "invHomo" stores the inverse of the homogeneous coordinate. The 
//	 * purpose of dividing the transformed point by "invHomo" is to normalize the coordinates
//	 * This normalization step ensures that the transformed point retains its relative 
//	 * position in 3D space. It effectively brings the point back from homogeneous coordinates 
//	 * to Cartesian coordinates. */
//	float invHomo = 1.0f / Point[3];
//	return Vector3<T>(Point[0] * invHomo, Point[1] * invHomo, Point[2] * invHomo);
//}


//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateIdentity(void)
//{
//	Matrix4<double> res = Matrix4<double>(
//		1, 0, 0, 0,
//		0, 1, 0, 0,
//		0, 0, 1, 0,
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateXRotation(double rad)
//{
//	double cos_ = cos(rad);
//	double sin_ = sin(rad);
//
//	Matrix4<double> res = Matrix4<double>(
//		1, 0, 0, 0,
//		0, cos_, -sin_, 0,
//		0, sin_, cos_, 0,
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateYRotation(double rad)
//{
//	/* Note that the sin() in the first row is positive and
//	 * sin() in the third row is negative. This is because
//	 * we are using right-hand coordinate.
//	 */
//	double cos_ = cos(rad);
//	double sin_ = sin(rad);
//
//	Matrix4<double> res = Matrix4<double>(
//		cos_, 0, sin_, 0,
//		0, 1, 0, 0,
//		-sin_, 0, cos_, 0,
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateZRotation(double rad)
//{
//	double cos_ = cos(rad);
//	double sin_ = sin(rad);
//
//	Matrix4<double> res = Matrix4<double>(
//		cos_, -sin_, 0, 0,
//		sin_, cos_, 0, 0,
//		0, 0, 1, 0,
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}


//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateTranslation(const Vector3<T>& vec)
//{
//	Matrix4<double> res = Matrix4<double>(
//		1, 0, 0, vec[0],
//		0, 1, 0, vec[1],
//		0, 0, 1, vec[2],
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateTranslation(T transX, T transY, T transZ)
//{
//	Matrix4<double> res = Matrix4<double>(
//		1, 0, 0, transX,
//		0, 1, 0, transY,
//		0, 0, 1, transZ,
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateScale(const Vector3<T>& vec)
//{
//	Matrix4<double> res = Matrix4<double>(
//		vec[0], 0, 0, 0,
//		0, vec[1], 0, 0,
//		0, 0, vec[2], 0,
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}
//
//
//template <typename T>
//inline Matrix4<T> Matrix4<T>::CreateScale(T scaleX, T scaleY, T scaleZ)
//{
//	Matrix4<double> res = Matrix4<double>(
//		scaleX, 0, 0, 0,
//		0, scaleY, 0, 0,
//		0, 0, scaleZ, 0,
//		0, 0, 0, 1);
//	return res.ConvertTo<T>();
//}


#pragma endregion



//}