
// Includes
//========

#include <Engine/Math/cQuaternion.h>
#include <Engine/Graphics/Graphics.h>
#include <ScrollShooterGame_/ScrollShooterGame/cBullet.h>

using namespace eae6320;


void ScrollShooterGame::cBullet::Initialize(
	eae6320::Math::sVector i_position,
	eae6320::Math::sVector i_velocity)
{
	// Initialize rigid body
	{
		m_rigidBody.position = i_position;
		m_rigidBody.velocity = i_velocity;
	}

	// Initialize collider
	{
		Physics::sColliderSetting setting_sphere;
		setting_sphere.SettingForSphere(Math::sVector(0, 0, 0), 0.5f);
		InitializeCollider(setting_sphere);
		InitializeColliderLine();
		m_collider->OnCollisionEnter =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { m_isCollide = true; };
		m_collider->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void {  };
		m_collider->OnCollisionExit =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { m_isCollide = false; };
	}

	// Initialize mesh & effect
	{
		InitializeMesh("data/meshes/mesh_rectangle.mesh");
		InitializeEffect("data/Shaders/Vertex/standard.shader", "data/Shaders/Fragment/standard.shader");
	}
}


void ScrollShooterGame::cBullet::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	cGameObject::UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);


	// TODO: 
	if (m_rigidBody.position.x >= 3.0f ||
		m_rigidBody.position.y >= 3.0f ||
		m_rigidBody.position.z >= 3.0f)
	{
		CleanUp();
	}

}

void ScrollShooterGame::cBullet::CleanUp()
{
	if (m_cleanUpCallback != nullptr)
		m_cleanUpCallback();

	cGameObject::CleanUp();

	delete this;
}



// TODO

void ScrollShooterGame::cBullet::InitializeColliderLine()
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


eae6320::Graphics::cLine* ScrollShooterGame::cBullet::GetColliderLine() const
{
	if (m_isCollide)
		return m_collisionLine;
	else
		return m_colliderLine;
}