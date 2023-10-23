/*
	A thread executes code sequentially,
	and using more than one thread in a program allows
	more than one task to execute concurrently.
*/

#ifndef EAE6320_CONCURRENCY_CTHREAD_H
#define EAE6320_CONCURRENCY_CTHREAD_H

// Includes
//=========

#include "Constants.h"

#include <Engine/Results/Results.h>
#include <functional>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

// Class Declaration
//==================

namespace eae6320
{
	namespace Concurrency
	{
		// A thread executes a single user-provided function,
		// and returns (stops executing) as soon as that user-provided function returns.
		// A user-provided function takes a single void pointer as input,
		// which allows shared data to be passed to the new thread.
		using fThreadFunction = std::function<void(void* const)>;

		class cThread
		{
			// Interface
			//==========

		public:

			// Calling this function will cause the specified function to be called in a new thread with the specified user data as input
			cResult Start( fThreadFunction const i_threadFunction, void* const io_userData = nullptr );

			// This function will return as soon as any one of the following happens:
			//	* The thread stops
			//	* The thread has already stopped
			//		* If the thread has already stopped when the function is called then it will return immediately
			//	* The specified time-out period elapses
			//		* If the caller doesn't specify a time-out period then the function will never return until the thread stops
			//		* If the caller specifies a time-out period of zero then the function will return immediately
			friend cResult WaitForThreadToStop( cThread& io_thread, const unsigned int i_timeToWait_inMilliseconds = Constants::DontTimeOut );

			// Initialization / Clean Up
			//--------------------------

			cThread();
			~cThread();

			// Data
			//=====

		private:

#if defined( EAE6320_PLATFORM_WINDOWS )
			HANDLE m_handle = NULL;
#endif

			// Implementation
			//===============

			// Initialization / Clean Up
			//--------------------------

			cResult CleanUp();

			cThread( const cThread& ) = delete;
			cThread( cThread&& ) = delete;
			cThread& operator =( const cThread& ) = delete;
			cThread& operator =( cThread&& ) = delete;
		};

		// Friends
		//========

		cResult WaitForThreadToStop( cThread& io_thread, const unsigned int i_timeToWait_inMilliseconds );
	}
}

#endif	// EAE6320_CONCURRENCY_CTHREAD_H
