#include "Engine.h"

namespace Engine
{

GameEngine::GameEngine() :
	physicManager(Physic::Instance()), 
	timeManager(Time::Instance()),
	inputManager(InputManager::Instance()),
	quitRequest(false)
{ }

GameEngine::~GameEngine()
{ }

void GameEngine::Destroy()
{
	timeManager->Destory();
	physicManager->Destory();
	inputManager->Destory();
	gameObjectList.~vector();
}

void GameEngine::Update(HWND hwnd)
{
	physicManager->Update();
	inputManager->Update();
	timeManager->UpdateDeltaTime();

	Service(hwnd);




	if (inputManager->GetKeyDown(KeyCode::A))
		DEBUG_PRINT("Get key down! \n");
	if (inputManager->GetKeyUp(KeyCode::A))
		DEBUG_PRINT("Get key up! \n");
	if (inputManager->GetKey(KeyCode::A))
		DEBUG_PRINT("Get key! \n");

	SendMessage(hwnd, WM_SETTEXT, NULL, NULL);
}

void GameEngine::Service(HWND hwnd)
{
	quitRequest = false;
	BOOL gotMsg = FALSE;

	do
	{
		MSG msg;
		msg.message = WM_NULL;

		gotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);

		if (gotMsg)
		{
			if (TranslateAccelerator(hwnd, NULL, &msg) == 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			switch (msg.message)
			{
			case WM_QUIT:
			{
				quitRequest = true;
				break;
			}
			case WM_INPUT:
			{
				inputManager->Service(HRAWINPUT(msg.lParam));
				break;
			}

			default:
				//DEBUG_PRINT( "message: 0x%04x\n", msg.message );
				break;
			}
		}

	} while ((gotMsg == TRUE) && (quitRequest == false));
}



void GameEngine::CreatGameObject(string name,
								 float mass,
								 const Vector2& posistion,
								 const Vector2& velocity,
								 const Vector2& force)
	
{
	SmartPtr<GameObject> obj = GameObject::Create(name, posistion);
	gameObjectList.push_back(obj);
	physicManager->AddPhysicObj(obj, mass, velocity, force);
}



#pragma region Console App Code

void GameEngine::GameLoop()
{
	/* Debug start */
	CreatGameObject("Mo", 1.0, Vector2(10, 10), Vector2::Zero, Vector2::Zero);
	/* Debug end */

	int i = 0;
	while (i < 3)
	{
		timeManager->UpdateDeltaTime();
		physicManager->Update();

		/* Debug start */
		SmartPtr<GameObject> obj = gameObjectList[0];
		PhysicObject* physicObj = physicManager->FindPhysicObj(obj);
		assert(physicObj != nullptr);

		Vector2 gravity = Vector2(0.0, -1.0);
		float dragFactor = 0.1f;
		double dragDirX = physicObj->velocity.x == 0 ? 0 : -1 * physicObj->velocity.x / abs(physicObj->velocity.x);
		double dragDirY = physicObj->velocity.y == 0 ? 0 : -1 * physicObj->velocity.y / abs(physicObj->velocity.y);
		Vector2 dragForce = dragFactor * physicObj->velocity * physicObj->velocity * Vector2(dragDirX, dragDirY);
		Vector2 totalForce = gravity + dragForce;

		printf("/////Debug///// \n");
		printf("DeltaTime: %lf \n", Time::DeltaTime);
		printf("GameObject: Pos: (%f, %f), Vel: (%f, %f) \n", obj->position.x, obj->position.y, physicObj->velocity.x, physicObj->velocity.y);
		printf("DragForce: (%f, %f), TotalForct (%f, %f) \n", dragForce.x, dragForce.y, totalForce.x, totalForce.y);
		Sleep(500);

		/* Debug end */

		i++;
	}
}
#pragma endregion


}
