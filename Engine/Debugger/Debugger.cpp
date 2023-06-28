#include <windows.h>
#include <stdio.h>
#include "Debugger.h"

namespace Engine
{
namespace Debugger
{

template <typename T>
void DebugManager::Print(const char* const text, T value)
{
	char p[100];
	sprintf_s(p, 50, text, value);
	OutputDebugStringA(p);

}


void DebugManager::ConsolePrint(const char* i_pFmt, const char* i_pFile, unsigned int i_Line, ...)
{
	const size_t		lenTemp = 128;
	char				strTemp[lenTemp] = "GLib: %s %u: ";

	strcat_s(strTemp, i_pFmt);

	const size_t		lenOutput = 256;
	char				strOutput[lenOutput];

	// define a variable argument list variable 
	va_list				args;

	// initialize it. second parameter is variable immediately
	// preceeding variable arguments
	va_start(args, i_pFmt);

	// (safely) print our formatted string to a char buffer
	vsprintf_s(strOutput, lenOutput, strTemp, args);

	va_end(args);

	OutputDebugStringA(strOutput);
}

}//Namespace Debugger
}//Namespace Engine
