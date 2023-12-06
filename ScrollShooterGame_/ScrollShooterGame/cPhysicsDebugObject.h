#pragma once

// Includes
//=========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Graphics/cLine.h>
#include <Engine/UserInput/UserInput.h>

#include <memory>


namespace ScrollShooterGame
{

	class cPhysicDebugObject : public eae6320::cGameObject
	{
	public:

		void CleanUp() override;

		void UpdateBasedOnInput() override;

		void InitializeColliderLine();

		std::weak_ptr<eae6320::Graphics::cLine> GetColliderLine() const;

		void SetIsCollide(bool isCollide)
		{
			m_isCollide = isCollide;
		}



		// Data
		//=========================

	private:

		// TODO: Temporary code for collider

		bool m_isCollide = false;

		std::shared_ptr<eae6320::Graphics::cLine> m_colliderLine;

		std::shared_ptr<eae6320::Graphics::cLine> m_collisionLine;

	};

}