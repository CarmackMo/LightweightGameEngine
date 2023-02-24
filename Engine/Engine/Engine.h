#pragma once
#include <vector>
#include "Time.h"
#include "Physics.h"
#include "DataType.h"

using namespace std;
using namespace Engine::Time;
using namespace Engine::Physics;

namespace Engine
{


class GameEngine
{
public:
	vector<GameObject> gameObjectList;
	Physic physicManager;
	Timer timeManager;

	/* Debug start */
	bool isKeyDown = false;
	/* Debug end */

	GameEngine();
	~GameEngine();

	void Update(HWND hwnd);
	void GameLoop();

};



}
