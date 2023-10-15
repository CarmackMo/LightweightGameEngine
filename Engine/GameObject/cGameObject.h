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


namespace eae6320
{

	class cGameObject
	{

	// Interface
	//=========================

	public:
		
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

		Physics::sRigidBodyState& GetRigidBody();

		Graphics::cMesh* GetMesh() const;

		Graphics::cEffect* GetEffect() const;

		Math::cMatrix_transformation GetCurrentTransform() const;

		Math::cMatrix_transformation GetPredictedTransform(const float i_secondCountToExtrapolate) const;
		
		virtual void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate);

		virtual void UpdateBasedOnInput();


		// Data
		//=========================

	protected:

		Physics::sRigidBodyState m_rigidBody;

		Graphics::cMesh* m_mesh = nullptr;
		Graphics::cEffect* m_effect = nullptr;

	};

}