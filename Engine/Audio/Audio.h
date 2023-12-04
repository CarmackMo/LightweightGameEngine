#pragma once

// Includes
//=========

#include <Engine/Results/Results.h>


// Interface
//==========

namespace eae6320
{
namespace Audio
{

	cResult Initialize();

	cResult CleanUp();

	void Play(const char* fileName);


}// Namespace Audio
}// Namespace eae6320