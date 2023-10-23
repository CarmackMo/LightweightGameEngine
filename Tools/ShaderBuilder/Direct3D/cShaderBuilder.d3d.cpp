// Includes
//=========

#include "../cShaderBuilder.h"

#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <iostream>
#include <limits>
#include <Tools/AssetBuildLibrary/Functions.h>

// Helper Class Declaration
//=========================

namespace
{
	class cIncludeHelper final : public ID3DInclude
	{
		// Interface
		//----------

	public:

		eae6320::cResult Initialize();
		cIncludeHelper( const std::string& i_shaderSourcePath );

		// Inherited Interface
		//--------------------

	private:

		HRESULT Open( D3D_INCLUDE_TYPE i_includeType, const char* i_path, const void* i_parentData,
			const void** o_dataFromFile, unsigned int* o_dataSize ) final;
		HRESULT Close( const void* i_data ) final;

		// Data
		//-----

	private:

		const std::string m_shaderSourcePath;
		std::string m_shaderSourceDirectory;
		std::string m_engineSourceContentDirectory;
		std::string m_gameSourceContentDirectory;
	};
}

// Interface
//==========

// Build
//------

eae6320::cResult eae6320::Assets::cShaderBuilder::Build( const Graphics::eShaderType i_shaderType, const std::vector<std::string>& i_arguments )
{
	auto result = Results::Success;

	Platform::sDataFromFile dataFromFile;
	ID3DBlob* compiledCode = nullptr;
	ID3DBlob* disassembledCode = nullptr;
	cScopeGuard scopeGuard( [&compiledCode, &disassembledCode, &dataFromFile]
		{
			if ( compiledCode )
			{
				compiledCode->Release();
				compiledCode = nullptr;
			}
			if ( disassembledCode )
			{
				disassembledCode->Release();
				disassembledCode = nullptr;
			}
		} );

	// Load the source code
	{
		std::string errorMessage;
		if ( !( result = Platform::LoadBinaryFile( m_path_source, dataFromFile, &errorMessage ) ) )
		{
			OutputErrorMessageWithFileInfo( m_path_source, "Failed to load shader source file: %s", errorMessage.c_str() );
			return result;
		}
	}
	// Compile it
	{
		const D3D_SHADER_MACRO defines[] =
		{
			{ "EAE6320_PLATFORM_D3D" },
			{}
		};
		cIncludeHelper includeHelper( m_path_source );
		if ( !( result = includeHelper.Initialize() ) )
		{
			return result;
		}
		constexpr auto entryPoint = "main";
		const auto* const targetProfile = [this, i_shaderType]
		{
			switch ( i_shaderType )
			{
			case Graphics::eShaderType::Vertex:
				return "vs_4_0";
				break;
			case Graphics::eShaderType::Fragment:
				return "ps_4_0";
				break;
			default:
				EAE6320_ASSERTF( false, "Invalid shader type" );
				eae6320::Assets::OutputErrorMessageWithFileInfo( m_path_source,
					"No Direct3D implementation exists for the platform-independent shader type %i", i_shaderType );
				return "";
			}
		}();
		constexpr unsigned int compileConstants = 0
#ifdef EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
			// Include debugging information
			| D3DCOMPILE_DEBUG
			// Disable optimizations
			| D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_OPTIMIZATION_LEVEL0
#else
			// Optimize as much as possible
			| D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif
			// Treat warnings as errors
			| D3DCOMPILE_WARNINGS_ARE_ERRORS
			;
		constexpr unsigned int notAnFxFile = 0;
		ID3DBlob* errorMessages = nullptr;
		const auto result_compile = D3DCompile( dataFromFile.data, dataFromFile.size, m_path_source, defines, &includeHelper, entryPoint,
			targetProfile, compileConstants, notAnFxFile, &compiledCode, &errorMessages );
		if ( errorMessages )
		{
			std::cerr << static_cast<char*>( errorMessages->GetBufferPointer() );
			errorMessages->Release();
			errorMessages = nullptr;
		}
		else if ( FAILED( result_compile ) )
		{
			OutputErrorMessageWithFileInfo( m_path_source, "Shader compiling failed for unknown reason" );
		}
		if ( FAILED( result_compile ) )
		{
			result = Results::Failure;
			return result;
		}
	}
	// Write the compiled shader to disk
	{
		std::string errorMessage;
		if ( !( result = eae6320::Platform::WriteBinaryFile( m_path_target, compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(), &errorMessage ) ) )
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo( m_path_source, errorMessage.c_str() );
		}
	}
	// Create a disassembled version of the shader
	if ( result )
	{
		// The following can be #defined
		// in order to create a human-readable file with the shader instructions
		// (this can be useful during development to see how code changes affect instruction count)
#define EAE6320_GRAPHICS_SHOULDDISASSEMBLYBECREATED
#ifdef EAE6320_GRAPHICS_SHOULDDISASSEMBLYBECREATED
		const auto path_disassembly = [i_shaderType]( const std::string& i_path_compiledShader )
		{
			const auto directory_disassembly = [i_shaderType, &i_path_compiledShader]()
			{
				std::string directory_disassembly;
				if ( !eae6320::Platform::GetEnvironmentVariable( "IntermediateDir", directory_disassembly ) )
				{
					const auto pos_lastSlash = i_path_compiledShader.find_last_of( '/' );
					if ( pos_lastSlash != i_path_compiledShader.npos )
					{
						directory_disassembly = i_path_compiledShader.substr( 0, pos_lastSlash + 1 );
					}
				}
				switch ( i_shaderType )
				{
				case Graphics::eShaderType::Vertex:
					directory_disassembly += "vertex/";
					break;
				case Graphics::eShaderType::Fragment:
					directory_disassembly += "fragment/";
					break;
				}
				return directory_disassembly;
			}();
			const auto filename = [&i_path_compiledShader]()
			{
				const auto filename_withExtension = [&i_path_compiledShader]
				{
					const auto pos_lastSlash = i_path_compiledShader.find_last_of( '/' );
					if ( pos_lastSlash != i_path_compiledShader.npos )
					{
						return i_path_compiledShader.substr( pos_lastSlash + 1 );
					}
					else
					{
						return i_path_compiledShader;
					}
				}();
				return filename_withExtension + ".dasm";
			}();
			return directory_disassembly + filename;
		}( m_path_target );

		constexpr unsigned int disassembleConstants = 0;
		constexpr char* const noComment = nullptr;
		if ( SUCCEEDED( D3DDisassemble( compiledCode->GetBufferPointer(), compiledCode->GetBufferSize(),
			disassembleConstants, noComment, &disassembledCode ) ) )
		{
			std::string errorMessage;
			if ( !eae6320::Platform::CreateDirectoryIfItDoesntExist( path_disassembly, &errorMessage ) )
			{
				eae6320::Assets::OutputWarningMessageWithFileInfo( path_disassembly.c_str(), errorMessage.c_str() );
			}
			if ( !eae6320::Platform::WriteBinaryFile( path_disassembly.c_str(),
				disassembledCode->GetBufferPointer(), disassembledCode->GetBufferSize(), &errorMessage ) )
			{
				eae6320::Assets::OutputWarningMessageWithFileInfo( path_disassembly.c_str(), errorMessage.c_str() );
			}
		}
		else
		{
			OutputWarningMessageWithFileInfo( path_disassembly.c_str(), "Failed to disassemble shader" );
		}
#endif
	}

	return result;
}

// Helper Class Definition
//========================

namespace
{
	// Interface
	//----------

	eae6320::cResult cIncludeHelper::Initialize()
	{
		auto result = eae6320::Results::Success;

		// Calculate the directory of the source file
		{
			const auto pos_lastSlash = m_shaderSourcePath.find_last_of( "/\\" );
			if ( pos_lastSlash != m_shaderSourcePath.npos )
			{
				m_shaderSourceDirectory = m_shaderSourcePath.substr( 0, pos_lastSlash + 1 );
			}
			else
			{
				m_shaderSourceDirectory = "";
			}
		}
		// Get the content directories to use as #include search paths
		{
			std::string errorMessage;

			// EngineSourceContentDir
			{
				if ( !( result = eae6320::Platform::GetEnvironmentVariable(
					"EngineSourceContentDir", m_engineSourceContentDirectory, &errorMessage ) ) )
				{
					eae6320::Assets::OutputErrorMessage( "Failed to get the engine's source content directory: %s", errorMessage.c_str() );
					return result;
				}
			}
			// GameSourceContentDir
			{
				if ( !( result = eae6320::Platform::GetEnvironmentVariable(
					"GameSourceContentDir", m_gameSourceContentDirectory, &errorMessage ) ) )
				{
					eae6320::Assets::OutputErrorMessage( "Failed to get the game's source content directory: %s", errorMessage.c_str() );
					return result;
				}
			}
		}

		return result;
	}

	cIncludeHelper::cIncludeHelper( const std::string& i_shaderSourcePath )
		:
		m_shaderSourcePath( i_shaderSourcePath )
	{

	}

	// Inherited Interface
	//--------------------

	HRESULT cIncludeHelper::Open( D3D_INCLUDE_TYPE i_includeType, const char* i_path, const void* i_parentData,
		const void** o_dataFromFile, unsigned int* o_dataSize )
	{
		auto result = S_OK;

		EAE6320_ASSERT( o_dataFromFile != nullptr );
		EAE6320_ASSERT( o_dataSize != nullptr );

		eae6320::Platform::sDataFromFile dataFromFile;
		eae6320::cScopeGuard scopeGuard( [o_dataFromFile, o_dataSize, &result, &dataFromFile]
			{
				if ( SUCCEEDED( result ) )
				{
					EAE6320_ASSERT( dataFromFile.data != nullptr );
					EAE6320_ASSERT( *o_dataFromFile == nullptr );				
					*o_dataFromFile = dataFromFile.data;
					dataFromFile.data = nullptr;
					EAE6320_ASSERT( dataFromFile.size < std::numeric_limits<std::remove_pointer<decltype( o_dataSize )>::type>::max() );
					*o_dataSize = static_cast<std::remove_pointer<decltype( o_dataSize )>::type>( dataFromFile.size );
					dataFromFile.size = 0;
				}
				else
				{
					*o_dataFromFile = nullptr;
					*o_dataSize = 0;
				}
			} );

		// Calculate the path to open
		std::string pathToOpen;
		{
			// BUG WARNING!
			// The code below assumes that the #include path is relative,
			// and it will not work correctly for absolute paths
			switch ( i_includeType )
			{
			case D3D_INCLUDE_LOCAL:
				{
					pathToOpen = m_shaderSourceDirectory + i_path;
					if ( !eae6320::Platform::DoesFileExist( pathToOpen.c_str() ) )
					{
						result = ERROR_PATH_NOT_FOUND;
						eae6320::Assets::OutputErrorMessageWithFileInfo( m_shaderSourcePath.c_str(),
							"The file %s doesn't exist (from the requested #include path \"%s\"", pathToOpen.c_str(), i_path );
						return result;
					}
				}
				break;
			case D3D_INCLUDE_SYSTEM:
				{
					// Files in the game source content directory take precedence over those in the engine source content directory
					pathToOpen = m_gameSourceContentDirectory + i_path;
					if ( !eae6320::Platform::DoesFileExist( pathToOpen.c_str() ) )
					{
						pathToOpen = m_engineSourceContentDirectory + i_path;
						if ( !eae6320::Platform::DoesFileExist( pathToOpen.c_str() ) )
						{
							result = ERROR_PATH_NOT_FOUND;
							eae6320::Assets::OutputErrorMessageWithFileInfo( m_shaderSourcePath.c_str(),
								"The file %s doesn't exist (from the requested #include path \"%s\"", pathToOpen.c_str(), i_path );
							return result;
						}
					}
				}
				break;
			}
		}

		{
			std::string errorMessage;
			const auto result_load = eae6320::Platform::LoadBinaryFile( pathToOpen.c_str(), dataFromFile, &errorMessage );
			if ( !result_load )
			{
				result = E_FAIL;
				eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__,
					"Failed to open requested #include path: %s",
					errorMessage.c_str() );
				return result;
			}
		}

		return result;
	}

	HRESULT cIncludeHelper::Close( const void* i_data )
	{
		EAE6320_ASSERT( i_data );

		// This data should be from the Open() function:
		// Assign it back to the platform data structure
		// so that it can be deallocated consistently
		eae6320::Platform::sDataFromFile dataFromFile;
		dataFromFile.data = const_cast<void*>( i_data );
		dataFromFile.Free();

		return S_OK;
	}
}
