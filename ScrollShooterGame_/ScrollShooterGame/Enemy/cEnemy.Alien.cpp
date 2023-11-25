
// Includes
//========

#include <Engine/Physics/Collision.h>
#include <Engine/Time/Time.h>
#include <Engine/UserOutput/UserOutput.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Enemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Player.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemy.Alien.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPlayer.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>


using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cEnemy_Alien::Initialize(eae6320::Math::sVector i_position, eae6320::Math::sVector i_velocity)
{
	// Initialize property
	{
		m_HP = 3;
		m_lastShoot_second = Time::ConvertTicksToSeconds(Time::GetCurrentSystemTimeTickCount());
	}

	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = m_velocity;
		m_rigidBody.isTrigger = true;
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
		InitializeMesh("data/meshes/mesh_triangle.mesh");
		InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

	// Define physics behavior
	{
		m_collider->OnCollisionEnter = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				m_isCollide = true;
				if (dynamic_cast<cBullet_Player*>(other->m_gameobject) != nullptr ||
					dynamic_cast<cPlayer*>(other->m_gameobject) != nullptr)
				{
					m_HP--;
					if (m_HP == 0)
					{
						UserOutput::ConsolePrint("Enemy is Killed!! \n");
						cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(self->m_gameobject);
					}
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


void ScrollShooterGame::cEnemy_Alien::CleanUp()
{
	auto game = cScrollShooterGame::Instance();

	auto objIter = std::find(game->m_gameObjectList.begin(), game->m_gameObjectList.end(), this);
	if (objIter != game->m_gameObjectList.end())
	{
		game->m_gameObjectList.erase(objIter);
	}

	Physics::Collision::DeregisterCollider(this->GetCollider());
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
			m_lastShoot_second = currentTime;
		}
	}
}


// Implementation
//=========================

void ScrollShooterGame::cEnemy_Alien::ShootBullet()
{
	Math::sVector selfPos = GetRigidBody().position;
	cBullet_Enemy* newBullet = new cBullet_Enemy();
	newBullet->Initialize(Math::sVector(selfPos.x, selfPos.y - 1.0f, selfPos.z));

	Physics::Collision::RegisterCollider(newBullet->GetCollider());

	auto game = cScrollShooterGame::Instance();
	game->m_gameObjectList.push_back(newBullet);
	game->m_bulletList.push_back(newBullet);
}
