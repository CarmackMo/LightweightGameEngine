// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/sContext.h>
#include <Engine/Logging/Logging.h>



eae6320::cResult eae6320::Graphics::cMesh::Initialize(
	VertexFormats::sVertex_mesh i_vertexData[],
	const uint32_t i_vertexCount,
	uint16_t i_indexData[],
	const uint32_t i_indexCount,
	const uint32_t i_indexCountToRender,
	const uint32_t i_indexOfFirstIndexToUse,
	const uint32_t i_offsetToAddToEachIndex)
{
	m_indexOfFirstIndexToUse = i_indexOfFirstIndexToUse;
	m_offsetToAddToEachIndex = i_offsetToAddToEachIndex;
	m_indexCountToRender = i_indexCountToRender;


	sContext& GLContext = sContext::g_context;
	auto GLContextHandler = wglGetCurrentContext();
	auto temp = 0;

	// Wait for the graphics thread finishes the rendering of last frame,
	// Then claim the rendering context from rendering thread and signal rendering
	// thread that a new effect object starts initializing.
	{
		cResult canBeInitialized;
		canBeInitialized = Graphics::WaitUntilRenderingOfCurrentFrameIsCompleted(~unsigned int(0u));

		if (canBeInitialized == Results::Success)
		{
			if (Graphics::ResetThatExistRenderObjectNotInitializedYet() == Results::Failure)
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new effect starts initializing");
				Logging::OutputError("Couldn't signal that new effect starts initializing");
				return Results::Failure;
			}

			if (sContext::g_context.EnableContext() == FALSE)
			{
				EAE6320_ASSERTF(false, "Enable rendering context for initializing new effect in main thread failed");
				Logging::OutputError("Enable rendering context for initializing new effect in main thread failed");
				return Results::Failure;
			}
		}
		else
		{
			Logging::OutputError("Failed to wait for rendering thread finishes rendering last frame");
			return Results::Failure;
		}
	}

	auto result = eae6320::Results::Success;

	// Create a vertex array object and make it active
	{
		constexpr GLsizei arrayCount = 1;
		glGenVertexArrays(arrayCount, &m_vertexArrayId);
		const auto errorCode = glGetError();

		if (errorCode == GL_NO_ERROR)
		{
			glBindVertexArray(m_vertexArrayId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}

	// Create a vertex buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_vertexBufferId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}

	// Create a index buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_indexBufferId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind a new index buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused index buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}

	// Assign the vertex data to the vertex buffer
	{
		uint64_t vertexBufferSize = static_cast<uint64_t>(sizeof(VertexFormats::sVertex_mesh)) * static_cast<uint64_t>(i_vertexCount);

		EAE6320_ASSERT(vertexBufferSize <= std::numeric_limits<GLsizeiptr>::max());
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexBufferSize), reinterpret_cast<GLvoid*>(i_vertexData),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}

	// Assign the index data to the index buffer
	{
		uint64_t indexBufferSize = static_cast<uint64_t>(sizeof(uint16_t)) * static_cast<uint64_t>(i_indexCount);

		EAE6320_ASSERT(indexBufferSize <= std::numeric_limits<GLsizeiptr>::max());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indexBufferSize), reinterpret_cast<GLvoid*>(i_indexData),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}

	// Initialize vertex format
	{
		// The "stride" defines how large a single vertex is in the stream of data
		// (or, said another way, how far apart each position element is)
		constexpr auto stride = static_cast<GLsizei>(sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh));
		constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is

		// Position (0): 3 floats == 12 bytes, Offset = 0
		{
			constexpr GLuint vertexElementLocation = 0;
			constexpr GLint elementCount = 3;
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(eae6320::Graphics::VertexFormats::sVertex_mesh, x)));
			const auto errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		// Color (1): 4 floats == 16 bytes, Offset = 12
		{
			constexpr GLuint vertexElementLocation = 1;
			constexpr GLint elementCount = 4;
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(eae6320::Graphics::VertexFormats::sVertex_mesh, r)));
			const auto errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to enable the COLOR vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set the COLOR vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
	}



	// Release rendering context and signal rendering thread that this effect 
	// object finishes initializing
	{
		if (sContext::g_context.DisableContext() == FALSE)
		{
			EAE6320_ASSERTF(false, "Release rendering context after initializing new effect in main thread failed");
			Logging::OutputError("Release rendering context after initializing new effect in main thread failed");
		}

		if (Graphics::SignalThatAllRenderObjectsHaveBeenInitialized() == Results::Failure)
		{
			EAE6320_ASSERTF(false, "Couldn't signal that new effect finishes initializing");
			Logging::OutputError("Couldn't signal that new effect finishes initializing");
		}
	}


	return result;

}


eae6320::cResult eae6320::Graphics::cMesh::CleanUp()
{
	auto result = Results::Success;

	if (m_vertexArrayId != 0)
	{
		// Make sure that the vertex array isn't bound
		{
			// Unbind the vertex array
			glBindVertexArray(0);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				if (result)
				{
					result = Results::Failure;
				}
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				Logging::OutputError("OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
			}
		}
		constexpr GLsizei arrayCount = 1;
		glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			if (result)
			{
				result = Results::Failure;
			}
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexArrayId = 0;
	}

	if (m_vertexBufferId != 0)
	{
		constexpr GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_vertexBufferId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			if (result)
			{
				result = Results::Failure;
			}
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexBufferId = 0;
	}

	if (m_indexBufferId != 0)
	{
		constexpr GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_indexBufferId);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			if (result)
			{
				result = Results::Failure;
			}
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_indexBufferId = 0;
	}

	return result;
}


void eae6320::Graphics::cMesh::Draw()
{
	// Draw mesh. Bind a specific vertex buffer and index buffer to the device as a data source
	{
		EAE6320_ASSERT(m_vertexArrayId != 0);
		glBindVertexArray(m_vertexArrayId);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}

	// Render triangles from the currently-bound vertex buffer
	{
		// The mode defines how to interpret multiple vertices as a single "primitive";
		// a triangle list is defined
		// (meaning that every primitive is a triangle and will be defined by three vertices)
		constexpr GLenum mode = GL_TRIANGLES;
		// It's possible to start rendering primitives in the middle of the stream
		const GLvoid* const offset = 0;
		glDrawElements(mode, static_cast<GLsizei>(m_indexCountToRender), GL_UNSIGNED_SHORT, offset);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
}