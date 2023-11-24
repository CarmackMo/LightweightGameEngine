#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>
#include <Engine/UserInput/UserInput.h>

// TODO
#include <Engine/Graphics/cLine.h>



namespace ScrollShooterGame
{

	class cEnemy : public eae6320::cGameObject
	{

	public:

		void Initialize(
			eae6320::Math::sVector i_position,
			eae6320::Math::sVector i_velocity);

		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;



		void CleanUp() override;


	public:

		std::function<void()> m_cleanUpCallback = nullptr;

		int enemyData = 0;

		// TODO

		bool m_isCollide = false;
		eae6320::Graphics::cLine* m_colliderLine = nullptr;
		eae6320::Graphics::cLine* m_collisionLine = nullptr;


		void UpdateBasedOnInput() override;

		void InitializeColliderLine();

		eae6320::Graphics::cLine* GetColliderLine() const;

		void SetIsCollide(bool isCollide)
		{
			m_isCollide = isCollide;
		}



	};

}