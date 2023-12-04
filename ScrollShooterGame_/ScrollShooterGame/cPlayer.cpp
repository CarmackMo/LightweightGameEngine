
// Includes
//========

#include <Engine/Audio/Audio.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/Collision.h>
#include <Engine/Time/Time.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Utilities/SmartPtrs.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Player.h>
#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Enemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPlayer.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>

using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cPlayer::Initialize(eae6320::Math::sVector i_position, eae6320::Math::sVector i_velocity)
{
	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = i_velocity;
		m_rigidBody.isTrigger = true;
	}

	// Initialize collider
	{
		Physics::sColliderSetting setting_AABB1;
		setting_AABB1.SettingForAABB(Math::sVector(-0.9f, -0.9f, -0.9f), Math::sVector(0.9f, 0.9f, 0.9f));
		InitializeCollider(setting_AABB1);
		InitializeColliderLine();
	}

	// Initialize mesh & effect
	{
		InitializeMesh("data/meshes/mesh_cube.mesh");
		InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

	// Define physics behavior
	{
		m_collider->OnCollisionEnter = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				m_isCollide = true;
				if (dynamic_cast<cEnemy*>(other->m_gameobject.lock().get()) != nullptr ||
					dynamic_cast<cBullet_Enemy*>(other->m_gameobject.lock().get()) != nullptr)
				{
					UserOutput::ConsolePrint("Player is killed by enemy!! \n");
					cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(self->m_gameobject.lock());
				}
			};

		m_collider->OnCollisionStay = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{};

		m_collider->OnCollisionExit = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				m_isCollide = false;
			};
	}
}


void ScrollShooterGame::cPlayer::CleanUp()
{
	Physics::Collision::DeregisterCollider(this->GetCollider());

	auto game = cScrollShooterGame::Instance();
	auto objIter = std::find(game->m_gameObjectList.begin(), game->m_gameObjectList.end(), this->m_self);
	if (objIter != game->m_gameObjectList.end())
	{
		game->m_gameObjectList.erase(objIter);
	}

	cGameObject::CleanUp();
}


void ScrollShooterGame::cPlayer::UpdateBasedOnInput()
{
	// Basic movement update
	{
		if (UserInput::IsKeyPressed('A'))
			m_rigidBody.velocity.x = -3.0f;
		else if (UserInput::IsKeyPressed('D'))
			m_rigidBody.velocity.x = 3.0f;
		else
			m_rigidBody.velocity.x = 0.0f;

		if (UserInput::IsKeyPressed('S'))
			m_rigidBody.velocity.y = -3.0f;
		else if (UserInput::IsKeyPressed('W'))
			m_rigidBody.velocity.y = 3.0f;
		else
			m_rigidBody.velocity.y = 0.0f;

		if (UserInput::IsKeyPressed('R'))
			m_rigidBody.velocity.z = -3.0f;
		else if (UserInput::IsKeyPressed('F'))
			m_rigidBody.velocity.z = 3.0f;
		else
			m_rigidBody.velocity.z = 0.0f;
	}

	// Attack update
	{
		if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter))
		{
			auto currentTime = Time::ConvertTicksToSeconds(Time::GetCurrentSystemTimeTickCount());
			if (currentTime - m_lastShoot_second >= m_shootCoolDown)
			{
				Audio::Play("shoot");

				ShootBullet();
				m_lastShoot_second = currentTime;
			}
		}
	}
}


// Implementation
//=========================

void ScrollShooterGame::cPlayer::ShootBullet()
{
	Math::sVector selfPos = GetRigidBody().position;
	cBullet_Player* newBullet = new cBullet_Player();
	newBullet->Initialize(Math::sVector(selfPos.x, selfPos.y + 1.5f, selfPos.z));

	Physics::Collision::RegisterCollider(newBullet->GetCollider());

	auto game = cScrollShooterGame::Instance();
	game->m_gameObjectList.push_back(newBullet->GetSelf());
}


// TODO: Debug
//=========================

void ScrollShooterGame::cPlayer::InitializeColliderLine()
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


eae6320::Graphics::cLine* ScrollShooterGame::cPlayer::GetColliderLine() const
{
	if (m_isCollide)
		return m_collisionLine;
	else
		return m_colliderLine;
}
