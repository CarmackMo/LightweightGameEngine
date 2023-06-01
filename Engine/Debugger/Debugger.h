#pragma once
#include <windows.h>
#include <stdio.h>

namespace Engine
{
namespace Debugger
{


class DebugManager
{
public:
	template <typename T>
	static void Print(const char* const text, T value);

	static void ConsolePrint(const char* i_pFmt, const char* i_pFile, unsigned int i_Line, ...);
};


}
}

#define DEBUG_PRINT(fmt,...) DebugManager::ConsolePrint((fmt),__FILE__,__LINE__,__VA_ARGS__)