#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>

#include <cstdint>

// TODO
#include <Engine/Graphics/cLine.h>



namespace ScrollShooterGame
{

	class cEnemy : public eae6320::cGameObject
	{

		// Interface
		//=========================

	public:

		// Initialization / Clean Up
		//--------------------------

		virtual void Initialize(
			eae6320::Math::sVector i_position = eae6320::Math::sVector(),
			eae6320::Math::sVector i_velocity = eae6320::Math::sVector()) = 0;

		// Property Getter
		//--------------------------

		uint16_t GetHP();


		// Data
		//=========================

	protected:

		uint16_t m_HP = 0;


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