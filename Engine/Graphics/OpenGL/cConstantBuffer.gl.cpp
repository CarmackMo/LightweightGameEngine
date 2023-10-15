// Includes
//=========

#include "../cConstantBuffer.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Math/Functions.h>

// Interface
//==========

// Render
//-------

void eae6320::Graphics::cConstantBuffer::Bind( const uint_fast8_t ) const
{
	EAE6320_ASSERT( m_bufferId != 0 );

	// OpenGL doesn't have a way to only bind the constant buffer to specific shader types,
	// and so the input parameter isn't used
	glBindBufferBase( GL_UNIFORM_BUFFER, static_cast<GLuint>( m_type ), m_bufferId );
	EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
}

void eae6320::Graphics::cConstantBuffer::Update( const void* const i_data )
{
	EAE6320_ASSERT( m_bufferId != 0 );

	// Make the uniform buffer active
	{
		glBindBuffer( GL_UNIFORM_BUFFER, m_bufferId );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
	// Copy the updated memory to the GPU
	{
		GLintptr updateAtTheBeginning = 0;
		glBufferSubData( GL_UNIFORM_BUFFER, updateAtTheBeginning, static_cast<GLsizeiptr>( m_size ), i_data );
		EAE6320_ASSERT( glGetError() == GL_NO_ERROR );
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cConstantBuffer::CleanUp()
{
	auto result = Results::Success;

	if ( m_bufferId != 0 )
	{
		constexpr GLsizei bufferCount = 1;
		glDeleteBuffers( bufferCount, &m_bufferId );
		const auto errorCode = glGetError();
		if ( errorCode != GL_NO_ERROR )
		{
			result = Results::Failure;
			EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			Logging::OutputError( "OpenGL failed to delete the constant buffer: %s",
				reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
		}
		m_bufferId = 0;
	}

	return result;
}

// Implementation
//===============

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cConstantBuffer::Initialize_platformSpecific( const void* const i_initialData )
{
	auto result = Results::Success;

	// Create a uniform buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers( bufferCount, &m_bufferId );
		const auto errorCode = glGetError();
		if ( errorCode == GL_NO_ERROR )
		{
			glBindBuffer( GL_UNIFORM_BUFFER, m_bufferId );
			const auto errorCode = glGetError();
			if ( errorCode != GL_NO_ERROR )
			{
				result = Results::Failure;
				EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				eae6320::Logging::OutputError( "OpenGL failed to bind the new uniform buffer %u: %s",
					m_bufferId, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
				return result;
			}
		}
		else
		{
			result = Results::Failure;
			EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			eae6320::Logging::OutputError( "OpenGL failed to get an unused uniform buffer ID: %s",
				reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			return result;
		}
	}
	// Allocate space and copy the constant data into the uniform buffer
	{
		constexpr GLenum usage = GL_DYNAMIC_DRAW;	// The buffer will be modified frequently and used to draw
		glBufferData( GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>( m_size ),
			reinterpret_cast<const GLvoid*>( i_initialData ), usage );
		const auto errorCode = glGetError();
		if ( errorCode != GL_NO_ERROR )
		{
			result = Results::Failure;
			EAE6320_ASSERTF( false, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			eae6320::Logging::OutputError( "OpenGL failed to allocate the new uniform buffer %u: %s",
				m_bufferId, reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
			return result;
		}
	}

	return result;
}
