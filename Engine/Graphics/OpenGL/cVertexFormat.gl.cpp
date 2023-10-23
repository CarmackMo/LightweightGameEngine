/*
	In OpenGL the vertex format data is encapsulated in a vertex array,
	and so this class isn't necessary
	(and shouldn't be used)
*/

// Includes
//=========

#include "../cVertexFormat.h"

// Interface
//==========

// Render
//-------

void eae6320::Graphics::cVertexFormat::Bind()
{

}

// Implementation
//===============

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cVertexFormat::Initialize( const eVertexType i_type, const char* const i_vertexShaderPath )
{
	return Results::Success;
}

eae6320::cResult eae6320::Graphics::cVertexFormat::CleanUp()
{
	return Results::Success;
}
