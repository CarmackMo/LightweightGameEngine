// Includes
//=========

#include "Asserts.h"

#ifdef EAE6320_ASSERTS_AREENABLED
	#include <cstdarg>
	#include <cstdio>
#endif

// Helper Definitions
//===================

#ifdef EAE6320_ASSERTS_AREENABLED

bool eae6320::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak( const unsigned int i_lineNumber, const char* const i_file,
	bool& io_shouldThisAssertBeIgnoredInTheFuture, const char* const i_message, ... )
{
	// Construct the message
	std::ostringstream message;
	{
		message << "An assertion failed on line " << i_lineNumber << " of " << i_file;
		constexpr size_t bufferSize = 512;
		char buffer[bufferSize];
		int result_format;
		{
			va_list insertions;
			va_start( insertions, i_message );
			result_format = vsnprintf( buffer, bufferSize, i_message, insertions );
			va_end( insertions );
		}
		if ( result_format != 0 )
		{
			message << ":\n\n";
			if ( result_format > 0 )
			{
				message << buffer;
				if ( result_format >= bufferSize )
				{
					message << "\n\n"
						"(The internal buffer of size " << bufferSize
						<< " was not big enough to hold the formatted message of length " << ( result_format + 1 ) << ")";
				}
			}
			else
			{
				message << "An encoding error occurred! The unformatted message is: \"" << i_message << "\"!";
			}
		}
		else
		{
			message << "!";
		}
	}
	// Display it and break if necessary
	return ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak_platformSpecific( message, io_shouldThisAssertBeIgnoredInTheFuture );
}

#endif	// EAE6320_ASSERTS_AREENABLED
