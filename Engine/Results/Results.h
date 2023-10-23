/*
	This file defines general results that any system can use
*/

#ifndef EAE6320_RESULTS_H
#define EAE6320_RESULTS_H

// Includes
//=========

#include "cResult.h"

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		// General non-specific success/failure results
		constexpr cResult Success( IsSuccess, eSystem::General, __LINE__, Severity::Success );
		constexpr cResult Failure( IsFailure, eSystem::General, __LINE__ );

		// This is returned when something is wrong with a file
		// (the file exists, but there is something wrong with its contents e.g. it is formatted incorrectly)
		constexpr cResult InvalidFile( IsFailure, eSystem::General, __LINE__ );
		// This is returned when a required file doesn't exist
		constexpr cResult FileDoesntExist( IsFailure, eSystem::General, __LINE__ );
		// This is returned when something fails because not enough memory was available
		constexpr cResult OutOfMemory( IsFailure, eSystem::General, __LINE__ );
		// This is returned when something fails because some context-specific amount of time passed
		// (e.g. the task might have succeeded if more time had been spent,
		// the task might succeed if tried again at a later time, etc.)
		constexpr cResult TimeOut( IsFailure, eSystem::General, __LINE__, Severity::Warning );

		// This can be compared against if the caller wants to know if no value was assigned
		constexpr cResult Undefined;
	}
}

#endif	// EAE6320_RESULTS_H
