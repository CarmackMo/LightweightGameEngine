
// Includes
//========

#include <Engine/Math/cQuaternion.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/Collision.h>
#include <Engine/UserOutput/UserOutput.h>

#include <ScrollShooterGame_/ScrollShooterGame/cBullet.h>
#include <ScrollShooterGame_/ScrollShooterGame/cEnemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>

using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cBullet::Initialize(
	eae6320::Math::sVector i_position,
	eae6320::Math::sVector i_velocity)
{
	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = i_velocity;
		m_rigidBody.isTrigger = true;
	}

	// Initialize collider
	{
		Physics::sColliderSetting setting_sphere;
		setting_sphere.SettingForSphere(Math::sVector(0, 0, 0), 0.5f);
		InitializeCollider(setting_sphere);
		InitializeColliderLine();
	}

	// Initialize mesh & effect
	{
		InitializeMesh("data/meshes/mesh_rectangle.mesh");
		InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

	// Define physics behavior
	{
		m_collider->OnCollisionEnter = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				m_isCollide = true;

				if (dynamic_cast<cEnemy*>(other->m_gameobject) != nullptr)
				{
					UserOutput::ConsolePrint("Bullet hit enemy!! \n");
					cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(self->m_gameobject);
				}
			};

		m_collider->OnCollisionStay = [this](Physics::cCollider* self, Physics::cCollider* other) -> void 
			{};

		m_collider->OnCollisionExit = [this](Physics::cCollider* self, Physics::cCollider* other) -> void 
			{ 
				dynamic_cast<cBullet*>(self->m_gameobject)->m_isCollide = false; 
			};
	}
}


void ScrollShooterGame::cBullet::CleanUp()
{
	auto game = cScrollShooterGame::Instance();
	
	auto objIter = std::find(game->m_gameObjectList.begin(), game->m_gameObjectList.end(), this);
	if (objIter != game->m_gameObjectList.end())
	{
		game->m_gameObjectList.erase(objIter);
	}

	auto bulletIter = std::find(game->m_bulletList.begin(), game->m_bulletList.end(), this);
	if (bulletIter != game->m_bulletList.end())
	{
		game->m_bulletList.erase(bulletIter);
	}

	Physics::Collision::DeregisterCollider(this->GetCollider());
}


// TODO: Debug
//=========================

void ScrollShooterGame::cBullet::InitializeColliderLine()
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


eae6320::Graphics::cLine* ScrollShooterGame::cBullet::GetColliderLine() const
{
	if (m_isCollide)
		return m_collisionLine;
	else
		return m_colliderLine;
}