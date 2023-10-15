/*
	This file lists external prebuilt libraries that this library depends on
*/

#pragma once

// External Libraries
//===================

#pragma comment( lib, "Kernel32.lib" )
#pragma comment( lib, "Shell32.lib" )
#pragma comment( lib, "Shlwapi.lib" )

#if defined( EAE6320_PLATFORM_GL )

	#pragma comment( lib, "Gdi32.lib" )
	#pragma comment( lib, "Opengl32.lib" )
	#pragma comment( lib, "User32.lib" )

#endif
