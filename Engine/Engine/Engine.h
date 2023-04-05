#pragma once
#include "Dependency.h"
#include "DataType.h"
#include "Input.h"
#include "Timer.h"
#include "Physics.h"

using namespace std;
using namespace Engine::Input;
using namespace Engine::Timer;
using namespace Engine::Physics;
using namespace Engine::Debugger;

namespace Engine
{


class GameEngine
{
public:
	vector<SmartPtr<GameObject>> gameObjectList;
	Physic* physicManager;
	Time* timeManager;
	InputManager* inputManager;

	bool quitRequest;

	/* Debug start */
	//bool isKeyDown = false;
	/* Debug end */

	GameEngine();
	~GameEngine();

	void Update(HWND hwnd);
	void Service(HWND hwnd);
	void GameLoop();
	void Destroy();

	void CreatGameObject(
		string name,
		float mass,
		const Vector2& posistion,
		const Vector2& velocity,
		const Vector2& force);


};



}