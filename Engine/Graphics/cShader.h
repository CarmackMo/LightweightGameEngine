/*
	A shader is a program that runs on the GPU (graphics hardware)

	This class encapsulates a platform-specific graphics API objec
	that can be loaded and refered to on the CPU.
*/

#ifndef EAE6320_GRAPHICS_CSHADER_H
#define EAE6320_GRAPHICS_CSHADER_H

// Includes
//=========

#include "Configuration.h"

#include <Engine/Assets/ReferenceCountedAssets.h>

#include <cstdint>
#include <Engine/Results/Results.h>
#include <string>

#ifdef EAE6320_PLATFORM_GL
	#include "OpenGL/Includes.h"
#endif

// Shader Types
//=============

namespace eae6320
{
	namespace Graphics
	{
		// In our class we will only deal with the two most common kinds of shaders
		enum class eShaderType : uint8_t
		{
			Unknown = 0,

			// A vertex shader program works with an individual geometric vertex.
			// Its job is to take vertex information as input
			// and output where that vertex should be drawn on the screen.
			Vertex = 1 << 0,
			// A fragment shader program works with an individual screen fragment
			// (which can be thought of as a pixel on screen or a texel in a texture).
			// Its job is to take interpolated data output from a vertex shader
			// and output what color the fragment should be.
			Fragment = 1 << 1
		};
	}
}

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Platform
	{
		struct sDataFromFile;
	}
}

#ifdef EAE6320_PLATFORM_D3D
	struct ID3D11VertexShader;
	struct ID3D11PixelShader;
#endif

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cShader
		{
			// Interface
			//==========

		public:

			// Initialization / Clean Up
			//--------------------------

			static cResult Load( const std::string& i_path, cShader*& o_shader, const eShaderType i_type );

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS( cShader );

			// Reference Counting
			//-------------------

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

			// Data
			//=====

			// In this initial assignment the Graphics system needs access to the shader API objects.
			// In future assignments you can make the data private and decide who needs access
			// and the best way to provide it.
			// (In other words, this doesn't follow my typical style,
			// but I am purposely leaving it up to you to decide what the best design is according to your style.)
		public:

#if defined( EAE6320_PLATFORM_D3D )
			union
			{
				ID3D11VertexShader* vertex = nullptr;
				ID3D11PixelShader* fragment;
			} m_shaderObject;
#elif defined( EAE6320_PLATFORM_GL )
			GLuint m_shaderId = 0;
#endif
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();
			const eShaderType m_type = eShaderType::Unknown;

			// Implementation
			//===============

		private:

			// Initialization / Clean Up
			//--------------------------

			cResult Initialize( const std::string& i_path, const Platform::sDataFromFile& i_loadedShader );
			cResult CleanUp();

			cShader( const eShaderType i_type );
			~cShader();
		};
	}
}

#endif	// EAE6320_GRAPHICS_CSHADER_H
