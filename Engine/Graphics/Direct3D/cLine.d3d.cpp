// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include "../cLine.h"


// TODO: current implementation still use OpenGL as rendering backend


eae6320::cResult eae6320::Graphics::cLine::Initialize(
	VertexFormats::sVertex_line i_vertexData[],
	const uint32_t i_vertexCount,
	uint16_t i_indexData[],
	const uint32_t i_indexCount)
{
	auto result = Results::Success;

	return result;
}


eae6320::cResult eae6320::Graphics::cLine::CleanUp()
{
	auto result = Results::Success;

	return result;
}


void eae6320::Graphics::cLine::Draw()
{

}