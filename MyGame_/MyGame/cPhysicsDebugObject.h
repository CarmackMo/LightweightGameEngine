#pragma once

// Includes
//=========

#include <Engine/GameObject/cGameObject.h>
#include <Engine/UserInput/UserInput.h>


namespace eae6320
{

	class cPhysicDebugObject : public cGameObject
	{


		void UpdateBasedOnInput() override;

	};

}