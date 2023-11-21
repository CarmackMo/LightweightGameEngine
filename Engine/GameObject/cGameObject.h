#pragma once

// Includes
//=========

#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Physics/cRigidBody.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/UserInput/UserInput.h>

#include <string>


namespace eae6320
{

	class cGameObject
	{

	// Interface
	//=========================

	public:
		
		// Initialization / Clean Up
		//--------------------------

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


		// Data
		//=========================

	protected:

		Graphics::cMesh* m_mesh = nullptr;
		Graphics::cEffect* m_effect = nullptr;

		Physics::sRigidBodyState m_rigidBody;
		Physics::cCollider* m_collider = nullptr;

	};

}