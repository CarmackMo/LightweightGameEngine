#pragma once

// Includes
//========

#include <Engine/Math/sVector.h>

#include <ScrollShooterGame_/ScrollShooterGame/cEnemy.h>

namespace ScrollShooterGame
{

	class cEnemy_Alien : public cEnemy
	{

		// Interface
		//=========================

	public:

		// Initialization / Clean Up
		//--------------------------

		void Initialize(
			eae6320::Math::sVector i_position = eae6320::Math::sVector(),
			eae6320::Math::sVector i_velocity = eae6320::Math::sVector());

		void CleanUp() override;

		// Update
		//--------------------------

		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;


		// Implementation
		//=========================

	private:

		void ShootBullet();


		// Data
		//=========================

	private:

		float m_Boundary = 4.0f;

		double m_shootInterval = 2.5;

		double m_lastShoot_second = 0;

		eae6320::Math::sVector m_velocity = eae6320::Math::sVector(-1.0f, -0.25f, 0.0f);
	};

}