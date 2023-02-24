#include "Engine.h"

using namespace Engine::Time;
using namespace Engine::Physics;

namespace Engine
{


GameEngine::GameEngine()
{
	gameObjectList = vector<GameObject>();
	physicManager = Physic();
	timeManager = Timer();

}

GameEngine::~GameEngine()
{ }
	
void GameEngine::Update(HWND hwnd)
{
	Vector2 gravity = Vector2(0.0, -1.0);
	float dragFactor = 0.1f;

	for (GameObject& object : gameObjectList)
	{
		float dragDirX = object.velocity.x() == 0 ? 0 : -1 * object.velocity.x() / abs(object.velocity.x());
		float dragDirY = object.velocity.y() == 0 ? 0 : -1 * object.velocity.y() / abs(object.velocity.y());
		Vector2 dragForce = (dragFactor * object.velocity * object.velocity) * Vector2(dragDirX, dragDirY);		// drag force magnitude * drag force direction  
		Vector2 totalForce = gravity + dragForce;

		if (isKeyDown)
			physicManager.Update(object, totalForce, timeManager.GetDeltaTime());
		else
			physicManager.Update(object, Vector2(0,0), timeManager.GetDeltaTime());
	}

	timeManager.UpdateDeltaTime();

	SendMessage(hwnd, WM_SETTEXT, NULL, NULL);
}

void GameEngine::GameLoop()
{
	Vector2 gravity = Vector2(0.0, -1.0);
	float dragFactor = 0.1f;

	while (true)
	{
		for (GameObject& object : gameObjectList)
		{
			float dragDirX = object.velocity.x() == 0 ? 0 : -1 * object.velocity.x() / abs(object.velocity.x());
			float dragDirY = object.velocity.y() == 0 ? 0 : -1 * object.velocity.y() / abs(object.velocity.y());
			Vector2 dragForce = (dragFactor * object.velocity * object.velocity) * Vector2(dragDirX, dragDirY);		// drag force magnitude * drag force direction  
			Vector2 totalForce = gravity + dragForce;
			physicManager.Update(object, totalForce, timeManager.GetDeltaTime());
		}

		timeManager.UpdateDeltaTime();

		/* Debug start */
		GameObject object = gameObjectList[0];
		float dragDirX = object.velocity.x() == 0 ? 0 : -1 * object.velocity.x() / abs(object.velocity.x());
		float dragDirY = object.velocity.y() == 0 ? 0 : -1 * object.velocity.y() / abs(object.velocity.y());
		Vector2 dragForce = dragFactor * object.velocity * object.velocity * Vector2(dragDirX, dragDirY);
		Vector2 totalForce = gravity + dragForce;
		printf("/////Debug///// \n");
		printf("DeltaTime: %lf \n", timeManager.GetDeltaTime());
		printf("GameObject: Pos: (%f, %f), Vel: (%f, %f) \n", object.position.x(), object.position.y(), object.velocity.x(), object.velocity.y());
		printf("DragForce: (%f, %f), TotalForct (%f, %f) \n", dragForce.x(), dragForce.y(), totalForce.x(), totalForce.y());
		Sleep(500);
		/* Debug end */
	}
}



}