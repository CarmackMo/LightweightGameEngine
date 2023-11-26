// Includes
//=========

#include <iostream>

#include "cScrollShooterGame.h"

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

using namespace eae6320;

// Inherited Implementation
//=========================

// Run
//----

void ScrollShooterGame::cScrollShooterGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Escape))
	{
		// Exit the application
		const auto result = Exit(EXIT_SUCCESS);
		EAE6320_ASSERT(result);
	}
}


void ScrollShooterGame::cScrollShooterGame::UpdateSimulationBasedOnInput()
{
	m_camera.UpdateBasedOnInput();

	for (size_t i = 0; i < m_gameObjectList.size(); i++)
	{
		m_gameObjectList[i]->UpdateBasedOnInput();
	}
}


void ScrollShooterGame::cScrollShooterGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	size_t listSize = m_gameObjectList.size();
	for (size_t i = 0; i < listSize; i++)
	{
		m_gameObjectList[i]->UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	}

	// TODO: Temporary code for collider debug

	Physics::Collision::Update_CollisionDetection();

	Physics::Collision::Update_CollisionResolution();

}


void ScrollShooterGame::cScrollShooterGame::SubmitDataToBeRendered(
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

	// Submit normal render data
	{
		size_t renderObjectNum = m_gameObjectList.size();
		size_t arraySize = m_gameObjectList.size();

		Graphics::ConstantBufferFormats::sNormalRender* normalRenderDataArray = new Graphics::ConstantBufferFormats::sNormalRender[arraySize];

		// Render data of render objects 
		for (size_t i = 0; i < m_gameObjectList.size(); i++)
		{
			if (m_gameObjectList[i]->GetMesh() == nullptr || m_gameObjectList[i]->GetEffect() == nullptr)
				continue;

			normalRenderDataArray[i].Initialize(
				m_gameObjectList[i]->GetMesh(), m_gameObjectList[i]->GetEffect(),
				m_gameObjectList[i]->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}

		Graphics::SubmitNormalRenderData(normalRenderDataArray, static_cast<uint32_t>(arraySize));

		// clean up 
		for (size_t i = 0; i < arraySize; i++)
		{
			normalRenderDataArray[i].CleanUp();
		}

		delete[] normalRenderDataArray;
	}

	// Submit debug render data (for colliders)
	{
		auto BVHRenderData = std::vector<std::pair<eae6320::Graphics::cLine*, eae6320::Math::cMatrix_transformation>>();
		BVHRenderData = Physics::Collision::GetBVHRenderData();

		size_t BVHTreeSize = BVHRenderData.size();
		size_t bulletSize = m_bulletList.size() + BVHTreeSize;
		size_t enemySize = 1;
		size_t totalArraySize = bulletSize + enemySize;

		Graphics::ConstantBufferFormats::sDebugRender* debugDataArray = new Graphics::ConstantBufferFormats::sDebugRender[totalArraySize];


		// Render data of BVH tree
		for (size_t i = 0; i < BVHTreeSize; i++)
		{
			if (BVHRenderData[i].first == nullptr)
				continue;

			debugDataArray[i].Initialize(BVHRenderData[i].first, BVHRenderData[i].second);
		}

		// Render data of bullets
		for (size_t i = BVHTreeSize; i < bulletSize; i++)
		{
			auto bullet = m_bulletList[i - BVHTreeSize];

			if (bullet == nullptr || bullet->GetColliderLine() == nullptr)
				continue;

			debugDataArray[i].Initialize(bullet->GetColliderLine(), bullet->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		}

		//// Render data of enemy
		//{
		//	if (m_enemy != nullptr && m_enemy->GetColliderLine() != nullptr)
		//	{
		//		debugDataArray[totalArraySize - 1].Initialize(m_enemy->GetColliderLine(), m_enemy->GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		//	}
		//}

		Graphics::SubmitDebugRenderData(debugDataArray, static_cast<uint32_t>(totalArraySize));

		// Clean up
		for (size_t i = 0; i < totalArraySize; i++)
		{
			debugDataArray[i].CleanUp();
		}

		delete[] debugDataArray;
	}
}


void ScrollShooterGame::cScrollShooterGame::RuntimeCleanUp()
{
	CleanUpGameObject();
}


// Initialize / Clean Up
//----------------------

eae6320::cResult ScrollShooterGame::cScrollShooterGame::Initialize()
{
	// Logging
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
	}


	InitializeCamera();

	InitializeGameObject();

	// TODO: temporary code for initialize collision system
	InitializeCollisionSystem();


	return Results::Success;
}


eae6320::cResult ScrollShooterGame::cScrollShooterGame::CleanUp()
{
	// Logging
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
	}


	m_camera.CleanUp();

	for (cGameObject* object : m_gameObjectList)
	{
		m_gameObjectCleanUpQueue.push(object);
	}

	CleanUpGameObject();

	return Results::Success;
}


void ScrollShooterGame::cScrollShooterGame::InitializeCamera()
{
	m_camera.Initialize(
		0.0f, 0.0f, 5.0f,
		Math::ConvertDegreesToRadians(45.0f),
		0.1f,
		100.0f);
}


void ScrollShooterGame::cScrollShooterGame::InitializeGameObject()
{
	// TODO: temporary code for player object
	{
		m_player = new cPlayer();
		m_player->Initialize(Math::sVector(0.0f, 0.0f, -15.0f), Math::sVector());

		m_gameObjectList.push_back(m_player);
	}

	// TODO: temporary code for enemy_alien object
	{
		m_alien = new cEnemy_Alien();
		m_alien->Initialize(Math::sVector(0.0f, 4.0f, -15.0f), Math::sVector(0.0f, 0.0f, 0.0f));

		m_gameObjectList.push_back(m_alien);
	}

	// TODO: temporary code for enemy generator object
	{
		m_enemyGenerator = new cEnemyGenerator();
		m_enemyGenerator->Initialize(Math::sVector(0.0f, 9.0f, -15.0f), Math::sVector(0.0f, 0.0f, 0.0f));

		m_gameObjectList.push_back(m_enemyGenerator);
	}
}


void ScrollShooterGame::cScrollShooterGame::CleanUpGameObject()
{
	while (m_gameObjectCleanUpQueue.empty() == false)
	{
		cGameObject* object = m_gameObjectCleanUpQueue.front();
		m_gameObjectCleanUpQueue.pop();

		object->CleanUp();
		delete object;
	}
}


void ScrollShooterGame::cScrollShooterGame::InitializeCollisionSystem()
{
	std::vector<Physics::cCollider*> colliderList;

	colliderList.push_back(m_player->GetCollider());
	colliderList.push_back(m_alien->GetCollider());
	colliderList.push_back(m_enemyGenerator->GetCollider());

	Physics::Collision::Initialize(colliderList, Physics::Collision::BroadPhase_BVH | Physics::Collision::NarrowPhase_Overlaps);
}


void ScrollShooterGame::cScrollShooterGame::AddGameObjectCleanUpTask(cGameObject* i_gameObject)
{
	m_gameObjectCleanUpQueue.push(i_gameObject);
}


 