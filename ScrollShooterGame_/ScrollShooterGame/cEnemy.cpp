
// Includes
//========

#include <Engine/Graphics/Graphics.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/Collision.h>
#include <Engine/UserOutput/UserOutput.h>

#include <ScrollShooterGame_/ScrollShooterGame/cBullet.h>
#include <ScrollShooterGame_/ScrollShooterGame/cEnemy.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>

using namespace eae6320;


// Interface
//=========================


// TODO: Debug
//=========================

void ScrollShooterGame::cEnemy::InitializeColliderLine()
{
	if (m_colliderLine != nullptr)
	{
		m_colliderLine->DecrementReferenceCount();
		m_colliderLine = nullptr;
	}

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
			Graphics::AddLineInitializeTask(&m_colliderLine, vertexData, vertexCount, indexData, indexCount);
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
			Graphics::AddLineInitializeTask(&m_collisionLine, vertexData, vertexCount, indexData, indexCount);
			Graphics::ReleaseRenderObjectInitMutex();
		}

		delete[] vertexData;
		delete[] indexData;
	}
}


eae6320::Graphics::cLine* ScrollShooterGame::cEnemy::GetColliderLine() const
{
	if (m_isCollide)
		return m_collisionLine;
	else
		return m_colliderLine;
}
