#pragma once
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Mathf.h"
#include "Debugger.h"

#define M_PI       3.14159265358979323846

template<typename T>
class AABB
{
public:
	Engine::Vector3<T> center;
	Engine::Vector3<T> extent;
};


struct Object
{
	Engine::Vector3<float>	m_Position;
	Engine::Vector3<float>	m_Velocity;
	Engine::Matrix3<float>	m_Orientation;

	AABB<float>					m_BB;
};

struct CollisionPair
{
	Object* m_pObjects[2];
	float			m_CollisionTime;
};


float DegreesToRadians(float i_Degrees);


//float RandInRange(float i_Min, float i_Max);


Engine::Vector3<float> GenerateRandomVector(const Engine::Vector3<float>& i_Min, const Engine::Vector3<float>& i_Max);


bool DetectCrossTimes(float i_Center, float i_Extent, float i_Point, float i_Travel, float& o_tEnter, float& o_tExit);


bool CheckSeparationAlongAxis(const Engine::Vector3<float>& i_CollisionAxis,
	const Object& i_Obj1, const Engine::Vector3<float>& i_Obj1BBCenterInWorld,
	const Object& i_Obj2, const Engine::Vector3<float>& i_Obj2BBCenterInWorld, float i_dt,
	float& i_tEnter, float& i_tExit);


bool CheckCollision(const Object& i_Obj1, const Object& i_Obj2, float i_dt, float& o_tCollision);


CollisionPair FindCollision(float i_dt);


void SingleCollisionCheck();


void CollisionPoolCheck();

