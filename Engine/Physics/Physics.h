#pragma once
#include "Timer.h"
#include "Engines/GameObject.h"
#include "Utility/SmartPtrs.h"


namespace Engine
{
namespace Physics
{

class PhysicObject
{
public:
	WeakPtr<GameObject> object;
	double				mass;
	Vector2<double>		velocity;
	Vector2<double>		force;

	inline PhysicObject() :
		object(),
		mass(0.0),
		velocity(Vector2<double>::Zero),
		force(Vector2<double>::Zero)
	{ }
	inline PhysicObject(const PhysicObject& other) :
		object(other.object),
		mass(other.mass),
		velocity(other.velocity),
		force(other.force)
	{ }
	inline PhysicObject(const WeakPtr<GameObject>& object, 
						double mass = 0.0, 
						const Vector2<double>& velocity = Vector2<double>::Zero,
						const Vector2<double>& force = Vector2<double>::Zero) :
		object(object),
		mass(mass),
		velocity(velocity),
		force(force)
	{ }
	
	inline ~PhysicObject() {}

	inline PhysicObject& operator=(const PhysicObject& other)
	{
		this->object = other.object;
		this->mass = other.mass;
		this->velocity = other.velocity;
		this->force = other.force;
		return *this;
	}
};


class Physic : public Singleton<Physic>
{
private:
	inline Physic() = default;
	
	inline ~Physic()
	{
		for (PhysicObject* obj : physicObjList)
		{
			delete obj;
		}
	}

public:
	friend class Singleton<Physic>;

	std::vector<PhysicObject*> physicObjList;

	/**
	 * @brief Update the physical information for all regiseted gameobjects (e.g. force, speed...)
	 *		  This function will be called every frame.
	 * @remarks We need to manually delete the physical objects whose original game object is 
	 *			deleted. For those objects, we will put them to the front of the object list
	 *			in each update using Fast-Slow Pointer algorithm and record how many objects
	 *			need to be erase. After erasing, shrink the object list.
	 */
	void Update();

	/**
	 * @param target:	target game object
	 * @return return a null pointer if physic object not found
	 */
	PhysicObject* FindPhysicObj(const SmartPtr<GameObject>& target);
	
	/**
	 * @param object: target object to add force
	 * @param force:  force
	 * @return return false if target game object is not found
	 */
	bool AddForceToObj(const SmartPtr<GameObject>& object, const Vector2<double>& force);


	inline void AddPhysicObj(
			const SmartPtr<GameObject>& object,
			float mass = 0.0,
			const Vector2<double>& velocity = Vector2<double>::Zero,
			const Vector2<double>& force = Vector2<double>::Zero)
	{
		PhysicObject* obj = new PhysicObject(WeakPtr<GameObject>(object), mass, velocity, force);
		physicObjList.push_back(obj);
	}


	//void RemovePhysicObj(WeakPtr<GameObject> object);
};

}
}