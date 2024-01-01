#pragma once

// Includes
//=========

#include <Engine/Results/Results.h>

#include <Windows.h>


// Interface
//==========

namespace eae6320
{
namespace Network
{

	cResult Initialize(HINSTANCE i_hInstance);

	cResult CleanUp();

	cResult Connect();
	

}
}