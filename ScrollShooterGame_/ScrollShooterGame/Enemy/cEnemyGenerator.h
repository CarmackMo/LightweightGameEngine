#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>

#include <memory>

// TODO
#include <Engine/Graphics/cLine.h>
#include <memory>


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

		float m_width = 0.0f;

		double m_lastSpawnTime_rock = 0.0;
		double m_lastSpawnTime_alien = 0.0;
		double m_spawnCoolDown_rock = 0.0;
		double m_spawnCoolDown_alien = 0.0;
		eae6320::Math::sVector m_spawnCoolDownRange_rock;
		eae6320::Math::sVector m_spawnCoolDownRange_alien;

		eae6320::Math::sVector m_enemySpeedRange_rock;


		// TODO: Debug
		//=========================

	public:

		bool m_isCollide = false;
		std::shared_ptr<eae6320::Graphics::cLine> m_colliderLine;
		std::shared_ptr<eae6320::Graphics::cLine> m_collisionLine;

		void SetIsCollide(bool isCollide);

		void InitializeColliderLine();

		std::weak_ptr<eae6320::Graphics::cLine> GetColliderLine() const;


	};


}


