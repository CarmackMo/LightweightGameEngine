// Includes
//=========

#include "../cThread.h"

#include "../cEvent.h"

#include <cstdlib>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Windows/Functions.h>
#include <process.h>

// Interface
//==========

eae6320::cResult eae6320::Concurrency::cThread::Start( fThreadFunction const i_threadFunction, void* const io_userData )
{
	auto result = Results::Success;

	if ( !m_handle )
	{
		// Windows requires a different function signature for its thread functions,
		// and so the user-provided data is passed to a generic Windows-appropriate function
		struct sThreadData
		{
			fThreadFunction const threadFunction;
			void* const userData;
			cEvent whenThreadDataHasBeenExtracted;
		} threadData = { i_threadFunction, io_userData };
		if ( result = threadData.whenThreadDataHasBeenExtracted.Initialize( EventType::RemainSignaledUntilReset ) )
		{
			// Start the new thread
			{
				constexpr SECURITY_ATTRIBUTES* const useDefaultSecurityAttributes = nullptr;
				constexpr unsigned int useDefaultStackSize = 0;
				constexpr unsigned int startExecutingImmediately = 0;
				unsigned int threadId;
				// Reset the global error variable before calling the next function
				// so that if starting the thread fails
				// the error reported isn't left over from an earlier unrelated error
				{
					const auto result_errorno = _set_doserrno( ERROR_SUCCESS );
					EAE6320_ASSERT( result_errorno == 0 );
				}
				m_handle = reinterpret_cast<HANDLE>( _beginthreadex( useDefaultSecurityAttributes, useDefaultStackSize,
					[]( void* io_threadData ) -> unsigned int
					{
						// Extract the user-provided data
						auto* threadData = static_cast<sThreadData*>( io_threadData );
						auto const threadFunction = threadData->threadFunction;
						auto* const userData = threadData->userData;
						// Signal that the data is extracted
						// (which means that the sThreadData struct can go away)
						const auto result = threadData->whenThreadDataHasBeenExtracted.Signal();
						if ( result )
						{
							threadData = nullptr;

							// Call the user-provided function with the user-provided data
							threadFunction( userData );
						}
						else
						{
							// This is bad, and will cause the calling thread to forcibly terminate this new one
							// after the wait for the event times out.
							// This thread will exit right now,
							// hopefully before the time out period elapses (if the assert window isn't left up for too long),
							// so that the forced termination won't leave anything in a bad state.
							EAE6320_ASSERTF( false, "Couldn't signal that a new thread's data was extracted" );
							Logging::OutputError( "Windows failed to signal that a new thread had extracted its thread data" );
						}
						// When the user-provided function returns the thread can exit.
						// Explicitly calling the following function isn't necessary
						// (Windows would do it automatically when this function exits),
						// but the documentation says it "helps ensure correct recovery of resources"
						_endthreadex( result );
						// This code won't be reached because _endthreadex() exits the thread
						return result;
					},
					&threadData, startExecutingImmediately, &threadId ) );
				if ( !m_handle )
				{
					result = Results::Failure;
					DWORD errorCode;
					if ( _get_doserrno( &errorCode ) == 0 )
					{
						const auto errorMessage = Windows::GetFormattedSystemMessage( errorCode );
						EAE6320_ASSERTF( false, "Couldn't start a thread: %s", errorMessage.c_str() );
						Logging::OutputError( "Windows failed to start a thread: %s", errorMessage.c_str() );
					}
					else
					{
						EAE6320_ASSERTF( false, "Couldn't start a thread" );
						Logging::OutputError( "Windows failed to start a thread for an unknown reason" );
					}
					return result;
				}
			}
			// The new thread needs to access the threadData variable that is local to this calling function,
			// and so this calling function must wait to let threadData go out of scope
			// until the new thread has extracted all of the information from it
			{
				constexpr unsigned int timeToWait_inMilliseconds = 15 * 1000;
				if ( !( result = WaitForEvent( threadData.whenThreadDataHasBeenExtracted ) ) )
				{
					// The new thread must be forcibly terminated to prevent crashes from illegal memory use,
					// but doing this can also potentially put the program in a bad state
					EAE6320_ASSERTF( false, "Couldn't wait for a new thread to extract thread data" );
					Logging::OutputError( "Windows failed to wait for a new thread to extract thread data,"
						" and so the new thread must be forcibly terminated" );
					UserOutput::Print( "Something unexpected went wrong when creating a new thread, "
						" and the application is now in an unstable state and may crash or show unpredictable behavior." );
					if ( TerminateThread( m_handle, result ) == FALSE )
					{
						const auto errorMessage = eae6320::Windows::GetLastSystemError();
						EAE6320_ASSERTF( false, "Couldn't forcibly terminate a thread: %s", errorMessage.c_str() );
						Logging::OutputError( "Windows failed to forcibly terminate the new thread: %s", errorMessage.c_str() );
					}
					return result;
				}
			}
		}
		else
		{
			EAE6320_ASSERTF( false, "A thread can't be started with no thread data extraction event" );
			Logging::OutputError( "A thread couldn't be started because Windows failed to create a thread data extraction event" );
			return result;
		}
	}
	else
	{
		result = Results::Failure;
		EAE6320_ASSERTF( false, "A thread can't be started if it is already running" );
		eae6320::Logging::OutputError( "An attempt was made to start a thread that was already running" );
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Concurrency::WaitForThreadToStop( cThread& io_thread, const unsigned int i_timeToWait_inMilliseconds )
{
	if ( io_thread.m_handle )
	{
		const auto result = WaitForSingleObject( io_thread.m_handle,
			( i_timeToWait_inMilliseconds == eae6320::Concurrency::Constants::DontTimeOut ) ? INFINITE : static_cast<DWORD>( i_timeToWait_inMilliseconds ) );
		switch ( result )
		{
		// The thread exited
		case WAIT_OBJECT_0:
			// CleanUp() is called to close the handle so that this thread object could be reused if desired
			return io_thread.CleanUp();
		// The time-out period elapsed before the thread exited
		case WAIT_TIMEOUT:
			return eae6320::Results::TimeOut;
		// A Windows error prevented the wait
		case WAIT_FAILED:
			{
				const auto errorMessage = eae6320::Windows::GetLastSystemError();
				EAE6320_ASSERTF( false, "Failed to wait for a thread to exit: %s", errorMessage.c_str() );
				eae6320::Logging::OutputError( "Windows failed waiting for a thread to exit: %s", errorMessage.c_str() );
			}
			break;
		// An unexpected error occurred
		default:
			EAE6320_ASSERTF( false, "Failed to wait for a thread to exit" );
			eae6320::Logging::OutputError( "Windows failed waiting for a thread to exit due to an unknown reason (this should never happen)" );
		}
		return eae6320::Results::Failure;
	}
	else
	{
		EAE6320_ASSERTF( false, "A thread can't be waited on to exit if it hasn't been started" );
		// Even calling the function with a NULL handle is probably a user error,
		// the thread isn't running (assuming the user didn't call CleanUp() prematurely)
		// and so success is returned
		return eae6320::Results::Success;
	}
}

// Initialize / Clean Up
//----------------------

eae6320::Concurrency::cThread::cThread()
{

}

// Implementation
//===============

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Concurrency::cThread::CleanUp()
{
	cResult result = eae6320::Results::Success;

	if ( m_handle )
	{
		if ( CloseHandle( m_handle ) == FALSE )
		{
			{
				const auto errorMessage = Windows::GetLastSystemError();
				EAE6320_ASSERTF( false, "Couldn't close thread handle: %s", errorMessage.c_str() );
				Logging::OutputError( "Windows failed to close a thread handle: %s", errorMessage.c_str() );
			}
			if ( result )
			{
				result = eae6320::Results::Failure;
			}
		}
		m_handle = NULL;
	}

	return result;
}
