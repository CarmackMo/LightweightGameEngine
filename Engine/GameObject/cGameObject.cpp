// Includes
//=========

#include <Engine/Concurrency/cEvent.h>
#include <Engine/GameObject/cGameObject.h>
#include <Engine/Graphics/Graphics.h>

#include <vector>
#include <functional>


void eae6320::cGameObject::CleanUp()
{
	if (m_mesh != nullptr) { m_mesh->DecrementReferenceCount(); }
	if (m_effect != nullptr) { m_effect->DecrementReferenceCount(); }
	if (m_collider != nullptr) { delete m_collider; }
}


void eae6320::cGameObject::InitializeMesh(const std::string& i_meshPath)
{
	if (m_mesh != nullptr)
	{
		m_mesh->DecrementReferenceCount();
		m_mesh = nullptr;
	}


	if (Graphics::AcquireRenderObjectInitMutex(5000) == WAIT_OBJECT_0)
	{
		Graphics::cMesh** mesh = &m_mesh;
		Concurrency::cEvent initEvent;
		initEvent.Initialize(Concurrency::EventType::RemainSignaledUntilReset);

		auto callback = [&initEvent, mesh](Graphics::cMesh* i_mesh) -> void
		{
			*mesh = i_mesh;
			initEvent.Signal();
			return;
		};


		Graphics::AddMeshInitializeTask(callback, i_meshPath);
		Graphics::ReleaseRenderObjectInitMutex();

		if (Concurrency::WaitForEvent(initEvent), 5000)
		{
			auto temp = 0;
		}
	}


	//Graphics::cMesh::Create(m_mesh, i_meshPath);
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


void eae6320::cGameObject::InitializeCollider(const Physics::sColliderSetting& i_builder)
{
	Physics::cCollider::Create(m_collider, i_builder, &m_rigidBody);
}


eae6320::Graphics::cMesh* eae6320::cGameObject::GetMesh() const
{
	return m_mesh;
}


eae6320::Graphics::cEffect* eae6320::cGameObject::GetEffect() const
{
	return m_effect;
}


eae6320::Physics::sRigidBodyState& eae6320::cGameObject::GetRigidBody()
{
	return m_rigidBody;
}


eae6320::Physics::cCollider* eae6320::cGameObject::GetCollider() const
{
	return m_collider;
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
}


void eae6320::cGameObject::UpdateBasedOnInput()
{ }