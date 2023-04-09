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


const Vector5<double> Vector5<double>::Up(1, 2, 3, 4);
const Vector5<int> Vector5<int>::Down(5, 6, 7, 8);

#pragma endregion


