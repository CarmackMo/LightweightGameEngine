// Includes
//=========

#include "cGameObject.h"


void eae6320::cGameObject::InitializeMesh(
	eae6320::Graphics::VertexFormats::sVertex_mesh* i_vertexData,
	const uint32_t i_vertexCount,
	uint16_t* i_indexData,
	const uint32_t i_indexCount,
	const uint32_t i_indexCountToRender,
	const uint32_t i_indexOfFirstIndexToUse,
	const uint32_t i_offsetToAddToEachIndex)
{
	if (m_mesh != nullptr)
	{
		m_mesh->DecrementReferenceCount();
		m_mesh = nullptr;
	}

	Graphics::cMesh::Create(
		m_mesh,
		i_vertexData, i_vertexCount,
		i_indexData, i_indexCount, 
		i_indexCountToRender,
		i_indexOfFirstIndexToUse, 
		i_offsetToAddToEachIndex);
}

void eae6320::cGameObject::InitializeMesh(const std::string& i_meshPath)
{
	if (m_mesh != nullptr)
	{
		m_mesh->DecrementReferenceCount();
		m_mesh = nullptr;
	}

	Graphics::cMesh::Create(m_mesh, i_meshPath);
}


void eae6320::cGameObject::InitializeEffect(
	const std::string& i_vertexShaderPath, 
	const std::string& i_fragmentShaderPath)
{
	if (m_effect != nullptr)
	{
		m_effect->DecrementReferenceCount();
		m_effect = nullptr;
	}

	Graphics::cEffect::Create(
		m_effect, i_vertexShaderPath, i_fragmentShaderPath);
}


void eae6320::cGameObject::CleanUp()
{
	if (m_mesh != nullptr) { m_mesh->DecrementReferenceCount(); }
	if (m_effect != nullptr) { m_effect->DecrementReferenceCount(); }

	// TODO: Temporary code for collider
	if (m_AABBLine != nullptr) { m_AABBLine->DecrementReferenceCount(); }
	if (m_collider != nullptr) { delete m_collider; }
}


eae6320::Physics::sRigidBodyState& eae6320::cGameObject::GetRigidBody()
{
	return m_rigidBody;
}


eae6320::Graphics::cMesh* eae6320::cGameObject::GetMesh() const
{
	return m_mesh;
}


eae6320::Graphics::cEffect* eae6320::cGameObject::GetEffect() const
{
	return m_effect;
}


eae6320::Math::cMatrix_transformation eae6320::cGameObject::GetCurrentTransform() const
{
	return Math::cMatrix_transformation(m_rigidBody.orientation, m_rigidBody.position);
}


eae6320::Math::cMatrix_transformation eae6320::cGameObject::GetPredictedTransform(const float i_secondCountToExtrapolate) const
{
	return m_rigidBody.PredictFutureTransform(i_secondCountToExtrapolate);
}


void eae6320::cGameObject::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	// Update rigid body 
	m_rigidBody.Update(i_elapsedSecondCount_sinceLastUpdate);
	m_AABB.Update(m_rigidBody);
}


void eae6320::cGameObject::UpdateBasedOnInput()
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
}





// TODO: Tempory code for rendering collider box and debug collider

#include <Engine/Math/sVector.h>

void eae6320::cGameObject::InitializeAABB(
	float x1, float y1, float z1,
	float x2, float y2, float z2)
{
	m_AABB = Physics::cAABBCollider(Math::sVector(x1, y1, z1), Math::sVector(x2, y2, z2));
}


void eae6320::cGameObject::InitializeAABBLine()
{
	if (m_AABBLine != nullptr)
	{
		m_AABBLine->DecrementReferenceCount();
		m_AABBLine = nullptr;
	}

	// vertex data
	constexpr uint32_t vertexCount = 24;
	Graphics::VertexFormats::sVertex_line vertexData[vertexCount];
	{
		Math::sVector modelPos = m_rigidBody.position;

		// 0-1
		vertexData[0] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_min.z);
		vertexData[1] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_min.z);
		// 1-2
		vertexData[2] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_min.z);
		vertexData[3] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_max.z);
		// 2-3
		vertexData[4] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_max.z);
		vertexData[5] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_max.z);
		// 3-0
		vertexData[6] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_max.z);
		vertexData[7] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_min.z);
		// 4-5
		vertexData[8] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_min.z);
		vertexData[9] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_min.z);
		// 5-6
		vertexData[10] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_min.z);
		vertexData[11] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_max.z);
		// 6-7
		vertexData[12] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_max.z);
		vertexData[13] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_max.z);
		// 7-4
		vertexData[14] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_max.z);
		vertexData[15] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_min.z);
		// 1-5
		vertexData[16] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_min.z);
		vertexData[17] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_min.z);
		// 2-6
		vertexData[18] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_max.z);
		vertexData[19] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_max.y, modelPos.z + m_AABB.m_max.z);
		// 0-4
		vertexData[20] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_min.z);
		vertexData[21] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_min.z);
		// 3-7
		vertexData[22] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_min.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_max.z);
		vertexData[23] = Graphics::VertexFormats::sVertex_line(modelPos.x + m_AABB.m_max.x, modelPos.y + m_AABB.m_min.y, modelPos.z + m_AABB.m_max.z);
	}

	// index data
	constexpr uint32_t indexCount = vertexCount;
	uint16_t indexData[indexCount];
	{
		for (uint32_t i = 0; i < indexCount; i++)
		{
			indexData[i] = i;
		}
	}

	Graphics::cLine::Create(
		m_AABBLine,
		vertexData, vertexCount,
		indexData, indexCount);
}


void eae6320::cGameObject::InitializeCollider(const Physics::sColliderSetting& i_builder)
{
	Physics::cCollider::Create(m_collider, i_builder);
}


eae6320::Graphics::cLine* eae6320::cGameObject::GetAABBLine() const
{
	return m_AABBLine;
}


eae6320::Physics::cAABBCollider& eae6320::cGameObject::GetAABBCollider()
{
	return m_AABB;
}


eae6320::Physics::cCollider* eae6320::cGameObject::GetCollider() const
{
	return m_collider;
}

