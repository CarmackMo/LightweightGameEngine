/*
	This file lists external prebuilt libraries that this library depends on
*/

#pragma once

// Includes
//=========

#include "../Configuration.h"

// External Libraries
//===================

#if defined( EAE6320_PLATFORM_D3D )
	#pragma comment( lib, "d3d11.lib" )
#elif defined( EAE6320_PLATFORM_GL )
	#pragma comment( lib, "Opengl32.lib" )
	#pragma comment( lib, "Glu32.lib" )
#endif
