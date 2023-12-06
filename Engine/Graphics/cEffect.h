/*
	A effect is a program that implements the effect that need to 
	be render

	This class encapsulates a platform-specific graphics API objec
	that can be loaded and refered to on the CPU.
*/

#pragma once

#include <Engine/Graphics/cRenderState.h>
#include <Engine/Graphics/cShader.h>
#include <Engine/Results/Results.h>

#include <memory>
#include <string>

#if defined ( EAE6320_PLATFORM_D3D )
#include <Engine/Graphics/Direct3D/Includes.h>
#elif defined ( EAE6320_PLATFORM_GL )
#include <Engine/Graphics/OpenGL/Includes.h>
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

		static cResult Create(
			cEffect*& o_effect, 
			const std::string& i_vertexShaderPath, 
			const std::string& i_fragmentShaderPath);

		static cResult Create(
			std::shared_ptr<cEffect>& o_effect, 
			const std::string& i_vertexShaderPath, 
			const std::string& i_fragmentShaderPath);

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

		cShader* m_vertexShader = nullptr;
		cShader* m_fragmentShader = nullptr;
		cRenderState* m_renderState = nullptr;

#if defined ( EAE6320_PLATFORM_GL )
		GLuint m_programId = 0;
#endif
		
	};


}// Namespace Graphic
}// Namespace eae6320