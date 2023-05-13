#pragma once
#include <./Math/Vector.h>
#include <Util.h>


namespace Engine
{

class GameObject
{
private:

public:
	string name;
	Vector2<double> position;

	inline GameObject();
	inline GameObject(const string& name, const Vector2<double>& position = Vector2<double>::Zero);
	inline ~GameObject();

	inline static SmartPtr<GameObject> Create(const string& name, const Vector2<double>& position = Vector2<double>::Zero);
};

}
