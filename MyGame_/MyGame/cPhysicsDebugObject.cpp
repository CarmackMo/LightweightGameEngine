
// Includes
//=========

#include <MyGame_/MyGame/cPhysicsDebugObject.h>



void eae6320::cPhysicDebugObject::UpdateBasedOnInput()
{
	// TODO 
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
