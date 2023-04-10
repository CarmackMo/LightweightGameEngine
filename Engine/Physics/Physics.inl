#pragma once

#pragma region PhysicObject

inline PhysicObject::PhysicObject() :
	object(nullptr),
	mass(0.0),
	velocity(Vector2<double>::Zero),
	force(Vector2<double>::Zero)
{ }

inline PhysicObject::PhysicObject(const PhysicObject& other) :
	object(other.object),
	mass(other.mass),
	velocity(other.velocity),
	force(other.force)
{ }

inline PhysicObject::PhysicObject(const WeakPtr<GameObject>& object,
								  double mass,
								  const Vector2<double>& velocity,
								  const Vector2<double>& force) :
	object(object),
	mass(mass),
	velocity(velocity),
	force(force)
{ }

inline PhysicObject::~PhysicObject()
{ }

inline PhysicObject& PhysicObject::operator=(const PhysicObject& other)
{
	this->object = other.object;
	this->mass = other.mass;
	this->velocity = other.velocity;
	this->force = other.force;
	return *this;
}

#pragma endregion



#pragma region Physic

inline Physic::Physic() { }

inline Physic::~Physic() 
{
	for (PhysicObject* obj : physicObjList)
	{
		delete obj;
	}
}

inline void Physic::AddPhysicObj(const SmartPtr<GameObject>& object,
								 float mass,
								 const Vector2<double>& velocity,
								 const Vector2<double>& force)

{
	PhysicObject* obj = new PhysicObject(WeakPtr<GameObject>(object), mass, velocity, force);
	physicObjList.push_back(obj);
}


#pragma endregion
