#pragma once

#pragma region PhysicObject

inline PhysicObject::PhysicObject() :
	object(nullptr),
	mass(0.0),
	velocity(Vector2::Zero),
	force(Vector2::Zero)
{ }

inline PhysicObject::PhysicObject(WeakPtr<GameObject> object, double mass, Vector2 velocity, Vector2 force) :
	object(object),
	mass(mass),
	velocity(velocity),
	force(force)
{ }

inline PhysicObject::~PhysicObject()
{ }

#pragma endregion



#pragma region Physic

inline Physic::Physic() { }

inline Physic::~Physic() { }

#pragma endregion
