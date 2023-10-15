/*
	This class allows a thread to signal to other threads when an event has happened
	and for those other threads to wait for the event to happen

	An event can be either "unsignaled" or "signaled":
		* When an event is signaled it means that the event has happened
		* (Note that an event's state can change from signaled back to unsignaled;
			this happens when an event repeats)
*/

#ifndef EAE6320_CONCURRENCY_CEVENT_H
#define EAE6320_CONCURRENCY_CEVENT_H

// Includes
//=========

#include "Constants.h"

#include <Engine/Results/Results.h>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif

// Constants
//==========

namespace eae6320
{
	namespace Concurrency
	{
		enum class EventType
		{
			ResetAutomaticallyAfterBeingSignaled,
			RemainSignaledUntilReset
		};
		enum class EventState
		{
			Unsignaled,
			Signaled
		};
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Concurrency
	{
		class cEvent
		{
			// Interface
			//==========

		public:

			// This function will return as soon as any one of the following happens:
			//	* The event happens
			//		* More specifically, the event becomes signaled (because a different thread calls cEvent::Signal())
			//		* If multiple threads are waiting on the same automatically-resetting event
			//			then only one of them will return from this function when the event is signaled
			//			and the rest will keep waiting
			//	* The event has already happened
			//		* If the event is already signaled when the function is called then it will return immediately
			//	* The specified time-out period elapses
			//		* If the caller doesn't specify a time-out period then the function will never return until the event happens
			//		* If the caller specifies a time-out period of zero then the function will return immediately
			friend cResult WaitForEvent( const cEvent& i_event, const unsigned int i_timeToWait_inMilliseconds = Constants::DontTimeOut );

			// This function should be called when an event happens
			// (which "signals" the event happening to any waiting threads)
			cResult Signal();
			// Calling this function sets the state of the event to unsignaled
			// (it resets the event as if it had never happened)
			cResult ResetToUnsignaled();

			// Initialization / Clean Up
			//--------------------------

			cResult Initialize( const EventType i_type, const EventState i_initialState = EventState::Unsignaled );
			cResult CleanUp();

			cEvent();
			~cEvent();

			// Data
			//=====

		private:

#if defined( EAE6320_PLATFORM_WINDOWS )
			HANDLE m_handle = NULL;
#endif

			// Implementation
			//---------------

		private:

			// Initialization / Clean Up
			//--------------------------

			cEvent( const cEvent& ) = delete;
			cEvent( cEvent&& ) = delete;
			cEvent& operator =( const cEvent& ) = delete;
			cEvent& operator =( cEvent&& ) = delete;
		};

		// Friends
		//========

		cResult WaitForEvent( const cEvent& i_event, const unsigned int i_timeToWait_inMilliseconds );
	}
}

#endif	// EAE6320_CONCURRENCY_CEVENT_H
