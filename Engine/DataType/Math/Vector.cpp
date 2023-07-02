#include "Vector.h"

namespace Engine
{

#pragma region Vector2

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

}//Namespace Engine


