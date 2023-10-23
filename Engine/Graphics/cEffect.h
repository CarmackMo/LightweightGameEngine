/*
	A effect is a program that implements the effect that need to 
	be render

	This class encapsulates a platform-specific graphics API objec
	that can be loaded and refered to on the CPU.
*/

#pragma once

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Results/Results.h>
#include "cRenderState.h"
#include "cShader.h"
#include <string>

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

	class cEffect
	{
		// Interface
		//=====================

	public:

		// Initialization / Clean Up
		//--------------------------

		static cResult Create(cEffect*& o_effect, const std::string& i_vertexShaderPath, const std::string& i_fragmentShaderPath);

		EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cEffect);

		// Reference Counting
		//--------------------------

		EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

		// Render
		//--------------------------

		void Bind();


		// Implementation
		//==============================

	private:

		// Initialization / Clean Up
		//--------------------------
		cEffect() = default;
		~cEffect();

		cResult Initialize(const std::string& i_vertexShaderPath, const std::string& i_fragmentShaderPath);
		cResult CleanUp();

		cResult InitializeShader(const std::string& i_vertexShaderPath, const std::string& i_fragmentShaderPath);
		cResult CleanUpShader();


		// Data
		//=====================

	private:

		EAE6320_ASSETS_DECLAREREFERENCECOUNT();

		cShader* m_vertexShader = nullptr;
		cShader* m_fragmentShader = nullptr;
		cRenderState* m_renderState = nullptr;

#if defined ( EAE6320_PLATFORM_GL )
		GLuint m_programId = 0;
#endif
		
	};


}// Namespace Graphic
}// Namespace eae6320