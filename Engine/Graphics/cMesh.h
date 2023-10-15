/*
	A mesh is a program stores the geometric data (i.e. vertex array) 
	of the objects that need to be render

	This class encapsulates a platform-specific graphics API objec
	that can be loaded and refered to on the CPU.
*/

#pragma once

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Results/Results.h>
#include "VertexFormats.h"

#include <string>

#if defined ( EAE6320_PLATFORM_D3D )
	#include "cVertexFormat.h"
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

	class cMesh
	{
		// Interface
		//=====================

	public:

		// Initialization / Clean Up
		//--------------------------

		static cResult Create(
			cMesh*& o_mesh,
			VertexFormats::sVertex_mesh i_vertexData[], 
			const uint32_t i_vertexCount, 
			uint16_t i_indexData[], 
			const uint32_t i_indexCount,
			const uint32_t i_indexCountToRender,
			const uint32_t i_indexOfFirstIndexToUse = 0,
			const uint32_t i_offsetToAddToEachIndex = 0);

		static cResult Create(
			cMesh*& o_mesh, 
			const std::string& i_meshPath);

		EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS( cMesh );

		// Reference Counting
		//--------------------------

		EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();

		// Render
		//--------------------------

		void Draw();


		// Implementation
		//=====================

	private:

		// Initialization / Clean Up
		//--------------------------

		cMesh() = default;
		~cMesh();

		cResult Initialize(
			VertexFormats::sVertex_mesh i_vertexData[],
			const uint32_t i_vertexCount,
			uint16_t i_indexData[],
			const uint32_t i_indexCount,
			const uint32_t i_indexCountToRender,
			const uint32_t i_indexOfFirstIndexToUse = 0,
			const uint32_t i_offsetToAddToEachIndex = 0);

		cResult CleanUp();

		cResult LoadLuaAsset(const char* const i_path);

		cResult LoadBinaryAsset(const char* const i_path);

		// Data
		//=====================

	private:

		EAE6320_ASSETS_DECLAREREFERENCECOUNT();

		uint32_t m_indexOfFirstIndexToUse = 0;
		uint32_t m_offsetToAddToEachIndex = 0;
		uint32_t m_indexCountToRender = 0;

#if defined ( EAE6320_PLATFORM_D3D )
		cVertexFormat* m_vertexFormat = nullptr;
		ID3D11Buffer* m_vertexBuffer = nullptr;
		ID3D11Buffer* m_indexBuffer = nullptr;
#elif defined (EAE6320_PLATFORM_GL)
		// A vertex array encapsulates the vertex data as well as the vertex input layout
		GLuint m_vertexArrayId = 0;
		// A vertex buffer holds the data for each vertex
		GLuint m_vertexBufferId = 0;
		// A index buffer holds the data of rendering order
		GLuint m_indexBufferId = 0;
#endif

	};


}// Namespace Graphic
}// Namespace eae6320
