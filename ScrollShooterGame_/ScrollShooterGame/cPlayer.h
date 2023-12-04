#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>

// TODO
#include <Engine/Graphics/cLine.h>



namespace ScrollShooterGame 
{

	class cPlayer : public eae6320::cGameObject
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

		void UpdateBasedOnInput() override;


		// Implementation
		//=========================

	private:
		
		void ShootBullet();


		// Data
		//=========================

	private:

		uint16_t m_HP;

		double m_shootCoolDown;
		double m_lastShoot_second;

		eae6320::Math::sVector m_maxBoundary;
		eae6320::Math::sVector m_minBoundary;


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