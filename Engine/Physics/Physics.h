#pragma once

#include <vector>
#include <algorithm>
#include "DataType.h"

using namespace std;


namespace Engine
{
namespace Physics
{




class PhysicObject
{
public:
	WeakPtr<GameObject> object;
	double mass;
	Vector2 velocity;
	Vector2 force;

	inline PhysicObject();
	inline PhysicObject(WeakPtr<GameObject> object, double mass = 0.0, Vector2 velocity = Vector2::Zero, Vector2 force = Vector2::Zero);
	inline ~PhysicObject();
};


class Physic
{
public:
	vector<PhysicObject> physicObjList;

	inline Physic();
	inline ~Physic();

	void Update();
	void Update(GameObject &object, const Vector2 &force, double time);
	void AddPhysicObj(SmartPtr<GameObject> object, Vector2 velocity);
	void RemovePhysicObj(WeakPtr<GameObject> object);
};


#include "Physics.inl"

}
}