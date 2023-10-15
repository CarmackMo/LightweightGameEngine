/*
	This file helps with working with OpenGL under Windows
*/

#ifndef EAE6320_WINDOWS_OPENGL_H
#define EAE6320_WINDOWS_OPENGL_H

// Includes
//=========

#include "Includes.h"

#include <Engine/Results/Results.h>
#include <string>

// Interface
//==========

namespace eae6320
{
	namespace Windows
	{
		namespace OpenGl
		{
			// Initializing OpenGL in Windows requires a device context,
			// and so a hidden window must be created and a device context created
			// before any OpenGL functions can called

			struct sHiddenWindowInfo
			{
				HWND window = NULL;
				ATOM windowClass = NULL;
				HDC deviceContext = NULL;
				HGLRC openGlRenderingContext = NULL;
			};

			// It is ok if the HINSTANCE that is passed to CreateHiddenContextWindow() is NULL,
			// but if it is then the valid HINSTANCE that CreateHiddenContextWindow() assigns _must_ be passed to FreeHiddenContextWindow()
			cResult CreateHiddenContextWindow( HINSTANCE& io_applicationInstance, sHiddenWindowInfo& o_info, std::string* const o_errorMessage = nullptr );
			cResult FreeHiddenContextWindow( const HINSTANCE& i_applicationInstance, sHiddenWindowInfo& io_info, std::string* const o_errorMessage = nullptr );
		}
	}
}

#endif	// EAE6320_WINDOWS_OPENGL_H
