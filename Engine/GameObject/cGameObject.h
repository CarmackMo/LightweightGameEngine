#pragma once

// Includes
//=========

#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/VertexFormats.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/UserInput/UserInput.h>

#include <string>


// TODO: Tempory code for collider
#include <Engine/Graphics/cLine.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>




namespace eae6320
{

	class cGameObject
	{

	// Interface
	//=========================

	public:
		
		// Initialization / Clean Up
		//--------------------------

		void InitializeMesh(
			Graphics::VertexFormats::sVertex_mesh* i_vertexData,
			const uint32_t i_vertexCount,
			uint16_t* i_indexData,
			const uint32_t i_indexCount,
			const uint32_t i_indexCountToRender,
			const uint32_t i_indexOfFirstIndexToUse = 0,
			const uint32_t i_offsetToAddToEachIndex = 0);

		void InitializeMesh(const std::string& i_meshPath);

		void InitializeEffect(
			const std::string& i_vertexShaderPath,
			const std::string& i_fragmentShaderPath);

		virtual void CleanUp();

		// Property Getters
		//--------------------------

		Physics::sRigidBodyState& GetRigidBody();

		Graphics::cMesh* GetMesh() const;

		Graphics::cEffect* GetEffect() const;

		Math::cMatrix_transformation GetCurrentTransform() const;

		Math::cMatrix_transformation GetPredictedTransform(const float i_secondCountToExtrapolate) const;
		
		// Update
		//--------------------------

		virtual void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate);

		virtual void UpdateBasedOnInput();




		// TODO: Tempory code for rendering collider box and debug collider
		void InitializeAABB(
			float x1, float y1, float z1,
			float x2, float y2, float z2);

		void InitializeAABBLine();

		void InitializeCollider(const Physics::sColliderSetting& i_builder);

		Graphics::cLine* GetAABBLine() const;

		Physics::cAABBCollider& GetAABBCollider();

		Physics::cCollider* GetCollider() const;





		// Data
		//=========================

	protected:

		Physics::sRigidBodyState m_rigidBody;

		Graphics::cMesh* m_mesh = nullptr;
		Graphics::cEffect* m_effect = nullptr;


		// TODO: Temporary code for collider
		Physics::cAABBCollider m_AABB;
		Graphics::cLine* m_AABBLine = nullptr;

		Physics::cCollider* m_collider = nullptr;
	};

}