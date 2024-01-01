// Includes
//=========

#include <iostream>

#include <MultiplayerGame_/MultiplayerGame/cMultiplayerGame.h>

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/ConstantBufferFormats.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/sVector.h>
#include <Engine/UserInput/UserInput.h>


// TODO: Tempory code for collider testing
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/Collision.h>
#include <vector>
#include <iostream>


// TODO: Temporary code for network system
#include <Engine/Network/Network.h>



using namespace eae6320;


// Inherited Implementation
//=========================

// Run
//----

void MultiplayerGame::cMultiplayerGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}


void MultiplayerGame::cMultiplayerGame::UpdateSimulationBasedOnInput()
{
	m_camera.UpdateBasedOnInput();

	m_renderObject_triangle.UpdateBasedOnInput();


	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter))
	{
		isKeyPressed = true;
	}
	else
	{
		isKeyPressed = false;
	}


	// TODO: Temporary code for collider debug
	m_colliderObject_AABB1.UpdateBasedOnInput();
	//m_colliderObject_sphere2.UpdateBasedOnInput();

}


void MultiplayerGame::cMultiplayerGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	m_renderObject_triangle.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_renderObject_Ganyu.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_renderObject_Keqing.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);



	// TODO: Temporary code for collider debug
	m_colliderObject_AABB1.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_colliderObject_AABB2.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_colliderObject_sphere2.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	Physics::Collision::Update_CollisionDetection();

	Physics::Collision::Update_CollisionResolution();

}


void MultiplayerGame::cMultiplayerGame::SubmitDataToBeRendered(
	const float i_elapsedSecondCount_systemTime, 
	const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	// Submit camera data
	{
		// aspectRatio = width / height
		uint16_t width, height;
		GetCurrentResolution(width, height);

		Graphics::SubmitCameraMatrices(
			m_camera.CreateWorldToCameraMatrix(i_elapsedSecondCount_sinceLastSimulationUpdate),
			m_camera.CreateCameraToProjectedMatrix(static_cast<float>(width), static_cast<float>(height)));
	}

	// Submit background color
	{
		Graphics::SubmitBackgroundColor(0.5f, 0.5f, 0.5f);
	}


	// Submit mesh-effect pair data
	{
		size_t arraySize = m_renderObjectList.size();

		Graphics::ConstantBufferFormats::sNormalRender* normalRenderDataArray = new Graphics::ConstantBufferFormats::sNormalRender[arraySize];

		// Render data of render objects 
		for (size_t i = 0; i < arraySize; i++)
		{
			if (m_renderObjectList[i]->GetMesh().expired() || m_renderObjectList[i]->GetEffect().expired())
				continue;

			normalRenderDataArray[i].Initialize(
				m_renderObjectList[i]->GetMesh(), m_renderObjectList[i]->GetEffect(),
				m_renderObjectList[i]->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}

		Graphics::SubmitNormalRenderData(normalRenderDataArray, static_cast<uint32_t>(arraySize));

		// clean up 
		for (size_t i = 0; i < arraySize; i++)
		{
			normalRenderDataArray[i].CleanUp();
		}

		delete[] normalRenderDataArray;
	}


	// TODO: Submit debug for box collider
	{
		auto BVHRenderData = Physics::Collision::GetBVHRenderData();

		size_t colliderListSize = m_colliderObjectList.size();
		size_t totalArraySize = BVHRenderData.size() + colliderListSize;

		Graphics::ConstantBufferFormats::sDebugRender* debugDataArray = new Graphics::ConstantBufferFormats::sDebugRender[totalArraySize];

		// Render data of hard-coded collider
		for (size_t i = 0 ; i < m_colliderObjectList.size(); i++)
		{
			auto collider = m_colliderObjectList[i];

			if (collider->GetColliderLine().expired())
				continue;

			debugDataArray[i].Initialize(collider->GetColliderLine(), collider->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}

		// Render data of BVH tree
		int idx = 0;
		for (auto iter = BVHRenderData.begin(); iter != BVHRenderData.end(); iter++)
		{
			if (iter->first.expired())
				continue;

			debugDataArray[idx].Initialize(iter->first, iter->second);
			idx++;
		}

		Graphics::SubmitDebugRenderData(debugDataArray, static_cast<uint32_t>(totalArraySize));

		// Clean up
		for (size_t i = 0; i < totalArraySize; i++)
		{
			debugDataArray[i].CleanUp();
		}

		delete[] debugDataArray;
	}
}


// Initialize / Clean Up
//----------------------

eae6320::cResult MultiplayerGame::cMultiplayerGame::Initialize()
{
	eae6320::Logging::OutputMessage("\n --------------MoZiheng MyGame customized log messge--------------");

#if defined( EAE6320_PLATFORM_D3D )
	eae6320::Logging::OutputMessage("Solution Platform: Direct3D");
#elif defined( EAE6320_PLATFORM_GL )
	eae6320::Logging::OutputMessage("Solution Platform: OpenGL");
#endif

#ifdef _WIN64
	eae6320::Logging::OutputMessage("Machine Platform: x64");
#else
	eae6320::Logging::OutputMessage("Machine Platform: x86");
#endif

#ifdef _DEBUG
	eae6320::Logging::OutputMessage("Configuration: Debug");
#else
	eae6320::Logging::OutputMessage("Configuration: Release");
#endif

	eae6320::Logging::OutputMessage("Initialization Successfull!! \n");


	InitializeCamera();

	InitializeMeshData();

	InitializeGameObject();

	// TODO: temporary code for initialize collision system
	InitializeCollisionSystem();


	// TODO: temporary code for network system
	Network::Connect();


	return Results::Success;
}


eae6320::cResult MultiplayerGame::cMultiplayerGame::CleanUp()
{
	eae6320::Logging::OutputMessage("\n --------------MoZiheng MyGame customized log messge--------------");

#if defined( EAE6320_PLATFORM_D3D )
	eae6320::Logging::OutputMessage("Solution Platform: Direct3D");
#elif defined( EAE6320_PLATFORM_GL )
	eae6320::Logging::OutputMessage("Solution Platform: OpenGL");
#endif

#ifdef _WIN64
	eae6320::Logging::OutputMessage("Machine Platform: x64");
#else
	eae6320::Logging::OutputMessage("Machine Platform: x86");
#endif

#ifdef _DEBUG
	eae6320::Logging::OutputMessage("Configuration: Debug");
#else
	eae6320::Logging::OutputMessage("Configuration: Release");
#endif

	eae6320::Logging::OutputMessage("Clan Up Successfull!!\n");

	CleanUpGameObject();

	return Results::Success;
}


void MultiplayerGame::cMultiplayerGame::InitializeMeshData()
{
	Graphics::cEffect::Create(m_effect_animate, m_vertexShaderPath, m_fragmentShaderPath_animate);

	Graphics::cEffect::Create(m_effect_standard, m_vertexShaderPath, m_fragmentShaderPath_standard);
}


void MultiplayerGame::cMultiplayerGame::InitializeCamera()
{
	m_camera.Initialize(
		0.0f, 0.0f, 5.0f,
		Math::ConvertDegreesToRadians(45.0f), 
		0.1f, 
		100.0f);

	//m_camera.InitializeMesh(m_rectangleMeshPath);
	//m_camera.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_animate);
}


void MultiplayerGame::cMultiplayerGame::InitializeGameObject()
{
	// Game object initialization
	{
		// Tirangle
		m_renderObject_triangle.InitializeMesh(m_triangleMeshPath);
		m_renderObject_triangle.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_triangle.GetRigidBody().angularSpeed = 1.0f;
		m_renderObject_triangle.GetRigidBody().angularVelocity_axis_local = Math::sVector(0.0f, 0.0f, 1.0f);

		// Rectangle
		m_renderObject_rectangle.InitializeMesh(m_rectangleMeshPath);
		m_renderObject_rectangle.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_rectangle.GetRigidBody().orientation = Math::cQuaternion(0.5f, Math::sVector(1, 0, 1));
		m_renderObject_rectangle.GetRigidBody().position = Math::sVector(+0.0f, -1.0f, +0.5f);

		// Plane
		m_renderObject_plane.InitializeMesh(m_planeMeshPath);
		m_renderObject_plane.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_plane.GetRigidBody().position = Math::sVector(+6.0f, -5.0f, -3.0f);

		// Cube
		m_renderObject_cube.InitializeMesh(m_cubeMeshPath);
		m_renderObject_cube.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_animate);
		m_renderObject_cube.GetRigidBody().position = Math::sVector(-2.0f, -2.0f, -3.0f);

		// Keqing
		m_renderObject_Keqing.InitializeMesh(m_keqingMeshPath);
		m_renderObject_Keqing.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_Keqing.GetRigidBody().position = Math::sVector(-5.0f, -10.0f, -20.0f);

		// Keqing - skin
		m_renderObject_Keqing_skin.InitializeMesh(m_keqing_SkinMeshPath);
		m_renderObject_Keqing_skin.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_Keqing_skin.GetRigidBody().position = Math::sVector(-5.0f, -10.0f, -20.0f);

		// Ganyu
		m_renderObject_Ganyu.InitializeMesh(m_ganyuMeshPath);
		m_renderObject_Ganyu.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
		m_renderObject_Ganyu.GetRigidBody().position = Math::sVector(5.0f, -10.0f, -20.0f);



		m_renderObjectList.push_back(&m_renderObject_triangle);
		m_renderObjectList.push_back(&m_renderObject_rectangle);
		m_renderObjectList.push_back(&m_renderObject_plane);
		m_renderObjectList.push_back(&m_renderObject_cube);
		m_renderObjectList.push_back(&m_renderObject_Keqing);
		//m_renderObjectList.push_back(&m_renderObject_Keqing_skin);
		m_renderObjectList.push_back(&m_renderObject_Ganyu);
	}


	// TODO: temporary code for initialize colldier object
	{
		Physics::sColliderSetting setting_AABB1;
		setting_AABB1.SettingForAABB(Math::sVector(-0.5, -0.5, -0.5), Math::sVector(0.5, 0.5, 0.5));

		Physics::sColliderSetting setting_sphere1;
		setting_sphere1.SettingForSphere(Math::sVector(0, 0, 0), 0.5f);


		m_colliderObject_AABB1.GetRigidBody().position = Math::sVector(0.5f, 0.0f, 0.5f);
		m_colliderObject_AABB1.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB1.InitializeColliderLine();
		m_colliderObject_AABB1.GetCollider()->m_name = "AABB_1";
		m_colliderObject_AABB1.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB1.SetIsCollide(true); };
		m_colliderObject_AABB1.GetCollider()->OnCollisionStay = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : " ).c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB1.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB1.SetIsCollide(false); };

		m_colliderObject_AABB2.GetRigidBody().position = Math::sVector(-1.0f, -0.5f, 1.5f);
		m_colliderObject_AABB2.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB2.InitializeColliderLine();
		m_colliderObject_AABB2.GetCollider()->m_name = "AABB_2";
		m_colliderObject_AABB2.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB2.SetIsCollide(true); };
		m_colliderObject_AABB2.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB2.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB2.SetIsCollide(false); };

		m_colliderObject_AABB3.GetRigidBody().position = Math::sVector(1.0f, -2.0f, 3.0f);
		m_colliderObject_AABB3.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB3.InitializeColliderLine();
		m_colliderObject_AABB3.GetCollider()->m_name = "AABB_3";
		m_colliderObject_AABB3.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB3.SetIsCollide(true); };
		m_colliderObject_AABB3.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB3.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB3.SetIsCollide(false); };

		m_colliderObject_AABB4.GetRigidBody().position = Math::sVector(-1.5f, -2.0f, 1.0f);
		m_colliderObject_AABB4.GetRigidBody().isStatic = true;
		m_colliderObject_AABB4.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB4.InitializeColliderLine();
		m_colliderObject_AABB4.GetCollider()->m_name = "AABB_4";
		m_colliderObject_AABB4.GetCollider()->OnCollisionEnter =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB4.SetIsCollide(true); };
		m_colliderObject_AABB4.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB4.GetCollider()->OnCollisionExit =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB4.SetIsCollide(false); };


		m_colliderObject_sphere1.GetRigidBody().position = Math::sVector(2.0f, -1.0f, 3.0f);
		m_colliderObject_sphere1.InitializeCollider(setting_sphere1);
		m_colliderObject_sphere1.InitializeColliderLine();
		m_colliderObject_sphere1.GetCollider()->m_name = "Sphere_1";
		m_colliderObject_sphere1.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_sphere1.SetIsCollide(true); };
		m_colliderObject_sphere1.GetCollider()->OnCollisionStay = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_sphere1.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_sphere1.SetIsCollide(false); };
	
		m_colliderObject_sphere2.GetRigidBody().position = Math::sVector(2.0f, 1.5f, 1.0f);
		m_colliderObject_sphere2.InitializeCollider(setting_sphere1);
		m_colliderObject_sphere2.InitializeColliderLine();
		m_colliderObject_sphere2.GetCollider()->m_name = "Sphere_2";
		m_colliderObject_sphere2.GetCollider()->OnCollisionEnter =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_sphere2.SetIsCollide(true); };
		m_colliderObject_sphere2.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_sphere2.GetCollider()->OnCollisionExit =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_sphere2.SetIsCollide(false); };

		m_colliderObject_sphere3.GetRigidBody().position = Math::sVector(1.5f, 0.0f, 2.0f);
		m_colliderObject_sphere3.GetRigidBody().isTrigger = true;
		m_colliderObject_sphere3.InitializeCollider(setting_sphere1);
		m_colliderObject_sphere3.InitializeColliderLine();
		m_colliderObject_sphere3.GetCollider()->m_name = "Sphere_3";
		m_colliderObject_sphere3.GetCollider()->OnCollisionEnter =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_sphere3.SetIsCollide(true); };
		m_colliderObject_sphere3.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_sphere3.GetCollider()->OnCollisionExit =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_sphere3.SetIsCollide(false); };



		m_colliderObjectList.push_back(&m_colliderObject_AABB1);
		m_colliderObjectList.push_back(&m_colliderObject_AABB2);
		m_colliderObjectList.push_back(&m_colliderObject_AABB3);
		m_colliderObjectList.push_back(&m_colliderObject_AABB4);
		m_colliderObjectList.push_back(&m_colliderObject_sphere1);
		m_colliderObjectList.push_back(&m_colliderObject_sphere2);
		m_colliderObjectList.push_back(&m_colliderObject_sphere3);
	}
}


void MultiplayerGame::cMultiplayerGame::CleanUpGameObject()
{
	//m_renderObject_triangle.CleanUp();
	//m_renderObject_rectangle.CleanUp();
	////m_renderObject_plane.CleanUp();
	//m_renderObject_cube.CleanUp();
	//m_renderObject_Keqing.CleanUp();
	m_renderObject_Keqing_skin.CleanUp();
	//m_renderObject_Ganyu.CleanUp();
	for (cGameObject* renderObject : m_renderObjectList)
	{
		renderObject->CleanUp();
	}




	m_camera.CleanUp();

	m_effect_animate.reset();
	m_effect_standard.reset();


	// TODO: temporary code for clean up colldier object
	for (cPhysicDebugObject* colliderObject : m_colliderObjectList)
	{
		colliderObject->CleanUp();
	}
}



void MultiplayerGame::cMultiplayerGame::InitializeCollisionSystem()
{
	std::vector<Physics::cCollider*> colliderList;

	for (cPhysicDebugObject* colliderObject : m_colliderObjectList)
	{
		colliderList.push_back(colliderObject->GetCollider());
	}

	Physics::Collision::Initialize(colliderList, Physics::Collision::BroadPhase_BVH | Physics::Collision::NarrowPhase_Overlaps);
}


 