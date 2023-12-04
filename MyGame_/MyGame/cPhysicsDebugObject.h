#pragma once

// Includes
//=========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/Graphics/cLine.h>
#include <Engine/UserInput/UserInput.h>


namespace eae6320
{

	class cPhysicDebugObject : public cGameObject
	{
	public:

		void CleanUp() override;

		void UpdateBasedOnInput() override;

		void InitializeColliderLine();

		Graphics::cLine* GetColliderLine() const;

		void SetIsCollide(bool isCollide)
		{
			m_isCollide = isCollide;
		}



		// Data
		//=========================

	private:

		// TODO: Temporary code for collider

		bool m_isCollide = false;

		Graphics::cLine* m_colliderLine = nullptr;

		Graphics::cLine* m_collisionLine = nullptr;

	};

}