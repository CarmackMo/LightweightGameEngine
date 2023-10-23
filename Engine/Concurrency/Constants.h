/*
	This file contains concurrency-related constants
*/

#ifndef EAE6320_CONCURRENCY_CONSTANTS_H
#define EAE6320_CONCURRENCY_CONSTANTS_H

namespace eae6320
{
	namespace Concurrency
	{
		namespace Constants
		{
			constexpr auto DontTimeOut = ~unsigned int( 0u );
		}
	}
}

#endif	// EAE6320_CONCURRENCY_CONSTANTS_H
