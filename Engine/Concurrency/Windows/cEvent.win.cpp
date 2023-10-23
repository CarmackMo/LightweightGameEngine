// Includes
//=========

#include "../cEvent.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Windows/Functions.h>

// Interface
//==========

eae6320::cResult eae6320::Concurrency::WaitForEvent( const eae6320::Concurrency::cEvent& i_event, const unsigned int i_timeToWait_inMilliseconds )
{
	if ( i_event.m_handle )
	{
		const auto result = WaitForSingleObject( i_event.m_handle,
			( i_timeToWait_inMilliseconds == eae6320::Concurrency::Constants::DontTimeOut ) ? INFINITE : static_cast<DWORD>( i_timeToWait_inMilliseconds ) );
		switch ( result )
		{
		// The event was signaled
		case WAIT_OBJECT_0:
			return eae6320::Results::Success;
		// The time-out period elapsed before the event was signaled
		case WAIT_TIMEOUT:
			return eae6320::Results::TimeOut;
		// A Windows error prevented the wait
		case WAIT_FAILED:
			{
				const auto errorMessage = eae6320::Windows::GetLastSystemError();
				EAE6320_ASSERTF( false, "Failed to wait for an event: %s", errorMessage.c_str() );
				eae6320::Logging::OutputError( "Windows failed waiting for an event: %s", errorMessage.c_str() );
			}
			break;
		// An unexpected error occurred
		default:
			EAE6320_ASSERTF( false, "Failed to wait for an event" );
			eae6320::Logging::OutputError( "Windows failed waiting for an event due to an unknown reason (this should never happen)" );
		}
		return eae6320::Results::Failure;
	}
	else
	{
		EAE6320_ASSERTF( false, "An event can't be waited for until it has been initialized" );
		eae6320::Logging::OutputError( "An attempt was made to wait for an event that hadn't been initialized" );
		return eae6320::Results::Failure;
	}
}

eae6320::cResult eae6320::Concurrency::cEvent::Signal()
{
	EAE6320_ASSERTF( m_handle, "An event can't be signaled until it has been initialized" );
	if ( SetEvent( m_handle ) != FALSE )
	{
		return Results::Success;
	}
	else
	{
		const auto errorMessage = Windows::GetLastSystemError();
		EAE6320_ASSERTF( false, "Couldn't signal event: %s", errorMessage.c_str() );
		Logging::OutputError( "Windows failed to signal an event: %s", errorMessage.c_str() );
		return Results::Failure;
	}
}

eae6320::cResult eae6320::Concurrency::cEvent::ResetToUnsignaled()
{
	EAE6320_ASSERTF( m_handle, "An event can't be reset until it has been initialized" );
	if ( ResetEvent( m_handle ) != FALSE )
	{
		return Results::Success;
	}
	else
	{
		const auto errorMessage = Windows::GetLastSystemError();
		EAE6320_ASSERTF( false, "Couldn't reset event to be unsignaled: %s", errorMessage.c_str() );
		Logging::OutputError( "Windows failed to reset an event to be unsignaled: %s", errorMessage.c_str() );
		return Results::Failure;
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Concurrency::cEvent::Initialize( const EventType i_type, const EventState i_initialState )
{
	constexpr SECURITY_ATTRIBUTES* const useDefaultSecurityAttributes = nullptr;
	constexpr wchar_t* const noName = nullptr;
	if ( m_handle = CreateEventW( useDefaultSecurityAttributes, i_type == EventType::RemainSignaledUntilReset, i_initialState == EventState::Signaled, noName ) )
	{
		return Results::Success;
	}
	else
	{
		const auto errorMessage = Windows::GetLastSystemError();
		EAE6320_ASSERTF( false, "Couldn't create event: %s", errorMessage.c_str() );
		Logging::OutputError( "Windows failed to create an event: %s", errorMessage.c_str() );
		return Results::Failure;
	}
}

eae6320::Concurrency::cEvent::cEvent()
{

}

eae6320::cResult eae6320::Concurrency::cEvent::CleanUp()
{
	auto result = Results::Success;

	if ( m_handle )
	{
		if ( CloseHandle( m_handle ) == FALSE )
		{
			const auto errorMessage = Windows::GetLastSystemError();
			EAE6320_ASSERTF( false, "Couldn't close event handle: %s", errorMessage.c_str() );
			Logging::OutputError( "Windows failed to close an event handle: %s", errorMessage.c_str() );
		}
		m_handle = NULL;
	}

	return result;
}
