// Includes
//=========

#include "../Asserts.h"

#ifdef EAE6320_ASSERTS_AREENABLED
	#include <Engine/Windows/Includes.h>
#endif

// Helper Definitions
//===================

#ifdef EAE6320_ASSERTS_AREENABLED

bool eae6320::Asserts::ShowMessageIfAssertionIsFalseAndReturnWhetherToBreak_platformSpecific(
	std::ostringstream& io_message, bool& io_shouldThisAssertBeIgnoredInTheFuture )
{
#ifdef EAE6320_ASSERTS_SHOULDPRINTTODEBUGGER
	OutputDebugStringA( io_message.str().c_str() );
#endif

	io_message << "\n\n"
		"Do you want to break into the debugger?"
		" Choose \"Yes\" to break, \"No\" to continue, or \"Cancel\" to disable this assertion until the program exits.";
	const auto result = MessageBoxA( GetActiveWindow(), io_message.str().c_str(), "Assertion Failed!", MB_YESNOCANCEL );
	if ( ( result == IDYES )
		// MessageBox() returns 0 on failure; if this happens the code breaks rather than trying to diagnose why
		|| ( result == 0 ) )
	{
		return true;
	}
	else
	{
		if ( result == IDCANCEL )
		{
			io_shouldThisAssertBeIgnoredInTheFuture = true;
		}
		return false;
	}
}

#endif	// EAE6320_ASSERTS_AREENABLED
