#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>

// TODO
#include <Engine/Graphics/cLine.h>


namespace ScrollShooterGame
{
	class cEnemyGenerator : public eae6320::cGameObject
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

		void SpawnRock();

		void SpawnAlien();


		// Data
		//=========================

	private:

		float m_width = 10.0f;

		double m_lastSpawnTime_rock = 0.0;
		double m_lastSpawnTime_alien = 0.0;
		double m_spawnCoolDown_rock = 0.0;
		double m_spawnCoolDown_alien = 0.0;
		eae6320::Math::sVector m_spawnCoolDownRange_rock = eae6320::Math::sVector(1.0f, 2.0f, 0.0f);
		eae6320::Math::sVector m_spawnCoolDownRange_alien = eae6320::Math::sVector(7.0f, 13.0f, 0.0f);

		eae6320::Math::sVector m_enemySpeedRange_rock = eae6320::Math::sVector(-2.0f, -1.0f, 0.0f);


		// TODO: Debug
		//=========================

	public:

		bool m_isCollide = false;
		eae6320::Graphics::cLine* m_colliderLine = nullptr;
		eae6320::Graphics::cLine* m_collisionLine = nullptr;

		void InitializeColliderLine();

		void SetIsCollide(bool isCollide)
		{
			m_isCollide = isCollide;
		}

		eae6320::Graphics::cLine* GetColliderLine() const;


	};


}


