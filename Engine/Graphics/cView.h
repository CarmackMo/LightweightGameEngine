/*
	"views" are objects that allow a texture to be used a particular way
*/

#pragma once

#include <Engine/Results/cResult.h>
#include <Engine/Graphics/Graphics.h>


#if defined ( EAE6320_PLATFORM_D3D )
#include "Direct3D/Includes.h"
#elif defined ( EAE6320_PLATFORM_GL )
#include "OpenGL/Includes.h"
#endif



// Class Declaration
//=====================

namespace eae6320
{
namespace Graphics
{

	class cView
	{
		// Interface
		//=====================

	public:
		
		// Initialization / Clean Up
		//==============================

		cResult Initialize(const sInitializationParameters& i_initializationParameters);

		cResult CleanUp();

		void ClearBackBuffer(const float* i_clearColor);

		void SwapBuffer();

	private:

		// Data
		//=====================
#if defined ( EAE6320_PLATFORM_D3D )
		// A render target view allows a texture to have color rendered to it
		ID3D11RenderTargetView* s_renderTargetView = nullptr;
		// A depth/stencil view allows a texture to have depth rendered to it
		ID3D11DepthStencilView* s_depthStencilView = nullptr;

#endif


	};



}// Namespace Graphics
}// Namespace eae6320
