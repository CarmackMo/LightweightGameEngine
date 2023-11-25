#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>

#include <functional>


// TODO
#include <Engine/Graphics/cLine.h>



namespace ScrollShooterGame
{

	class cBullet : public eae6320::cGameObject
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


		// TODO: Debug
		//=========================

	public: 

		//std::function<void()> m_cleanUpCallback = nullptr;
		
		bool m_isCollide = false;
		eae6320::Graphics::cLine* m_colliderLine = nullptr;
		eae6320::Graphics::cLine* m_collisionLine = nullptr;

		void SetIsCollide(bool isCollide)
		{
			m_isCollide = isCollide;
		}

		void InitializeColliderLine();

		eae6320::Graphics::cLine* GetColliderLine() const;

	};

}