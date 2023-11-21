
// Includes
//========


#include <ScrollShooterGame_/ScrollShooterGame/cBullet.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPlayer.h>

using namespace eae6320;



void ScrollShooterGame::cPlayer::UpdateBasedOnInput()
{
	// Basic movement
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



	// Attack
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter))
	{
		//TODO

		//cBullet* newBullet = new cBullet();
		//newBullet->Initialize(m_rigidBody.position, Math::sVector(0,1,0) );

		if (bulletCreation != nullptr)
			bulletCreation();
	}


}
