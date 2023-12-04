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

	//void Play(const char* fileName);

	cResult Play(const char* i_audioId);

}// Namespace Audio
}// Namespace eae6320