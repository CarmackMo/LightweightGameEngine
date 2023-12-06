
// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Graphics/Graphics.h>

#include <ScrollShooterGame_/ScrollShooterGame/cPhysicsDebugObject.h>


using namespace eae6320;



void ScrollShooterGame::cPhysicDebugObject::CleanUp()
{
	cGameObject::CleanUp();

	// This function might be called when cLine is not yet initialized by the
	// rendering thread. Add the cLine clean up task to rendering thread anyway
	// and do null pointer safety check in rendering thread
	if (Graphics::AcquireRenderObjectCleanUpMutex() == WAIT_OBJECT_0)
	{
		Graphics::AddLineCleanUpTask(m_colliderLine);
		Graphics::ReleaseRenderObjectCleanUpMutex();
		m_colliderLine.reset();
	}

	if (Graphics::AcquireRenderObjectCleanUpMutex() == WAIT_OBJECT_0)
	{
		Graphics::AddLineCleanUpTask(m_collisionLine);
		Graphics::ReleaseRenderObjectCleanUpMutex();
		m_collisionLine.reset();
	}
}


void ScrollShooterGame::cPhysicDebugObject::UpdateBasedOnInput()
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


void ScrollShooterGame::cPhysicDebugObject::InitializeColliderLine()
{
	uint32_t vertexCount = 0;
	auto vertexVec = std::vector<Math::sVector>();
	uint32_t indexCount = 0;
	auto indexVec = std::vector<uint16_t>();
	m_collider->GenerateRenderData(vertexCount, vertexVec, indexCount, indexVec);

	// Initialize collider outline
	{
		Graphics::VertexFormats::sVertex_line* vertexData = new Graphics::VertexFormats::sVertex_line[vertexCount];
		for (uint32_t i = 0; i < vertexCount; i++)
		{
			if (m_rigidBody.isTrigger == true)
				vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 1.0f, 0.75f, 0.796f, 1.0f);
			else if (m_rigidBody.isStatic == true)
				vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 1.0f, 0.8f, 0.0f, 1.0f);
			else
				vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 0.0f, 1.0f, 0.0f, 1.0f);
		}
		uint16_t* indexData = new uint16_t[indexCount];
		for (uint32_t i = 0; i < indexCount; i++)
		{
			indexData[i] = indexVec[i];
		}

		// Send cLine data to rendering thread for initialization
		if (Graphics::AcquireRenderObjectInitMutex() == WAIT_OBJECT_0)
		{
			Graphics::AddLineInitializeTask(m_colliderLine, vertexData, vertexCount, indexData, indexCount);
			Graphics::ReleaseRenderObjectInitMutex();
		}

		delete[] vertexData;
		delete[] indexData;
	}

	// Initialize collision outline
	{
		Graphics::VertexFormats::sVertex_line* vertexData = new Graphics::VertexFormats::sVertex_line[vertexCount];
		for (uint32_t i = 0; i < vertexCount; i++)
		{
			vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z, 1, 0, 0, 1);
		}
		uint16_t* indexData = new uint16_t[indexCount];
		for (uint32_t i = 0; i < indexCount; i++)
		{
			indexData[i] = indexVec[i];
		}

		// Send cLine data to rendering thread for initialization
		if (Graphics::AcquireRenderObjectInitMutex() == WAIT_OBJECT_0)
		{
			Graphics::AddLineInitializeTask(m_collisionLine, vertexData, vertexCount, indexData, indexCount);
			Graphics::ReleaseRenderObjectInitMutex();
		}

		delete[] vertexData;
		delete[] indexData;
	}
}


std::weak_ptr<eae6320::Graphics::cLine> ScrollShooterGame::cPhysicDebugObject::GetColliderLine() const
{
	if (m_isCollide)
		return m_collisionLine;
	else
		return m_colliderLine;
}
