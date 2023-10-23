// Includes
//=========

#include "../UserOutput.h"

#include <cstdarg>
#include <cstdio>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Windows/Functions.h>
#include <sstream>
#include <string>

// Static Data
//============

namespace
{
	HWND s_mainWindow = NULL;
}

// Interface
//==========

void eae6320::UserOutput::Print( const char* const i_message, ... )
{
	std::string message;
	{
		constexpr size_t bufferSize = 512;
		char buffer[bufferSize];
		int result_format;
		{
			va_list insertions;
			va_start( insertions, i_message );
			result_format = vsnprintf( buffer, bufferSize, i_message, insertions );
			va_end( insertions );
		}
		if ( result_format >= 0 )
		{
			if ( result_format < bufferSize )
			{
				message = buffer;
			}
			else
			{
				EAE6320_ASSERTF( false, "The internal UserOutput buffer of size %u was not big enough to hold the formatted message of length %i",
					bufferSize, result_format + 1 );
				std::ostringstream errorMessage;
				errorMessage << "FORMATTING ERROR! (The internal user output buffer of size " << bufferSize
					<< " was not big enough to hold the formatted message of length " << ( result_format + 1 ) << ".)"
					" Cut-off message is:\n\t" << buffer;
				message = errorMessage.str();
			}
		}
		else
		{
			EAE6320_ASSERTF( false, "An encoding error occurred in UserOutput for the message \"%s\"", i_message );
			std::ostringstream errorMessage;
			errorMessage << "ENCODING ERROR! Unformatted message was:\n\t" << i_message;
			message = errorMessage.str();
		}
	}

	// If UserOutput doesn't get initialized a NULL will be passed as the HWND to MessageBox(),
	// and it will still work but just not be a child of any window
	constexpr auto* const caption = "Message to User";
	MessageBoxW( s_mainWindow, Windows::ConvertUtf8ToUtf16( message.c_str() ).c_str(),
		Windows::ConvertUtf8ToUtf16( caption ).c_str(), MB_OK | MB_ICONINFORMATION );
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::UserOutput::Initialize( const sInitializationParameters& i_initializationParameters )
{
	s_mainWindow = i_initializationParameters.mainWindow;

	return Results::Success;
}

eae6320::cResult eae6320::UserOutput::CleanUp()
{
	s_mainWindow = NULL;

	return Results::Success;
}
