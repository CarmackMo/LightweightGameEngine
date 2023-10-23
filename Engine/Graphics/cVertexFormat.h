/*
	A vertex format determines how the GPU interprets
	the geometric data that the CPU sends it

	It is important to differentiate a "vertex format" object from the actual vertex data:
		* An instance of this cVertexFormat class is an object owned by the platform-specific graphics API
		* The vertex data is an array of one of the structs defined in VertexFormats.h,
			but in order for it to be used in a draw call
			the GPU must use a vertex format object (an instance of this class)
			to know how to interpret the data in the vertex shader program
*/

#ifndef EAE6320_GRAPHICS_VERTEXFORMATS_H
#define EAE6320_GRAPHICS_VERTEXFORMATS_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>
#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Results/Results.h>

// Forward Declarations
//=====================

#ifdef EAE6320_PLATFORM_D3D
	struct ID3D11InputLayout;
#endif

// Vertex Types
//=============

namespace eae6320
{
	namespace Graphics
	{
		// (see VertexFormats.h for the data layout of the vertex types)
		enum class eVertexType : uint8_t
		{
			//	* Mesh:
			//		* This is the type of vertex used by 3D geometry in world space
			//			(which is the only kind of geometry we will render in this class)
			//		* In our class we will only define one type of vertex for a mesh,
			//			but in a real game there would be many different kinds
			//			depending on what different shaders needed
			//		* See VertexFormats::sVertex_mesh for the data layout of this vertex type
			Mesh,
		};
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Graphics
	{
		class cVertexFormat
		{
			// Interface
			//==========

		public:

			// Render
			//-------

			void Bind();

			// Initialize / Clean Up
			//----------------------

			static cResult Load( const eVertexType i_type, cVertexFormat*& o_vertexFormat, const char* const i_vertexShaderPath );

			EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS( cVertexFormat );

			// Reference Counting
			//-------------------

			EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

			// Data
			//=====

		private:

#if defined( EAE6320_PLATFORM_D3D )
			// D3D has an "input layout" object that associates the layout of the vertex format struct
			// with the input from a vertex shader
			ID3D11InputLayout* m_vertexInputLayout = nullptr;
#endif
			
			EAE6320_ASSETS_DECLAREREFERENCECOUNT();

			// Implementation
			//===============

		private:

			// Initialize / Clean Up
			//----------------------

			cResult Initialize( const eVertexType i_type, const char* const i_vertexShaderPath );
			cResult CleanUp();

			cVertexFormat() = default;
			~cVertexFormat();
		};
	}
}

#endif	// EAE6320_GRAPHICS_VERTEXFORMATS_H
