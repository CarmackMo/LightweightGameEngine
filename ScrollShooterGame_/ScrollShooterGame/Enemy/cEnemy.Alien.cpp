
// Includes
//========

#include <Engine/Audio/Audio.h>
#include <Engine/Math/Random.h>
#include <Engine/Physics/Collision.h>
#include <Engine/Time/Time.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Enemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Player.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemy.Alien.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemy.Rock.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPlayer.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>

#include <memory>


using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cEnemy_Alien::Initialize(
	eae6320::Math::sVector i_position, 
	eae6320::Math::sVector i_velocity)
{
	// Initialize property
	{
		m_HP = 3;
		m_Boundary = 7.0f;

		m_lastShoot_second = Time::ConvertTicksToSeconds(Time::GetCurrentSystemTimeTickCount());
		m_shootCoolDownRange = eae6320::Math::sVector(3.0f, 6.5f, 0.0f);
		m_shootCoolDown = static_cast<double>(Math::Random::RandInRange(m_shootCoolDownRange.x, m_shootCoolDownRange.y));

		m_velocity = eae6320::Math::sVector(-2.0f, -0.25f, 0.0f);
	}

	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = m_velocity;
		m_rigidBody.isTrigger = true;

		m_rigidBody.angularSpeed = 2.5f;
		m_rigidBody.angularVelocity_axis_local = Math::sVector(0, 1, 0);
	}

	// Initialize collider
	{
		Physics::sColliderSetting setting_AABB;
		setting_AABB.SettingForAABB(Math::sVector(-0.5f, -0.5f, -0.5f), Math::sVector(0.5f, 0.5f, 0.5f));
		InitializeCollider(setting_AABB);
		InitializeColliderLine();
	}

	// Initialize mesh & effect
	{
		InitializeMesh("data/meshes/mesh_enemy_alien.mesh");
		InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

	// Define physics behavior
	{
		m_collider->OnCollisionEnter = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				m_isCollide = true;

				if (std::dynamic_pointer_cast<cBullet_Player>(other->m_gameobject.lock()) != nullptr ||
					std::dynamic_pointer_cast<cPlayer>(other->m_gameobject.lock()) != nullptr)
				{
					m_HP--;
					if (m_HP == 0)
					{
						Audio::Play("boom_enemy");
						SetActive(false);
						cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(GetSelf());
					}
					else
					{
						Audio::Play("hit_enemy");
					}
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


void ScrollShooterGame::cEnemy_Alien::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	cEnemy::UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	// Movement update
	{
		if (m_rigidBody.position.x < -1 * m_Boundary)
		{
			m_rigidBody.velocity.x = -1 * m_velocity.x;
		}
		else if (m_rigidBody.position.x > m_Boundary)
		{
			m_rigidBody.velocity.x = m_velocity.x;
		}
	}

	// Shoot update
	{
		auto currentTime = Time::ConvertTicksToSeconds(Time::GetCurrentSystemTimeTickCount());

		if (currentTime - m_lastShoot_second >= m_shootCoolDown)
		{
			ShootBullet();
			m_shootCoolDown = static_cast<double>(Math::Random::RandInRange(m_shootCoolDownRange.x, m_shootCoolDownRange.y));
			m_lastShoot_second = currentTime;
		}
	}
}


// Implementation
//=========================

void ScrollShooterGame::cEnemy_Alien::ShootBullet()
{
	auto game = cScrollShooterGame::Instance();

	Math::sVector playerPos = game->GetPlayer()->GetRigidBody().position;
	Math::sVector selfPos = GetRigidBody().position;
	Math::sVector velocity = (playerPos - selfPos).GetNormalized() * 1.5f;

	cBullet_Enemy* newBullet = new cBullet_Enemy();
	newBullet->Initialize(Math::sVector(selfPos.x, selfPos.y - 1.0f, selfPos.z), velocity);

	Physics::Collision::RegisterCollider(newBullet->GetCollider());

	game->m_gameObjectList.push_back(newBullet->GetSelf());
}
