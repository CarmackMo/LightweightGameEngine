
// Includes
//========

#include <Engine/Graphics/Graphics.h>
#include <Engine/Math/Random.h>
#include <Engine/Physics/Collision.h>
#include <Engine/Time/Time.h>

#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemy.Rock.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemyGenerator.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>


using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cEnemyGenerator::Initialize(eae6320::Math::sVector i_position, eae6320::Math::sVector i_velocity)
{
	// Initialize property
	{
		m_spawnCoolDown_rock = Math::Random::RandInRange(1.0f, 2.0f);
		m_spawnCoolDown_alien = Math::Random::RandInRange(3.0f, 5.0f);
	}

	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = Math::sVector(0.0f, 0.0f, 0.0f);
		m_rigidBody.isTrigger = true;
	}

	// Initialize collider
	{
		Physics::sColliderSetting setting_AABB;
		setting_AABB.SettingForAABB(Math::sVector(m_width * -0.5f, -0.2f, -0.2f), Math::sVector(m_width * 0.5f, 0.2f, 0.2f));
		InitializeCollider(setting_AABB);
	}

	// Initialize mesh & effect
	{
		//InitializeMesh("data/meshes/mesh_triangle.mesh");
		//InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

}


void ScrollShooterGame::cEnemyGenerator::CleanUp()
{
	auto game = cScrollShooterGame::Instance();

	auto objIter = std::find(game->m_gameObjectList.begin(), game->m_gameObjectList.end(), this);
	if (objIter != game->m_gameObjectList.end())
	{
		game->m_gameObjectList.erase(objIter);
	}

	Physics::Collision::DeregisterCollider(this->GetCollider());
}


void ScrollShooterGame::cEnemyGenerator::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	cGameObject::UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	double currentTime = Time::ConvertTicksToSeconds(Time::GetCurrentSystemTimeTickCount());
	if (currentTime - m_lastSpawnTime_rock >= m_spawnCoolDown_rock)
	{
		SpawnRock();
		m_lastSpawnTime_rock = currentTime;
		m_spawnCoolDown_rock = static_cast<double>(Math::Random::RandInRange(0.5f, 1.5f));
	}
	if (currentTime - m_lastSpawnTime_alien >= m_spawnCoolDown_alien)
	{
		// TODO
	}

}


// Implementation
//=========================

void ScrollShooterGame::cEnemyGenerator::SpawnRock()
{
	Math::sVector rockPos;
	rockPos.x = GetRigidBody().position.x + Math::Random::RandInRange(m_width * -0.5f, m_width * 0.5f);
	rockPos.y = GetRigidBody().position.y - 1.0f;
	rockPos.z = GetRigidBody().position.z;

	Math::sVector rockSpeed;
	rockSpeed.y = Math::Random::RandInRange(-2.0f, -1.0f);

	cEnemy_Rock* rock = new cEnemy_Rock();
	rock->Initialize(rockPos, rockSpeed);


	Physics::Collision::RegisterCollider(rock->GetCollider());

	auto game = cScrollShooterGame::Instance();
	game->m_gameObjectList.push_back(rock);

}


// TODO: Debug
//=========================

void ScrollShooterGame::cEnemyGenerator::InitializeColliderLine()
{
	if (m_colliderLine != nullptr)
	{
		m_colliderLine->DecrementReferenceCount();
		m_colliderLine = nullptr;
	}

	uint32_t vertexCount = 0;
	auto vertexVec = std::vector<Math::sVector>();
	uint32_t indexCount = 0;
	auto indexVec = std::vector<uint16_t>();
	m_collider->GenerateRenderData(vertexCount, vertexVec, indexCount, indexVec);

	// Initialize collider outline
	{
		Graphics::VertexFormats::sVertex_line* vertexData = new Graphics::VertexFormats::sVertex_line[vertexCount];
		for (uint32_t i = 0; i < vertexCount; i++)
		{
			if (m_rigidBody.isTrigger == true)
				vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 1.0f, 0.75f, 0.796f, 1.0f);
			else if (m_rigidBody.isStatic == true)
				vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 1.0f, 0.8f, 0.0f, 1.0f);
			else
				vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 0.0f, 1.0f, 0.0f, 1.0f);
		}
		uint16_t* indexData = new uint16_t[indexCount];
		for (uint32_t i = 0; i < indexCount; i++)
		{
			indexData[i] = indexVec[i];
		}

		// Send cLine data to rendering thread for initialization
		if (Graphics::AcquireRenderObjectInitMutex() == WAIT_OBJECT_0)
		{
			Graphics::AddLineInitializeTask(&m_colliderLine, vertexData, vertexCount, indexData, indexCount);
			Graphics::ReleaseRenderObjectInitMutex();
		}

		delete[] vertexData;
		delete[] indexData;
	}

	// Initialize collision outline
	{
		Graphics::VertexFormats::sVertex_line* vertexData = new Graphics::VertexFormats::sVertex_line[vertexCount];
		for (uint32_t i = 0; i < vertexCount; i++)
		{
			vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 1, 0, 0, 1);
		}
		uint16_t* indexData = new uint16_t[indexCount];
		for (uint32_t i = 0; i < indexCount; i++)
		{
			indexData[i] = indexVec[i];
		}

		// Send cLine data to rendering thread for initialization
		if (Graphics::AcquireRenderObjectInitMutex() == WAIT_OBJECT_0)
		{
			Graphics::AddLineInitializeTask(&m_collisionLine, vertexData, vertexCount, indexData, indexCount);
			Graphics::ReleaseRenderObjectInitMutex();
		}

		delete[] vertexData;
		delete[] indexData;
	}
}


eae6320::Graphics::cLine* ScrollShooterGame::cEnemyGenerator::GetColliderLine() const
{
	if (m_isCollide)
		return m_collisionLine;
	else
		return m_colliderLine;
}
