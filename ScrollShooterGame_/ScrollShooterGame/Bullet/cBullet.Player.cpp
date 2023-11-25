
// Includes
//========

#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/Collision.h>
#include <Engine/UserOutput/UserOutput.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Enemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Player.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>

using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cBullet_Player::Initialize(
	eae6320::Math::sVector i_position,
	eae6320::Math::sVector i_velocity)
{
	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = m_velocity;
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

				if (dynamic_cast<cEnemy*>(other->m_gameobject) != nullptr ||
					dynamic_cast<cBullet_Enemy*>(other->m_gameobject) != nullptr)
				{
					UserOutput::ConsolePrint("Player bullet hit enemy!! \n");
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


void ScrollShooterGame::cBullet_Player::CleanUp()
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