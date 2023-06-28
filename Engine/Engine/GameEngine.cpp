#include "GameEngine.h"

namespace Engine
{

using std::string;
using std::ifstream;
using std::ios;
using namespace Physics;
using namespace Timer;
using namespace Input;


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
								 const Vector2<double>& position,
								 const Vector2<double>& velocity,
								 const Vector2<double>& force)
	
{
	SmartPtr<GameObject> obj = GameObject::Create(name, position);
	gameObjectList.push_back(obj);
	physicManager->AddPhysicObj(obj, mass, velocity, force);
}


void GameEngine::CreateGameObject(string jsonPath)
{
	// Using fstream to get the file pointer in "file"
	ifstream file(jsonPath, ios::in);
	Json::Value root;
	Json::Reader reader;
	 
	if (file.fail())
	{
		DEBUG_PRINT("Read file fail!! \n ");
	}
	else
	{
		// Using reader we are parsing the JSON
		reader.parse(file, root);

		for (Json::Value::ArrayIndex i = 0; i != root.size(); i++)
		{
			Json::Value value = root[i];

			// Data placeholder
			string name;
			float mass;
			Vector2<double> position, velocity, force;

			// Parse data to placeholder
			name = value["Name"].asString();
			mass = value["Mass"].asFloat();

			position[0] = value["Position"][0].asDouble();
			position[1] = value["Position"][1].asDouble();

			velocity[0] = value["Velocity"][0].asDouble();
			velocity[1] = value["Velocity"][1].asDouble();

			force[0] = value["Force"][0].asDouble();
			force[1] = value["Force"][1].asDouble();

			//Create gameobject
			SmartPtr<GameObject> obj = GameObject::Create(name, position);
			gameObjectList.push_back(obj);
			physicManager->AddPhysicObj(obj, mass, velocity, force);

		}




		
	}
}



#pragma region Console App Code

void GameEngine::GameLoop()
{
	/* Debug start */
	CreatGameObject("Mo", 1.0, Vector2<double>(10, 10), Vector2<double>::Zero, Vector2<double>::Zero);
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

		Vector2<double> gravity = Vector2<double>(0.0, -1.0);
		double dragFactor = 0.1;
		double dragDirX = physicObj->velocity[0] == 0 ? 0 : -1 * physicObj->velocity[0] / abs(physicObj->velocity[0]);
		double dragDirY = physicObj->velocity[1] == 0 ? 0 : -1 * physicObj->velocity[1] / abs(physicObj->velocity[1]);
		Vector2<double> dragForce = dragFactor * physicObj->velocity * physicObj->velocity * Vector2<double>(dragDirX, dragDirY);
		Vector2<double> totalForce = gravity + dragForce;

		printf("/////Debug///// \n");
		printf("DeltaTime: %lf \n", Time::DeltaTime);
		printf("GameObject: Pos: (%f, %f), Vel: (%f, %f) \n", obj->position[0], obj->position[1], physicObj->velocity[0], physicObj->velocity[1]);
		printf("DragForce: (%f, %f), TotalForct (%f, %f) \n", dragForce[0], dragForce[1], totalForce[0], totalForce[1]);
		Sleep(500);

		/* Debug end */

		i++;
	}
}
#pragma endregion


}//Namespace Engine
