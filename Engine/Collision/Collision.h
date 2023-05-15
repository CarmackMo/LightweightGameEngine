#pragma once
#include "Dependency.h"
#include "DataType.h"
#include "Mathf.h"

using namespace Engine;

#define M_PI       3.14159265358979323846



template<typename T>
class AABB
{
public:
	Vector3<T> center;
	Vector3<T> extent;
};


struct Object
{
	Vector3<float>	m_Position;
	Vector3<float>	m_Velocity;
	Matrix3<float>	m_Orientation;

	AABB<float>					m_BB;
};

struct CollisionPair
{
	Object* m_pObjects[2];
	float			m_CollisionTime;
};


float DegreesToRadians(float i_Degrees);


float RandInRange(float i_Min, float i_Max);


Vector3<float> GenerateRandomVector(const Vector3<float>& i_Min, const Vector3<float>& i_Max);


bool DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float& o_tEnter, float& o_tExit);


bool CheckSeparationAlongAxis(const Vector3<float>& i_CollisionAxis,
	const Object& i_Obj1, const Vector3<float>& i_Obj1BBCenterInWorld,
	const Object& i_Obj2, const Vector3<float>& i_Obj2BBCenterInWorld, float i_dt,
	float& i_tEnter, float& i_tExit);


bool CheckCollision(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& o_tCollision);


CollisionPair FindCollision(float i_dt);


void SingleCollisionCheck();


void CollisionPoolCheck();

