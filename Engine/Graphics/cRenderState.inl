#ifndef EAE6320_GRAPHICS_CRENDERSTATE_INL
#define EAE6320_GRAPHICS_CRENDERSTATE_INL

// Includes
//=========

#include "cRenderState.h"

// Render State Bits
//==================

// Alpha Transparency
constexpr bool eae6320::Graphics::RenderStates::IsAlphaTransparencyEnabled( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & AlphaTransparency ) != 0;
}
constexpr void eae6320::Graphics::RenderStates::EnableAlphaTransparency( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= AlphaTransparency;
}
constexpr void eae6320::Graphics::RenderStates::DisableAlphaTransparency( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~AlphaTransparency;
}

// Depth Buffering
constexpr bool eae6320::Graphics::RenderStates::IsDepthTestingEnabled( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & DepthTesting ) != 0;
}
constexpr void eae6320::Graphics::RenderStates::EnableDepthTesting( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= DepthTesting;
}
constexpr void eae6320::Graphics::RenderStates::DisableDepthTesting( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~DepthTesting;
}
constexpr bool eae6320::Graphics::RenderStates::IsDepthWritingEnabled( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & DepthWriting ) != 0;
}
constexpr void eae6320::Graphics::RenderStates::EnableDepthWriting( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= DepthWriting;
}
constexpr void eae6320::Graphics::RenderStates::DisableDepthWriting( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~DepthWriting;
}

// Draw Both Triangle Sides
constexpr bool eae6320::Graphics::RenderStates::ShouldBothTriangleSidesBeDrawn( const uint8_t i_renderStateBits )
{
	return ( i_renderStateBits & DrawBothTriangleSides ) != 0;
}
constexpr void eae6320::Graphics::RenderStates::EnableDrawingBothTriangleSides( uint8_t& io_renderStateBits )
{
	io_renderStateBits |= DrawBothTriangleSides;
}
constexpr void eae6320::Graphics::RenderStates::DisableDrawingBothTriangleSides( uint8_t& io_renderStateBits )
{
	io_renderStateBits &= ~DrawBothTriangleSides;
}

#endif	// EAE6320_GRAPHICS_CRENDERSTATE_INL
