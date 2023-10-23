// Includes
//=========

#include "cShader.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Concurrency/cMutex.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <new>
#include <vector>

// Helper Class Declaration
//=========================

namespace
{
	// This exists for our class to help you catch reference counting problems.
	// In a real game there would be more sophisticated methods that would work for all asset types.
	class cShaderTracker
	{
		// Interface
		//----------

	public:

		// Access
		void AddShader( const eae6320::Graphics::cShader* const i_shader );
		void RemoveShader( const eae6320::Graphics::cShader* const i_shader );
		// Initializate / Clean Up
		cShaderTracker() = default;
		~cShaderTracker();

		// Data
		//-----

	private:

		std::vector<const eae6320::Graphics::cShader*> m_shaders;
		eae6320::Concurrency::cMutex m_mutex;

		// Implementation
		//---------------

	private:

		// Initializate / Clean Up
		cShaderTracker( const cShaderTracker& ) = delete;
		cShaderTracker( cShaderTracker&& ) = delete;
		cShaderTracker& operator =( const cShaderTracker& ) = delete;
		cShaderTracker& operator =( cShaderTracker&& ) = delete;
	} s_shaderTracker;
}

// Interface
//==========

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::cShader::Load( const std::string& i_path, cShader*& o_shader, const eShaderType i_type )
{
	auto result = Results::Success;

	Platform::sDataFromFile dataFromFile;
	cShader* newShader = nullptr;
	cScopeGuard scopeGuard( [&o_shader, &result, &dataFromFile, &newShader]
		{
			if ( result )
			{
				EAE6320_ASSERT( newShader != nullptr );
				o_shader = newShader;
			}
			else
			{
				if ( newShader )
				{
					newShader->DecrementReferenceCount();
					newShader = nullptr;
				}
				o_shader = nullptr;
			}
		} );

	// Validate the shader type
	{
		switch ( i_type )
		{
		case eShaderType::Vertex:
		case eShaderType::Fragment:
			break;
		default:
			result = Results::Failure;
			EAE6320_ASSERTF( false, "Invalid shader type: %i", i_type );
			Logging::OutputError( "An attempt was made to load a shader with an invalid type (%i)", i_type );
			return result;
		}
	}
	// Load the binary data
	{
		std::string errorMessage;
		if ( !( result = Platform::LoadBinaryFile( i_path.c_str(), dataFromFile, &errorMessage ) ) )
		{
			EAE6320_ASSERTF( false, errorMessage.c_str() );
			Logging::OutputError( "Failed to load shader from file %s: %s", i_path.c_str(), errorMessage.c_str() );
			return result;
		}
	}
	// Allocate a new shader
	{
		newShader = new (std::nothrow) cShader( i_type );
		if ( !newShader )
		{
			result = Results::OutOfMemory;
			EAE6320_ASSERTF( false, "Couldn't allocate memory for the shader %s", i_path.c_str() );
			Logging::OutputError( "Failed to allocate memory for the shader %s", i_path.c_str() );
			return result;
		}
	}
	// Initialize the platform-specific graphics API shader object
	if ( !( result = newShader->Initialize( i_path, dataFromFile ) ) )
	{
		EAE6320_ASSERTF( false, "Initialization of new shader failed" );
		return result;
	}

	return result;
}

// Implementation
//===============

// Initialize / Clean Up
//----------------------

eae6320::Graphics::cShader::cShader( const eShaderType i_type )
	:
	m_type( i_type )
{
	s_shaderTracker.AddShader( this );
}

eae6320::Graphics::cShader::~cShader()
{
	EAE6320_ASSERT( m_referenceCount == 0 );
	s_shaderTracker.RemoveShader( this );
	const auto result = CleanUp();
	EAE6320_ASSERT( result );
}

// Helper Class Definition
//========================

namespace
{
	// Interface
	//----------

	// Access

	void cShaderTracker::AddShader( const eae6320::Graphics::cShader* const i_shader )
	{
		EAE6320_ASSERT( i_shader != nullptr );
		eae6320::Concurrency::cMutex::cScopeLock scopeLock( m_mutex );
#ifdef EAE6320_ASSERTS_AREENABLED
		for ( const auto *const shader : m_shaders )
		{
			EAE6320_ASSERTF( shader != i_shader, "This shader is already being tracked" );
		}
#endif
		m_shaders.push_back( i_shader );
	}

	void cShaderTracker::RemoveShader( const eae6320::Graphics::cShader* const i_shader )
	{
		eae6320::Concurrency::cMutex::cScopeLock scopeLock( m_mutex );
		const auto shaderCount = m_shaders.size();
		for ( std::remove_const<decltype( shaderCount )>::type i = 0; i < shaderCount; ++i )
		{
			if ( m_shaders[i] == i_shader )
			{
				m_shaders.erase( m_shaders.begin() + i );
				return;
			}
		}
		EAE6320_ASSERTF( false, "This shader wasn't being tracked" );
	}

	// Initialize / Clean Up

	cShaderTracker::~cShaderTracker()
	{
		eae6320::Concurrency::cMutex::cScopeLock scopeLock( m_mutex );
		const auto haveAnyShadersNotBeenDeleted = !m_shaders.empty();
		if ( haveAnyShadersNotBeenDeleted )
		{
			EAE6320_ASSERTF( !haveAnyShadersNotBeenDeleted,
				"If you are seeing this message there are %i shaders that haven't been deleted. You probably have a reference counting bug!", m_shaders.size() );
			eae6320::Logging::OutputError( "Not every shader was released. The program will now crash!" );
			constexpr int* const invalidAddress = 0x0;
			*invalidAddress = 123;	// Intentional crash! You have a bug that must be fixed!
		}
	}
}
