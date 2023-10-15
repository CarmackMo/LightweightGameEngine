/*
	A constant buffer is a block of data
	that is copied from the CPU to the GPU

	The name "constant" is because the data is
	constant for every vertex and fragment in a single draw call
	(as opposed to data that will be different in different invocations of a shader,
	like per-vertex data in a vertex shader or interpolated data in a fragment shader).

	It is important to differentiate a "constant buffer" object from the actual constant data:
		* A constant buffer is an object owned by the platform-specific graphics API,
			whose sole purpose is to get data from the CPU to the GPU
		* The constant data is calculated and handled just like any other C++ data in the application
			(and is stored in one of the structs defined in ConstantBufferFormats.h)
			but in order for it to be used in a draw call
			it must use a constant buffer object to be available to the shader programs running on the GPU
*/

#ifndef EAE6320_GRAPHICS_CCONSTANTBUFFER_H
#define EAE6320_GRAPHICS_CCONSTANTBUFFER_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>
#include <Engine/Results/Results.h>

#ifdef EAE6320_PLATFORM_GL
	#include "OpenGL/Includes.h"
#endif

// Forward Declarations
//=====================

#ifdef EAE6320_PLATFORM_D3D
	struct ID3D11Buffer;
#endif

// Constant Buffer Types
//======================

namespace eae6320
{
namespace Graphics
{
	// In our class we will define three different types of constant buffers
	// (see ConstantBufferFormats.h for the data layout of each type)
	enum class ConstantBufferTypes : uint8_t
	{
		// These values aren't arbitrary enumerations;
		// they must match the IDs assigned to the corresponding constant buffer definitions in shader code

		//	* Frame:
		//		* These values are constant for the entire rendered frame
		//		* The constant buffer must be updated and bound at the start of a frame,
		//			but then it doesn't ever change
		Frame = 0,
		//	* Material:
		//		* These are values that come from an authored material
		//		* If multiple successive draw calls use the same material
		//			then the constant buffer must be updated and bound before the first one
		//			but doesn't need to change until a new material is rendered
		Material = 1,
		//	* Draw Call:
		//		* These are values that are associated with a specific draw call
		//		* The constant buffer must be updated and bound for every draw call that is made
		DrawCall = 2,

		Count,
		Invalid = Count
	};
}
}

// Class Declaration
//==================

namespace eae6320
{
namespace Graphics
{
	class cConstantBuffer
	{
		// Interface
		//==========

	public:

		// Render
		//-------
			
		// When a constant buffer is bound all shaders that use a defined constant buffer with matching ID
		// (i.e. when a C++ constant buffer's type enumeration matches the constant buffer ID defined in the shader)
		// will get data from that constant buffer until a different constant buffer is bound.
		// i_shaderTypesToBindTo is a concatenation of Graphics::ShaderTypes,
		// and specifies which shader types need to use this constant buffer.
		void Bind( const uint_fast8_t i_shaderTypesToBindTo ) const;

		// Copies the specified CPU data to the GPU memory associated with the constant buffer.
		// The specified data must be the appropriate Graphics::ConstantBufferFormats struct corresponding to this constant buffer's type!
		// This function only needs to be called when the constant data that the GPU is using needs to change.
		void Update( const void* const i_data );

		// Initialize / Clean Up
		//----------------------

		cResult Initialize( const void* const i_initialData = nullptr );
		cResult CleanUp();

		cConstantBuffer( const ConstantBufferTypes i_type );
		~cConstantBuffer();

		// Data
		//=====

	private:

		// The size of the constant data associated with this constant buffer.
		// It is calculated internally as sizeof( Graphics::ConstantBufferFormats::[m_type] )
		size_t m_size = 0;

#if defined( EAE6320_PLATFORM_D3D )
		ID3D11Buffer* m_buffer = nullptr;
#elif defined( EAE6320_PLATFORM_GL )
		GLuint m_bufferId = 0;
#endif
			
		// The constant buffer type defines the size of the constant data
		// and is used to bind the constant buffer (the type enumeration is used as an ID)
		const ConstantBufferTypes m_type = ConstantBufferTypes::Invalid;

		// Implementation
		//---------------

	private:

		// Initialize / Clean Up
		//----------------------

		cResult Initialize_platformSpecific( const void* const i_initialData );

		cConstantBuffer( const cConstantBuffer& ) = delete;
		cConstantBuffer( cConstantBuffer&& ) = delete;
		cConstantBuffer& operator =( const cConstantBuffer& ) = delete;
		cConstantBuffer& operator =( cConstantBuffer&& ) = delete;
	};
}
}

#endif	// EAE6320_GRAPHICS_CCONSTANTBUFFER_H
