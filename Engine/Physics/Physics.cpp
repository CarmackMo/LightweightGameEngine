#include "Physics.h"

namespace Engine
{
namespace Physics
{

using std::vector;
using namespace Timer;


void Physic::Update()
{
	int removeCount = 0;
	vector<PhysicObject*>::iterator fast = physicObjList.begin();
	vector<PhysicObject*>::iterator slow = physicObjList.begin();

	for (PhysicObject* physicObj : physicObjList)
	{
		if (physicObj->object != nullptr)
		{
			SmartPtr<GameObject> obj = SmartPtr<GameObject>(physicObj->object);

			/* Numerical Integration Approach */
			Vector2<double> acceleration = physicObj->force / physicObj->mass;
			Vector2<double> newVelocity = physicObj->velocity + acceleration * Time::DeltaTime;
			Vector2<double> deltaVelocity = newVelocity - physicObj->velocity;

			obj->position = obj->position + (physicObj->velocity + newVelocity) / 2.0 * Time::DeltaTime;
			physicObj->velocity = newVelocity;



			/* TODO: Temperary Drag Force Logic */
			
			double dragFactor = 1000;
			double dragDirX = physicObj->velocity[0] == 0 ? 0 : -1 * physicObj->velocity[0] / abs(physicObj->velocity[0]);
			double dragDirY = physicObj->velocity[1] == 0 ? 0 : -1 * physicObj->velocity[1] / abs(physicObj->velocity[1]);

			Vector2<double> deltaDragForce = (dragFactor * deltaVelocity * deltaVelocity) * Vector2<double>(dragDirX, dragDirY);
			physicObj->force += deltaDragForce;

			//Vector2 dragForce = (dragFactor * physicObj->velocity * physicObj->velocity) * Vector2(dragDirX, dragDirY);		// drag force magnitude * drag force direction  
			//physicObj->force += dragForce;
			/* TODO: Temperary Drag Force Logic */

		}
		else 
		{
			iter_swap(fast, slow);
			removeCount++;
			slow++;
		}

		fast++;
	}

	/* Erase invalid physical objects together and shrink the object list */
	for (int i = 0; i < removeCount; i++)
	{
		delete physicObjList.front();
		physicObjList.erase(physicObjList.begin());
	}
	physicObjList.shrink_to_fit();
}


PhysicObject* Physic::FindPhysicObj(const SmartPtr<GameObject>& target)
{
	WeakPtr<GameObject> obj = WeakPtr<GameObject>(target);
	for (PhysicObject* item : physicObjList)
	{
		if (item->object == obj)
		{
			return item;
		}
	}

	return nullptr;
}


bool Physic::AddForceToObj(const SmartPtr<GameObject>& object, const Vector2<double>& force)
{
	PhysicObject* physicObj = FindPhysicObj(object);

	if (physicObj != nullptr)
	{
		physicObj->force += force;
		return true;
	}
	else
		return false;
}


}//Namespace Physics
}//Namespace Engine


