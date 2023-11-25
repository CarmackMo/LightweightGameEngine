#pragma once

// Includes
//========

#include <Engine/Math/sVector.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.h>


namespace ScrollShooterGame
{

	class cBullet_Player : public cBullet
	{

		// Interface
		//=========================

	public:

		// Initialization / Clean Up
		//--------------------------

		void Initialize(
			eae6320::Math::sVector i_position = eae6320::Math::sVector(),
			eae6320::Math::sVector i_velocity = eae6320::Math::sVector()) override;

		void CleanUp() override;


		// Data
		//=========================

	private:

		eae6320::Math::sVector m_velocity = eae6320::Math::sVector(0, 1, 0);


	};

}