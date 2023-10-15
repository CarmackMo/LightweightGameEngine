/*
	This file can be included by a project that wants to use OpenGL
*/

#ifndef EAE6320_ENGINE_GRAPHICS_OPENGL_INCLUDES_H
#define EAE6320_ENGINE_GRAPHICS_OPENGL_INCLUDES_H

#if defined( EAE6320_PLATFORM_WINDOWS )
	// windows.h _must_ be #included before GL.h
	#include <Engine/Windows/Includes.h>

	// The names of these two headers are standard for any platform,
	// but the location isn't
	#include <gl/GL.h>
	#include <gl/GLU.h>	// The "U" is for "utility functions"
#endif

// Modern OpenGL requires extensions
#include <External/OpenGlExtensions/OpenGlExtensions.h>

#endif	// EAE6320_ENGINE_GRAPHICS_OPENGL_INCLUDES_H
