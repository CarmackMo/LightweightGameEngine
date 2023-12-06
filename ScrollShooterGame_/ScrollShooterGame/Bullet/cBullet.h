#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Math/sVector.h>

// TODO
#include <Engine/Graphics/cLine.h>
#include <memory>



namespace ScrollShooterGame
{

	class cBullet : public eae6320::cGameObject
	{

		// Interface
		//=========================

	public:

		// Initialization / Clean Up
		//--------------------------

		virtual void Initialize(
			eae6320::Math::sVector i_position = eae6320::Math::sVector(),
			eae6320::Math::sVector i_velocity = eae6320::Math::sVector()) = 0;

		void CleanUp() override;

		// Update
		//--------------------------

		void UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) override;


		// Data
		//=========================

	private:

		float m_boundary = 15.0f;



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