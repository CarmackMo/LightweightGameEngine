#pragma once
#include "./Math/Vector.h"
#include "./Utility/SmartPtrs.h"


namespace Engine
{

class GameObject
{
private:

public:
	std::string		name;
	Vector2<double> position;

	inline GameObject() :
		name(""),
		position(Vector2<double>::Zero)
	{ }
	inline GameObject(const std::string& name, const Vector2<double>& position) :
		name(name),
		position(position)
	{ }	
	inline ~GameObject()
	{ }

	inline static SmartPtr<GameObject> Create(const std::string& name, const Vector2<double>& position)
	{
		return SmartPtr<GameObject>(new GameObject(name, position));
	}
};

}//Namespace Engine
