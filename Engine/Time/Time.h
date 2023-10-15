/*
	This file manages time-related functionality
*/

#ifndef EAE6320_TIME_H
#define EAE6320_TIME_H

// Includes
//=========

#include <cstdint>
#include <Engine/Results/Results.h>

// Interface
//==========

namespace eae6320
{
	namespace Time
	{
		// Time
		//-----

		uint64_t GetCurrentSystemTimeTickCount();

		double ConvertTicksToSeconds( const uint64_t i_tickCount );
		uint64_t ConvertSecondsToTicks( const double i_secondCount );
		double ConvertRatePerSecondToRatePerTick( const double i_rate_perSecond );

		// Initialize / Clean Up
		//----------------------

		cResult Initialize();
		cResult CleanUp();
	}
}

#endif	// EAE6320_TIME_H
