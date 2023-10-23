/*
	This file provides configurable settings
	that can be used to modify the graphics project
*/

#ifndef EAE6320_GRAPHICS_CONFIGURATION_H
#define EAE6320_GRAPHICS_CONFIGURATION_H

// Usually device debug info is only enabled on debug builds
#ifdef _DEBUG
	#define EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
#endif

// Debug shaders are useful for debugging,
// but shouldn't be included in the shipping game
#ifdef _DEBUG
	#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

#endif	// EAE6320_GRAPHICS_CONFIGURATION_H
