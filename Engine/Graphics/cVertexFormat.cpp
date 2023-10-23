// Includes
//=========

#include "cVertexFormat.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <new>

// Interface
//==========

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cVertexFormat::Load( const eVertexType i_type, cVertexFormat*& o_vertexFormat, const char* const i_vertexShaderPath )
{
	auto result = Results::Success;

	EAE6320_ASSERT( o_vertexFormat == nullptr );

	cVertexFormat* newVertexFormat = nullptr;
	cScopeGuard scopeGuard( [&o_vertexFormat, &result, &newVertexFormat]
		{
			if ( result )
			{
				EAE6320_ASSERT( newVertexFormat != nullptr );
				o_vertexFormat = newVertexFormat;
			}
			else
			{
				if ( newVertexFormat )
				{
					newVertexFormat->DecrementReferenceCount();
					newVertexFormat = nullptr;
				}
				o_vertexFormat = nullptr;
			}
		} );

	// Allocate a new vertex format
	{
		newVertexFormat = new (std::nothrow) cVertexFormat();
		if ( !newVertexFormat )
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF( false, "Couldn't allocate memory for a vertex format" );
			Logging::OutputError( "Failed to allocate memory for a vertex format" );
			return result;
		}
	}
	// Initialize the platform-specific vertex format
	if ( !( result = newVertexFormat->Initialize( i_type, i_vertexShaderPath ) ) )
	{
		EAE6320_ASSERTF( false, "Initialization of new vertex format failed" );
		return result;
	}

	return result;
}

// Implementation
//===============

// Initialize / Clean Up
//----------------------

eae6320::Graphics::cVertexFormat::~cVertexFormat()
{
	EAE6320_ASSERT( m_referenceCount == 0 );
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}
