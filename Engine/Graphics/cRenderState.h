/*
	"Render state" is a generic term for fixed-function state
	that can be configured on the GPU

	"Fixed-function" means that it isn't programmable like a shader,
	but instead has a pre-determined set of behaviors that can be configured.
*/

#ifndef EAE6320_GRAPHICS_CRENDERSTATE_H
#define EAE6320_GRAPHICS_CRENDERSTATE_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>
#include <Engine/Results/Results.h>

// Render State Bits
//==================

namespace eae6320
{
namespace Graphics
{
namespace RenderStates
{
	// The values below are used as bit masks
	enum eRenderState : uint8_t
	{
		// Set to enable alpha transparency
		AlphaTransparency = 1 << 0,
		// Set to enable depth testing
		DepthTesting = 1 << 1,
		// Set to enable depth writing
		DepthWriting = 1 << 2,
		// Set to draw triangles regardless of whether they are front-facing or back-facing
		// (set to disable culling)
		DrawBothTriangleSides = 1 << 3,
	};

	// Alpha Transparency
	constexpr bool IsAlphaTransparencyEnabled( const uint8_t i_renderStateBits );
	constexpr void EnableAlphaTransparency( uint8_t& io_renderStateBits );
	constexpr void DisableAlphaTransparency( uint8_t& io_renderStateBits );
	// Depth Buffering
	constexpr bool IsDepthTestingEnabled( const uint8_t i_renderStateBits );
	constexpr void EnableDepthTesting( uint8_t& io_renderStateBits );
	constexpr void DisableDepthTesting( uint8_t& io_renderStateBits );
	constexpr bool IsDepthWritingEnabled( const uint8_t i_renderStateBits );
	constexpr void EnableDepthWriting( uint8_t& io_renderStateBits );
	constexpr void DisableDepthWriting( uint8_t& io_renderStateBits );
	// Draw Both Triangle Sides
	constexpr bool ShouldBothTriangleSidesBeDrawn( const uint8_t i_renderStateBits );
	constexpr void EnableDrawingBothTriangleSides( uint8_t& io_renderStateBits );
	constexpr void DisableDrawingBothTriangleSides( uint8_t& io_renderStateBits );

}// Namespace RenderStates
}// Namespace Graphics
}// Namespace eae6320


// Forward Declarations
//=====================

#if defined( EAE6320_PLATFORM_D3D )
	struct ID3D11BlendState;
	struct ID3D11DepthStencilState;
	struct ID3D11RasterizerState;
#endif

// Class Declaration
//==================

namespace eae6320
{
namespace Graphics
{
	class cRenderState
	{
		// Interface
		//==========

	public:

		// Render
		//-------

		void Bind() const;

		// Access
		//-------

		bool IsAlphaTransparencyEnabled() const;
		bool IsDepthTestingEnabled() const;
		bool IsDepthWritingEnabled() const;
		bool ShouldBothTriangleSidesBeDrawn() const;

		uint8_t GetRenderStateBits() const;
		static constexpr uint8_t g_invalidRenderStateBits = 0xff;

		// Initialize / Clean Up
		//----------------------

		// The input parameter is a concatenation of RenderStates::eRenderState bits
		// which define which render states should be enabled
		cResult Initialize( const uint8_t i_renderStateBits );

		cRenderState() = default;
		~cRenderState();

		// Data
		//=====

	private:

#if defined( EAE6320_PLATFORM_D3D )
		ID3D11BlendState* m_blendState = nullptr;
		ID3D11DepthStencilState* m_depthStencilState = nullptr;
		ID3D11RasterizerState* m_rasterizerState = nullptr;
#endif

		uint8_t m_bits = g_invalidRenderStateBits;

		// Implementation
		//===============

	private:

		// Initialize / Clean Up
		//----------------------

		cRenderState( const cRenderState& ) = delete;
		cRenderState( cRenderState&& ) = delete;
		cRenderState& operator =( const cRenderState& ) = delete;
		cRenderState& operator =( cRenderState&& ) = delete;
	};
}
}

#include "cRenderState.inl"

#endif	// EAE6320_GRAPHICS_CRENDERSTATE_H
