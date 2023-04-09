#include "Vector.h"

using namespace Engine;

#pragma region Vector2

const Vector2 Vector2::Zero(0, 0);
const Vector2 Vector2::Left(-1, 0);
const Vector2 Vector2::Right(1, 0);
const Vector2 Vector2::Up(0, 1);
const Vector2 Vector2::Down(0, -1);

#pragma endregion



#pragma region Vector3

template <typename T> const Vector3<T> Vector3<T>::Zero(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
template <typename T> const Vector3<T> Vector3<T>::Left(static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0));
template <typename T> const Vector3<T> Vector3<T>::Right(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
template <typename T> const Vector3<T> Vector3<T>::Up(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
template <typename T> const Vector3<T> Vector3<T>::Down(static_cast<T>(0), static_cast<T>(-1), static_cast<T>(0));
template <typename T> const Vector3<T> Vector3<T>::Front(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
template <typename T> const Vector3<T> Vector3<T>::Back(static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1));

#pragma endregion



#pragma region Vector4





const Vector5<double> Vector5<double>::Up(1, 2, 3, 4);
const Vector5<int> Vector5<int>::Down(5, 6, 7, 8);

#pragma endregion


