
// Includes
//========

#include <Engine/Audio/Audio.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/UserOutput/UserOutput.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Enemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Player.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPlayer.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemy.Rock.h>

using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cBullet_Enemy::Initialize(
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
		setting_sphere.SettingForSphere(Math::sVector(0, 0, 0), 0.3f);
		InitializeCollider(setting_sphere);
		InitializeColliderLine();
	}

	// Initialize mesh & effect
	{
		InitializeMesh("data/meshes/mesh_bullet_enemy.mesh");
		InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

	// Define physics behavior
	{
		m_collider->OnCollisionEnter = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				m_isCollide = true;

				if (std::dynamic_pointer_cast<cPlayer>(other->m_gameobject.lock()) != nullptr ||
					std::dynamic_pointer_cast<cBullet_Player>(other->m_gameobject.lock()) != nullptr)
				{
					Audio::Play("hit_enemy");
					SetActive(false);
					cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(GetSelf());
				}
				else if (std::dynamic_pointer_cast<cEnemy_Rock>(other->m_gameobject.lock()) != nullptr)
				{
					GetRigidBody().velocity.x *= -1;
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
