
// Includes
//========

#include <Engine/Physics/cColliderBase.h>

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

		m_rigidBody.angularSpeed = 10.0f;
		m_rigidBody.angularVelocity_axis_local = Math::sVector(0, 1, 0);
	}

	// Initialize collider
	{
		Physics::sColliderSetting setting_sphere;
		setting_sphere.SettingForSphere(Math::sVector(0, 0, 0), 0.45f);
		InitializeCollider(setting_sphere);
		InitializeColliderLine();
	}

	// Initialize mesh & effect
	{
		InitializeMesh("data/meshes/mesh_bullet_player.mesh");
		InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

	// Define physics behavior
	{
		m_collider->OnCollisionEnter = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				m_isCollide = true;

				if (std::dynamic_pointer_cast<cEnemy>(other->m_gameobject.lock()) != nullptr ||
					std::dynamic_pointer_cast<cBullet_Enemy>(other->m_gameobject.lock()) != nullptr)
				{
					self->m_gameobject.lock()->SetActive(false);
					cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(self->m_gameobject.lock());
				}
			};

		m_collider->OnCollisionStay = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{};

		m_collider->OnCollisionExit = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				std::dynamic_pointer_cast<cBullet>(self->m_gameobject.lock())->m_isCollide = false;
			};
	}
}
