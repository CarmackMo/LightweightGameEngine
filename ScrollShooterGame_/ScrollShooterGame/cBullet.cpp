
// Includes
//========

#include <Engine/Math/cQuaternion.h>
#include <ScrollShooterGame_/ScrollShooterGame/cBullet.h>

using namespace eae6320;


void ScrollShooterGame::cBullet::Initialize(
	eae6320::Math::sVector i_position,
	eae6320::Math::sVector i_velocity)
{
	m_rigidBody.position = i_position;
	m_rigidBody.velocity = i_velocity;
}


void ScrollShooterGame::cBullet::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	cGameObject::UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);


	// TODO: 
	if (m_rigidBody.position.x >= 40.0f ||
		m_rigidBody.position.y >= 40.0f ||
		m_rigidBody.position.z >= 40.0f)
	{
		cGameObject::CleanUp();
		delete this;
	}

}

