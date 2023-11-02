// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include "../cLine.h"



eae6320::cResult eae6320::Graphics::cLine::Initialize(
	VertexFormats::sVertex_line i_vertexData[],
	const uint32_t i_vertexCount,
	uint16_t i_indexData[],
	const uint32_t i_indexCount)
{
	auto result = Results::Success;

	m_indexCountToRender = i_indexCount;

	// Create a vertex array object and make it active
	{
		constexpr GLsizei arrayCount = 1;
		glGenVertexArrays(arrayCount, &m_vertexArrayId);
		glBindVertexArray(m_vertexArrayId);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return Results::Failure;
		}
	}

	// Create a vertex buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return Results::Failure;
		}
	}
	// Assign vertex data to the vertex buffer
	{
		uint64_t vertexBufferSize = static_cast<uint64_t>(sizeof(VertexFormats::sVertex_line)) * static_cast<uint64_t>(i_vertexCount);
		EAE6320_ASSERT(vertexBufferSize <= std::numeric_limits<GLsizeiptr>::max());
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexBufferSize), reinterpret_cast<GLvoid*>(i_vertexData), GL_STATIC_DRAW);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return Results::Failure;
		}
	}

	// Create a index buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_indexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to bind a new index array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return Results::Failure;
		}
	}
	// Assign index data to the index buffer
	{
		uint64_t indexBufferSize = static_cast<uint64_t>(sizeof(uint16_t)) * static_cast<uint64_t>(i_indexCount);
		EAE6320_ASSERT(indexBufferSize <= std::numeric_limits<GLsizeiptr>::max());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexBufferSize), reinterpret_cast<GLvoid*>(i_indexData), GL_STATIC_DRAW);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return Results::Failure;
		}
	}
	
	// Initialize vertex format
	{
		constexpr auto stride = static_cast<GLsizei>(sizeof(VertexFormats::sVertex_line));

		// Position (0): 3 floats == 12 bytes, Offset = 0
		{
			constexpr GLuint elementLocation = 0;
			constexpr GLint elementCount = 3;
			glVertexAttribPointer(elementLocation, elementCount, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid*>(offsetof(VertexFormats::sVertex_line, x)));
			glEnableVertexAttribArray(elementLocation);

			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
					elementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return Results::Failure;
			}
		}
		// Color (1): 4 floats == 16 bytes, Offest = 12
		{
			constexpr GLuint elementLocation = 1;
			constexpr GLint elementCount = 4;
			glVertexAttribPointer(elementLocation, elementCount, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid*>(offsetof(VertexFormats::sVertex_line, r)));
			glEnableVertexAttribArray(elementLocation);

			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to enable the COLOR vertex attribute at location %u: %s",
					elementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return Results::Failure;
			}
		}
	}

	return result;
}


eae6320::cResult eae6320::Graphics::cLine::CleanUp()
{
	auto result = Results::Success;

	// Clean up vertex array object
	if (m_vertexArrayId != 0)
	{
		glBindVertexArray(0);
		constexpr GLsizei arrayCount = 1;
		glDeleteVertexArrays(arrayCount, &m_vertexArrayId);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}

		m_vertexArrayId = 0;
	}

	// Clean up vertex buffer
	if (m_vertexBufferId != 0)
	{
		constexpr GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_vertexBufferId);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}

		m_vertexBufferId = 0;
	}

	// Clean up index buffer
	if (m_indexBufferId != 0)
	{
		constexpr GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_indexBufferId);

		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}

		m_indexBufferId = 0;
	}

	return result;
}


void eae6320::Graphics::cLine::Draw()
{
	// Draw line. Bind a specific vertex buffer and index buffer to the device as a data source
	{
		EAE6320_ASSERT(m_vertexArrayId != 0);
		glBindVertexArray(m_vertexArrayId);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}

	// Render line from the currently-bound vertex buffer
	{
		constexpr GLenum mode = GL_LINES;
		const GLvoid* const offset = 0;
		glDrawElements(mode, static_cast<GLsizei>(m_indexCountToRender), GL_UNSIGNED_SHORT, offset);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
}