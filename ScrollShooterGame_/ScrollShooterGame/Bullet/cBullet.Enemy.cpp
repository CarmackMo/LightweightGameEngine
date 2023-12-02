
// Includes
//========

#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/Collision.h>
#include <Engine/UserOutput/UserOutput.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Enemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.Player.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPlayer.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>

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

				if (dynamic_cast<cPlayer*>(std::shared_ptr<cGameObject>(other->m_gameobject).get()) != nullptr ||
					dynamic_cast<cBullet_Player*>(std::shared_ptr<cGameObject>(other->m_gameobject).get()) != nullptr)
				{
					UserOutput::ConsolePrint("Enemy bullet hit player!! \n");
					cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(std::shared_ptr<cGameObject>(self->m_gameobject));
				}
			};

		m_collider->OnCollisionStay = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{};

		m_collider->OnCollisionExit = [this](Physics::cCollider* self, Physics::cCollider* other) -> void
			{
				dynamic_cast<cBullet*>(std::shared_ptr<cGameObject>(self->m_gameobject).get())->m_isCollide = false;
			};
	}
}


void ScrollShooterGame::cBullet_Enemy::CleanUp()
{
	Physics::Collision::DeregisterCollider(this->GetCollider());

	auto game = cScrollShooterGame::Instance();

	// TODO
	//auto objIter = std::find(game->m_gameObjectList.begin(), game->m_gameObjectList.end(), this);
	//if (objIter != game->m_gameObjectList.end())
	//{
	//	game->m_gameObjectList.erase(objIter);
	//}

	auto objIter = std::find(game->m_gameObjectList_sp.begin(), game->m_gameObjectList_sp.end(), this->m_self);
	if (objIter != game->m_gameObjectList_sp.end())
	{
		game->m_gameObjectList_sp.erase(objIter);
	}

	auto bulletIter = std::find(game->m_bulletList.begin(), game->m_bulletList.end(), this);
	if (bulletIter != game->m_bulletList.end())
	{
		game->m_bulletList.erase(bulletIter);
	}

	cGameObject::CleanUp();
}