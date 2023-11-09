// Includes
//=========

#include <Engine/GameObject/cGameObject.h>

#include <vector>


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


	// TODO: Temporary code for collider debug

	if (m_colliderLine != nullptr) { m_colliderLine->DecrementReferenceCount(); }
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


	// TODO: temporary code for collider debug

	if (m_collider != nullptr)
	{
		m_collider->Update(m_rigidBody);
	}
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


void eae6320::cGameObject::InitializeColliderLine()
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

	Graphics::VertexFormats::sVertex_line* vertexData = new Graphics::VertexFormats::sVertex_line[vertexCount];
	for (uint32_t i = 0; i < vertexCount; i++)
	{
		vertexData[i] = Graphics::VertexFormats::sVertex_line(vertexVec[i].x, vertexVec[i].y, vertexVec[i].z);
	}
	uint16_t* indexData = new uint16_t[indexCount];
	for (uint32_t i = 0; i < indexCount; i++)
	{
		indexData[i] = indexVec[i];
	}

	Graphics::cLine::Create(
		m_colliderLine,
		vertexData, vertexCount,
		indexData, indexCount);

	delete[] vertexData;
	delete[] indexData;
}


void eae6320::cGameObject::InitializeCollider(const Physics::sColliderSetting& i_builder)
{
	Physics::cCollider::Create(m_collider, i_builder, m_rigidBody);
}


eae6320::Graphics::cLine* eae6320::cGameObject::GetColliderLine() const
{
	return m_colliderLine;
}


eae6320::Physics::cCollider* eae6320::cGameObject::GetCollider() const
{
	return m_collider;
}

