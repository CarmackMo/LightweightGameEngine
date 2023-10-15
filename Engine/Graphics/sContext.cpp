// Includes
//=========

#include "sContext.h"

#include <Engine/Asserts/Asserts.h>

// Static Data
//============

eae6320::Graphics::sContext eae6320::Graphics::sContext::g_context;

// Interface
//==========

// Initialize / Clean Up
//----------------------

eae6320::Graphics::sContext::~sContext()
{
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}
