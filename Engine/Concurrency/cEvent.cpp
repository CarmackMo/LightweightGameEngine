// Includes
//=========

#include "cEvent.h"

#include <Engine/Asserts/Asserts.h>

// Interface
//==========

// Initialize / Clean Up
//----------------------

eae6320::Concurrency::cEvent::~cEvent()
{
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}
