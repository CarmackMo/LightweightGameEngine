#include "Physics.h"

using namespace std;
using namespace Engine;
using namespace Engine::Physics;


void Physic::Update()
{
	for (PhysicObject item : physicObjList)
	{
		SmartPtr<GameObject> obj = SmartPtr<GameObject>(item.object);

		if (obj != nullptr)
		{
			Vector2 acceleration = item.force / item.mass;
			Vector2 newVelocity = item.velocity + acceleration * tim;


			// TODO: do physic
		}
		else
		{
			// TODO: release the instance
		}
	}
}


void Physic::Update(GameObject &object, const Vector2 &force, double time)
{
	/* Numerical Integration Approach */
	Vector2 acceleration = force / object.mass;
	Vector2 newVelocity = object.velocity + acceleration * time;

	object.position = object.position + (object.velocity + newVelocity) / 2.0 * time;
	object.velocity = newVelocity;
}