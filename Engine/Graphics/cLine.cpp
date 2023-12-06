// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/cLine.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>

#include <new>


eae6320::cResult eae6320::Graphics::cLine::Create(std::shared_ptr<cLine>& o_line, VertexFormats::sVertex_line i_vertexData[], const uint32_t i_vertexCount, uint16_t i_indexData[], const uint32_t i_indexCount)
{
	auto result = Results::Success;

	std::shared_ptr<cLine> newLine;

	cScopeGuard scopeGuard([&o_line, &result, &newLine]
		{
			if (result)
			{
				EAE6320_ASSERT(newLine != nullptr);
				o_line.swap(newLine);
			}
			else
			{
				if (newLine)
				{
					newLine.reset();
				}
				o_line = nullptr;
			}
		});

	// Allocate a new line
	{
		// Prevent program crash if not enough memory
		newLine = std::shared_ptr<cLine>(new (std::nothrow) cLine(), [](cLine* const i_line) { delete i_line; });
		if (!newLine)
		{
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the line");
			Logging::OutputError("Failed to allocate memory for the line");
			return Results::OutOfMemory;
		}
	}
	// Initialize the platform-specific graphics API mesh object
	if (!(result = newLine->Initialize(
		i_vertexData, i_vertexCount,
		i_indexData, i_indexCount)))
	{
		EAE6320_ASSERTF(false, "Initialization of new line failed");
		return result;
	}

	return result;
}


eae6320::Graphics::cLine::~cLine()
{
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}