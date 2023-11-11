#pragma once

#include <Engine/Assets/ReferenceCountedAssets.h>
#include <Engine/Results/Results.h>
#include "VertexFormats.h"

#if defined (EAE6320_PLATFORM_D3D)
	#include "cVertexFormat.h"
	#include "Direct3D/Includes.h"
#elif defined (EAE6320_PLATFORM_GL)
	#include "OpenGL/Includes.h"
#endif


// Class Declaration
//=====================

namespace eae6320
{
namespace Graphics
{
	
	class cLine
	{
		// Interface
		//=====================

	public:

		// Initialization / Clean Up
		//--------------------------

		static cResult Create(
			cLine*& o_line,
			VertexFormats::sVertex_line i_vertexData[],
			const uint32_t i_vertexCount,
			uint16_t i_indexData[],
			const uint32_t i_indexCount);

		// Render
		//--------------------------

		void Draw();

		// Reference Counting
		//--------------------------

		EAE6320_ASSETS_DECLAREDELETEDREFERENCECOUNTEDFUNCTIONS(cLine);

		EAE6320_ASSETS_DECLAREREFERENCECOUNTINGFUNCTIONS();


		// Implementation
		//=====================

	private:

		cLine() = default;
		~cLine();

		cResult Initialize(
			VertexFormats::sVertex_line i_vertexData[],
			const uint32_t i_vertexCount,
			uint16_t i_indexData[],
			const uint32_t i_indexCount);

		cResult CleanUp();


		// Data
		//=====================

	private:

		EAE6320_ASSETS_DECLAREREFERENCECOUNT();

		uint32_t m_indexCountToRender = 0;

#if defined (EAE6320_PLATFORM_D3D)
		cVertexFormat* m_vertexFormat = nullptr;
		ID3D11Buffer* m_vertexBuffer = nullptr;
		ID3D11Buffer* m_indexBuffer = nullptr;
#elif defined (EAE6320_PLATFORM_GL)
		GLuint m_vertexArrayId = 0;
		GLuint m_vertexBufferId = 0;
		GLuint m_indexBufferId = 0;
#endif
		
	};


}// Namespace Graphic
}// Namespace eae6320