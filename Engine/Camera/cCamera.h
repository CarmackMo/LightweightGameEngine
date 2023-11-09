#pragma once

// Includes
//=========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/sVector.h>
#include <Engine/Physics/cRigidBody.h>
#include <Engine/Results/Results.h>


namespace eae6320
{
namespace Camera
{

	class cCamera : public cGameObject
	{

	// Interface
	//=========================

	public:

		void Initialize(
			float i_posX, float i_posY, float i_posZ,
			float i_verticalFieldOfView_inRadians,
			float i_z_nearPlane,
			float i_z_farPlane);
		
		void UpdateBasedOnInput() override;

		// Create the world-to-camera transform matrix using elapsed simulation time
		Math::cMatrix_transformation CreateWorldToCameraMatrix(
			const float i_secondCountToExtrapolate);

		Math::cMatrix_transformation CreateCameraToProjectedMatrix(
			float windowWidth, 
			float windowHeight);


	// Data
	//=========================

	private:

		float m_verticalFieldOfView_inRadians;
		float m_z_nearPlane;
		float m_z_farPlane;

	};


}// Namespace Camera
}// eae6320