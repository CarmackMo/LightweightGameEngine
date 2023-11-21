#pragma once

// Includes
//========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/UserInput/UserInput.h>


// TODO
#include <functional>


namespace ScrollShooterGame 
{

	class cPlayer : public eae6320::cGameObject
	{

	public:

		void UpdateBasedOnInput() override;


	public:

		std::function<void()> bulletCreation = nullptr;


	private:


	};

} 