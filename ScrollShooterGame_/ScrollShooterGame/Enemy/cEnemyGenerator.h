#pragma once

// Includes
//========

#include <Engine/Math/sVector.h>
#include <Engine/GameObject/cGameObject.h>



namespace ScrollShooterGame
{
	class cEnemyGenerator : public eae6320::cGameObject
	{

		// Interface
		//=========================

	public:

		// Initialization / Clean Up
		//--------------------------

		virtual void Initialize(
			eae6320::Math::sVector i_position = eae6320::Math::sVector(),
			eae6320::Math::sVector i_velocity = eae6320::Math::sVector());

		// Update
		//--------------------------

		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;


		// Data
		//=========================

	private:

	};


}


