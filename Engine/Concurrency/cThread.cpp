// Includes
//=========

#include "cThread.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Initialize / Clean Up
//----------------------

eae6320::Concurrency::cThread::~cThread()
{
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}
