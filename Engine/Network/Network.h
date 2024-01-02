#pragma once

// Includes
//=========

#include <Engine/Results/Results.h>

#include <Windows.h>



// Collision Type Bits
//==================

namespace eae6320
{
namespace Network
{

	// The values below are used as bit masks
	enum eNetworkType : uint8_t
	{
		None = 0,

		Server = 1,

		Client = 2,
	};

}// Namespace Network
}// Namespace eae6320


// Interface
//==========

namespace eae6320
{
namespace Network
{

	cResult Initialize(HINSTANCE i_hInstance);

	cResult CleanUp();

	cResult Connect();
	
	cResult SendData(const char* i_sendBuffer);

	cResult ReceiveData(char* i_receiveBuffer);

	eNetworkType GetNetworkTypeOfThisComputer();
}
}