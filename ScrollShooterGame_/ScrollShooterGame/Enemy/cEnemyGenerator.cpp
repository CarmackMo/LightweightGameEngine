
// Includes
//========

#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemyGenerator.h>


using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cEnemyGenerator::Initialize(eae6320::Math::sVector i_position, eae6320::Math::sVector i_velocity)
{

	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = Math::sVector(0.0f, 0.0f, 0.0f);
		m_rigidBody.isTrigger = true;
	}

	// Initialize collider
	{
		Physics::sColliderSetting setting_AABB;
		setting_AABB.SettingForAABB(Math::sVector(-2.5f, -0.5f, -0.5f), Math::sVector(2.5f, 0.5f, 0.5f));
		InitializeCollider(setting_AABB);
	}

	// Initialize mesh & effect
	{
		//InitializeMesh("data/meshes/mesh_triangle.mesh");
		//InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}

}


void ScrollShooterGame::cEnemyGenerator::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	cGameObject::UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);


}
