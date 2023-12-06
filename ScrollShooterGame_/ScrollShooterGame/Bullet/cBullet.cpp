
// Includes
//========

#include <Engine/Graphics/Graphics.h>
#include <Engine/Physics/Collision.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.h>
#include <ScrollShooterGame_/ScrollShooterGame/cScrollShooterGame.h>

using namespace eae6320;


// Interface
//=========================

void ScrollShooterGame::cBullet::CleanUp()
{
	Physics::Collision::DeregisterCollider(this->GetCollider());

	auto game = cScrollShooterGame::Instance();
	auto objIter = std::find(game->m_gameObjectList.begin(), game->m_gameObjectList.end(), this->m_self);
	if (objIter != game->m_gameObjectList.end())
	{
		game->m_gameObjectList.erase(objIter);
	}

	if (Graphics::AcquireRenderObjectCleanUpMutex() == WAIT_OBJECT_0)
	{
		Graphics::AddLineCleanUpTask(m_colliderLine);
		Graphics::ReleaseRenderObjectCleanUpMutex();
	}

	if (Graphics::AcquireRenderObjectCleanUpMutex() == WAIT_OBJECT_0)
	{
		Graphics::AddLineCleanUpTask(m_collisionLine);
		Graphics::ReleaseRenderObjectCleanUpMutex();
	}

	cGameObject::CleanUp();
}


void ScrollShooterGame::cBullet::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	cGameObject::UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	if (m_rigidBody.position.y >= m_boundary ||
		m_rigidBody.position.y <= m_boundary * -1)
	{
		this->SetActive(false);
		cScrollShooterGame::Instance()->AddGameObjectCleanUpTask(this->GetSelf());
	}
}


// TODO: Debug
//=========================

void ScrollShooterGame::cBullet::SetIsCollide(bool isCollide)
{
	m_isCollide = isCollide;
}


void ScrollShooterGame::cBullet::InitializeColliderLine()
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


std::weak_ptr<eae6320::Graphics::cLine> ScrollShooterGame::cBullet::GetColliderLine() const
{
	if (m_isCollide)
		return m_collisionLine;
	else
		return m_colliderLine;
}