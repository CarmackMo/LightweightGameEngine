#pragma once
#include <Windows.h>

#include "Engines/GameObject.h"
#include "Utility/SmartPtrs.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

#include "Debugger.h"

#include "Input.h"
#include "Timer.h"
#include "Physics.h"
#include "FileSys.h"
#include "JobSys.h"
#include "Collision.h"

namespace Engine
{

class GameEngine
{
public:
	std::vector<SmartPtr<GameObject>>	gameObjectList;
	Physics::Physic*					physicManager;
	Timer::Time*						timeManager;
	Input::InputManager*				inputManager;

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
		std::string name,
		float mass,
		const Vector2<double>& position,
		const Vector2<double>& velocity,
		const Vector2<double>& force);

	void CreateGameObject(std::string jsonPath);


};

}//Namespace Engine