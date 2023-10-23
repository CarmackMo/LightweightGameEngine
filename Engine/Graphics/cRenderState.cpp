// Includes
//=========

#include "cRenderState.h"

// Interface
//==========

// Access
//-------

bool eae6320::Graphics::cRenderState::IsAlphaTransparencyEnabled() const
{
	return RenderStates::IsAlphaTransparencyEnabled( m_bits );
}

bool eae6320::Graphics::cRenderState::IsDepthTestingEnabled() const
{
	return RenderStates::IsDepthTestingEnabled( m_bits );
}

bool eae6320::Graphics::cRenderState::IsDepthWritingEnabled() const
{
	return RenderStates::IsDepthWritingEnabled( m_bits );
}

bool eae6320::Graphics::cRenderState::ShouldBothTriangleSidesBeDrawn() const
{
	return RenderStates::ShouldBothTriangleSidesBeDrawn( m_bits );
}

uint8_t eae6320::Graphics::cRenderState::GetRenderStateBits() const
{
	return m_bits;
}
