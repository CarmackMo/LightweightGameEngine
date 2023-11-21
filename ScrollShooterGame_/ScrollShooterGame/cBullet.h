#pragma once

#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>
#include <Engine/UserInput/UserInput.h>


namespace ScrollShooterGame
{

	class cBullet : public eae6320::cGameObject
	{

	public:

		void Initialize(
			eae6320::Math::sVector i_position, 
			eae6320::Math::sVector i_velocity);

		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;



	private:



	};

}