#pragma once
#pragma once

using namespace Engine;


#pragma region ReferenceCount

inline ReferenceCount::ReferenceCount(unsigned long smartRef, unsigned long weakRef) :
	smartRefCount(smartRef),
	weakRefCount(weakRef)
{ }

inline ReferenceCount::~ReferenceCount()
{ }

#pragma endregion


#pragma region GameObject

inline GameObject::GameObject() :
	name(""),
	position(Vector2::Zero)
{ }

inline GameObject::GameObject(const string& name, const Vector2& position) :
	name(name),
	position(position)
{ }

inline GameObject::~GameObject()
{ }

inline SmartPtr<GameObject> GameObject::Create(const string& name, const Vector2& position)
{
	return SmartPtr<GameObject>(new GameObject(name, position));
}

#pragma endregion
