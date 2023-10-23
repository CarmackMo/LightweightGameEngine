// Includes
//=========

#include "iBuilder.h"

#include "Functions.h"

#include <Engine/Asserts/Asserts.h>
#include <sstream>

// Interface
//==========

// Build
//------

eae6320::cResult eae6320::Assets::iBuilder::ParseCommandArgumentsAndBuild( char* const* i_arguments, const unsigned int i_argumentCount )
{
	constexpr unsigned int commandCount = 1;
	const auto actualArgumentCount = i_argumentCount - commandCount;
	constexpr unsigned int requiredArgumentCount = 2;
	if ( actualArgumentCount >= requiredArgumentCount )
	{
		EAE6320_ASSERT( i_arguments != nullptr );
		m_path_source = i_arguments[commandCount + 0];
		m_path_target = i_arguments[commandCount + 1];

		std::vector<std::string> optionalArguments;
		for ( auto i = ( commandCount + requiredArgumentCount ); i < i_argumentCount; ++i )
		{
			optionalArguments.push_back( i_arguments[i] );
		}
		return Build( optionalArguments );
	}
	else
	{
		{
			std::ostringstream errorMessage;
			errorMessage << "An asset builder must be called with at least " << requiredArgumentCount << " command line arguments "
				"(the source path and the target path), but ";
			if ( actualArgumentCount == 0 )
			{
				errorMessage << "none were provided\n";
			}
			else if ( actualArgumentCount == 1 )
			{
				errorMessage << "only 1 was provided\n";
			}
			else
			{
				errorMessage << "only " << actualArgumentCount << " were provided\n";
			}
			eae6320::Assets::OutputErrorMessage( errorMessage.str().c_str() );
		}
		return Results::Failure;
	}
}
