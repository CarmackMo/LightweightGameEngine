// Includes
//=========

#include "../cShaderBuilder.h"

#include <cstdlib>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/OpenGL/Includes.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/Windows/OpenGl.h>
#include <External/Mcpp/Includes.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <Tools/AssetBuildLibrary/Functions.h>

// Static Data
//============

namespace
{
	enum class eGlVendor
	{
		NVIDIA,
		AMD,
		Intel,

		Other
	};
	auto s_glVendor = eGlVendor::Other;
}

// Helper Declarations
//====================

namespace
{
	eae6320::cResult BuildAndVerifyGeneratedShaderSource( const char* const i_path_source, const char* const i_path_target,
		const eae6320::Graphics::eShaderType i_shaderType, const std::string& i_source );
	eae6320::cResult PreProcessShaderSource( const char* const i_path_source, std::string& o_shaderSource_preProcessed );
	eae6320::cResult SaveGeneratedShaderSource( const char* const i_path, const std::string& i_source );
}

// Interface
//==========

// Build
//------

eae6320::cResult eae6320::Assets::cShaderBuilder::Build( const Graphics::eShaderType i_shaderType, const std::vector<std::string>& i_arguments )
{
	auto result = Results::Success;

	std::string shaderSource_preProcessed;
	if ( !( result = PreProcessShaderSource( m_path_source, shaderSource_preProcessed ) ) )
	{
		return result;
	}
	if ( !( result = SaveGeneratedShaderSource( m_path_target, shaderSource_preProcessed ) ) )
	{
		return result;
	}
	if ( !( result = BuildAndVerifyGeneratedShaderSource( m_path_source, m_path_target, i_shaderType, shaderSource_preProcessed ) ) )
	{
		return result;
	}

	return result;
}

// Helper Definitions
//===================

namespace
{
	eae6320::cResult BuildAndVerifyGeneratedShaderSource( const char* const i_path_source, const char* const i_path_target,
		const eae6320::Graphics::eShaderType i_shaderType, const std::string& i_source )
	{
		auto result = eae6320::Results::Success;
		{
			HINSTANCE applicationInstance = NULL;
			eae6320::Windows::OpenGl::sHiddenWindowInfo hiddenWindowInfo;
			eae6320::cScopeGuard scopGuard_hiddenWindowInfo( [i_path_source, &result, &applicationInstance, &hiddenWindowInfo]
				{
					std::string errorMessage;
					const auto result_hiddenWindowInfo = eae6320::Windows::OpenGl::FreeHiddenContextWindow( applicationInstance, hiddenWindowInfo, &errorMessage );
					if ( !result_hiddenWindowInfo )
					{
						if ( result )
						{
							result = result_hiddenWindowInfo;
						}
						eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source, errorMessage.c_str() );
					}
				} );

			// Load any required OpenGL extensions
			{
				std::string errorMessage;
				if ( !( result = eae6320::OpenGlExtensions::Load( &errorMessage ) ) )
				{
					eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source, errorMessage.c_str() );
					return result;
				}
			}

			// Create a hidden OpenGL window
			{
				std::string errorMessage;
				if ( !( result = eae6320::Windows::OpenGl::CreateHiddenContextWindow( applicationInstance, hiddenWindowInfo, &errorMessage ) ) )
				{
					eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source, errorMessage.c_str() );
					return result;
				}
			}
			// Determine which vendor makes the GPU
			{
				const auto* const glString = glGetString( GL_VENDOR );
				const auto errorCode = glGetError();
				if ( glString && ( errorCode == GL_NO_ERROR ) )
				{
					const auto* const glVendor = reinterpret_cast<const char*>( glString );
					if ( strcmp( glVendor, "NVIDIA Corporation" ) == 0 )
					{
						s_glVendor = eGlVendor::NVIDIA;
					}
					else if ( strcmp( glVendor, "ATI Technologies Inc." ) == 0 )
					{
						s_glVendor = eGlVendor::AMD;
					}
					else if ( strcmp( glVendor, "Intel" ) == 0 )
					{
						s_glVendor = eGlVendor::Intel;
					}
					else
					{
						s_glVendor = eGlVendor::Other;
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					{
						std::ostringstream errorMessage;
						errorMessage << "OpenGL failed to return a string identifying the GPU vendor";
						if ( errorCode != GL_NO_ERROR )
						{
							errorMessage << ": " << reinterpret_cast<const char*>( gluErrorString( errorCode ) );
						}
						eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source, errorMessage.str().c_str() );
					}
					return result;
				}
			}
			// Verify that compiling shaders at run-time is supported
			{
				GLboolean isShaderCompilingSupported;
				glGetBooleanv( GL_SHADER_COMPILER, &isShaderCompilingSupported );
				if ( !isShaderCompilingSupported )
				{
					result = eae6320::Results::Failure;
					eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
						"Compiling shaders at run-time isn't supported on this implementation (this should never happen)" );
					return result;
				}
			}

			// Load the source code from file and set it into a shader
			GLuint shaderId = 0;
			eae6320::cScopeGuard scopeGuard_shaderId( [i_path_source, &result, &shaderId]
				{
					if ( shaderId != 0 )
					{
						glDeleteShader( shaderId );
						const auto errorCode = glGetError();
						if ( errorCode != GL_NO_ERROR )
						{
							if ( result )
							{
								result = eae6320::Results::Failure;
							}
							eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
								"OpenGL failed to delete the shader source code: %s", reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						}
						shaderId = 0;
					}
				} );
			{
				// Get the platform-specific shader type
				GLenum shaderType;
				switch ( i_shaderType )
				{
				case eae6320::Graphics::eShaderType::Vertex:
					shaderType = GL_VERTEX_SHADER;
					break;
				case eae6320::Graphics::eShaderType::Fragment:
					shaderType = GL_FRAGMENT_SHADER;
					break;
				default:
					result = eae6320::Results::Failure;
					eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
						"No OpenGL implementation exists for the platform-independent shader type %i", i_shaderType );
					return result;
				}
				// Generate a shader
				shaderId = glCreateShader( shaderType );
				{
					const auto errorCode = glGetError();
					if ( errorCode != GL_NO_ERROR )
					{
						result = eae6320::Results::Failure;
						eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
							"OpenGL failed to get an unused shader ID: %s", reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						return result;
					}
					else if ( shaderId == 0 )
					{
						result = eae6320::Results::Failure;
						eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source, "OpenGL failed to get an unused shader ID" );
						return result;
					}
				}
				// Set the source code into the shader
				{
					constexpr GLsizei shaderSourceCount = 1;
					const auto* const shaderSource = static_cast<const GLchar*>( i_source.c_str() );
					const auto shaderLength = static_cast<GLint>( i_source.length() );	// The NULL terminator isn't necessary
					glShaderSource( shaderId, shaderSourceCount, &shaderSource, &shaderLength );
					const auto errorCode = glGetError();
					if ( errorCode != GL_NO_ERROR )
					{
						result = eae6320::Results::Failure;
						eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
							"OpenGL failed to set the shader source code: %s", reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
						return result;
					}
				}
			}

			// Compile the shader source code
			{
				glCompileShader( shaderId );
				const auto errorCode = glGetError();
				if ( errorCode == GL_NO_ERROR )
				{
					// Get compilation info
					// (this won't be used unless compilation fails
					// but it can be useful to look at when debugging)
					std::string compilationInfo;
					{
						GLint infoSize;
						glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &infoSize );
						const auto errorCode = glGetError();
						if ( errorCode == GL_NO_ERROR )
						{
							if ( infoSize > 0 )
							{
								auto* const info = new (std::nothrow) GLchar[infoSize];
								if ( info )
								{
									eae6320::cScopeGuard scopeGuard_info( [info]
										{
											delete [] info;
										} );
									constexpr GLsizei* const dontReturnLength = nullptr;
									glGetShaderInfoLog( shaderId, static_cast<GLsizei>( infoSize ), dontReturnLength, info );
									const auto errorCode = glGetError();
									if ( errorCode == GL_NO_ERROR )
									{
										compilationInfo = info;
									}
									else
									{
										result = eae6320::Results::Failure;
										eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
											"OpenGL failed to get compilation info of the shader source code: %s",
											reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
										return result;
									}
								}
								else
								{
									result = eae6320::Results::OutOfMemory;
									eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
										"Windows failed to allocate space for compilation info of the shader source code" );
									return result;
								}
							}
						}
						else
						{
							result = eae6320::Results::Failure;
							eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
								"OpenGL failed to get the length of the shader compilation info: %s",
								reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
							return result;
						}
					}
					// Check to see if there were compilation errors
					GLint didCompilationSucceed;
					{
						glGetShaderiv( shaderId, GL_COMPILE_STATUS, &didCompilationSucceed );
						const auto errorCode = glGetError();
						if ( errorCode == GL_NO_ERROR )
						{
							if ( didCompilationSucceed == GL_FALSE )
							{
								result = eae6320::Results::Failure;

								// Convert the error messages into something that will be output in Visual Studio's Error window
								// (and that will open the file when double-clicked)
								if ( s_glVendor != eGlVendor::Other )
								{
									try
									{
										std::regex pattern_match;
										std::string pattern_replace;
										{
											switch ( s_glVendor )
											{
											case eGlVendor::NVIDIA:
												{
													pattern_match.assign( R"(^\s*0\s*\(\s*(\d+)\s*\))" );
													{
														std::ostringstream stringBuilder;
														stringBuilder << i_path_target << "($1)";
														pattern_replace = stringBuilder.str();
													}
												}
												break;
											case eGlVendor::AMD:
												{
													// The following code can be uncommented to try and debug AMD parsing on an NVIDIA machine
													/*
													{
														std::string fakeAmd;
														std::regex pattern_match( R"(^\s*0\s*\(\s*(\d+)\s*\)\s*:\s*(error|warning)\s*(\w+)\s*:)" );
														const std::string pattern_replace = "ERROR: 0:$1: $2(#$3)";
														fakeAmd = std::regex_replace( compilationInfo, pattern_match, pattern_replace );
														compilationInfo = fakeAmd;
													}
													*/

													pattern_match.assign( R"(^\s*\w+\s*:\s*0\s*:\s*(\d+)\s*:\s*(\w+)\s*\(\s*(\S+)\s*\))" );
													{
														std::ostringstream stringBuilder;
														stringBuilder << i_path_target << "($1) : $2 $3:";
														pattern_replace = stringBuilder.str();
													}
												}
												break;
											case eGlVendor::Intel:
												{
													pattern_match.assign( R"(^\s*(\w+)\s*:\s*0\s*:\s*(\d+)\s*:)" );
													{
														std::ostringstream stringBuilder;
														stringBuilder << i_path_target << "($2) : $1:";
														pattern_replace = stringBuilder.str();
													}
												}
											}
										}
										const auto convertedErrors = std::regex_replace( compilationInfo, pattern_match, pattern_replace );
										std::cerr << convertedErrors << std::endl;
									}
									catch ( std::regex_error& )
									{
										// If the parsing code has an error pass the output directly from the driver
										std::cerr << compilationInfo << std::endl;
									}
								}
								else
								{
									std::cerr << compilationInfo << std::endl;
								}
								return result;
							}
						}
						else
						{
							result = eae6320::Results::Failure;
							eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
								"OpenGL failed to find out if compilation of the shader source code succeeded: %s",
								reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
							return result;
						}
					}
				}
				else
				{
					result = eae6320::Results::Failure;
					eae6320::Assets::OutputErrorMessageWithFileInfo( i_path_source,
						"OpenGL failed to compile the shader source code: %s", reinterpret_cast<const char*>( gluErrorString( errorCode ) ) );
					return result;
				}
			}
		}
		return result;
	}

	eae6320::cResult PreProcessShaderSource( const char* const i_path_source, std::string& o_shaderSource_preProcessed )
	{
		auto result = eae6320::Results::Success;

		// Get the content directories to use as #include search paths
		std::string includeSearchPathArgument_engineSourceContentDir, includeSearchPathArgument_gameSourceContentDir;
		{
			const std::string prefix = "-I";
			std::string errorMessage;

			// EngineSourceContentDir
			{
				if ( result = eae6320::Platform::GetEnvironmentVariable(
					"EngineSourceContentDir", includeSearchPathArgument_engineSourceContentDir, &errorMessage ) )
				{
					includeSearchPathArgument_engineSourceContentDir = prefix + includeSearchPathArgument_engineSourceContentDir;
				}
				else
				{
					eae6320::Assets::OutputErrorMessage( "Failed to get the engine's source content directory: %s", errorMessage.c_str() );
					return result;
				}
			}
			// GameSourceContentDir
			{
				if ( result = eae6320::Platform::GetEnvironmentVariable(
					"GameSourceContentDir", includeSearchPathArgument_gameSourceContentDir, &errorMessage ) )
				{
					includeSearchPathArgument_gameSourceContentDir = prefix + includeSearchPathArgument_gameSourceContentDir;
				}
				else
				{
					eae6320::Assets::OutputErrorMessage( "Failed to get the game's source content directory: %s", errorMessage.c_str() );
					return result;
				}
			}
		}

		// mcpp consumes non-const char*s
		// and so an array of temporary strings must be allocated
		const char* arguments_const [] =
		{
			// The command (necessary even though it is being run as a function)
			"mcpp",
			// The platform #define
			"-DEAE6320_PLATFORM_GL",
			// The #include search paths
			// (list the game's content path before the engine's
			// so that if there are identically-named files
			// the game-specific one will be used)
			includeSearchPathArgument_gameSourceContentDir.c_str(),
			includeSearchPathArgument_engineSourceContentDir.c_str(),
#ifdef EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
			// Keep comments
			"-C",
#endif
			// Don't output #line number information
			"-P",
			// Treat unknown directives (like #version and #extension) as warnings instead of errors
			"-a",
			// The input file to pre-process
			i_path_source
		};
		const auto argumentCount = sizeof( arguments_const ) / sizeof( char* );
		char* arguments[argumentCount] = {};
		std::remove_const<decltype( argumentCount )>::type allocatedArgumentCount = 0;
		eae6320::cScopeGuard scopeGuard_arguments( [&arguments, &allocatedArgumentCount]
			{
				for ( size_t i = 0; i < allocatedArgumentCount; ++i )
				{
					if ( arguments[i] )
					{
						free( arguments[i] );
						arguments[i] = nullptr;
					}
				}
			} );
		for ( size_t i = 0; i < argumentCount; ++i )
		{
			const auto stringSize = strlen( arguments_const[i] ) + 1;	// NULL terminator
			char* temporaryString = reinterpret_cast<char*>( malloc( stringSize ) );
			if ( temporaryString )
			{
				memcpy( temporaryString, arguments_const[i], stringSize );
				arguments[i] = temporaryString;
			}
			else
			{
				result = eae6320::Results::OutOfMemory;
				eae6320::Assets::OutputErrorMessage( "Failed to allocate &u bytes of temporary storage for argument #&u (\"%s\")",
					stringSize, i, arguments_const[i] );
				return result;
			}
		}

		// Configure mcpp to output to buffers instead of files
		mcpp_use_mem_buffers( 1 );
		eae6320::cScopeGuard scopeGuard_mcppBuffers( []
			{
				// Release mcpp's internal buffers
				mcpp_use_mem_buffers( 0 );
			} );
		// Preprocess the file
		{
			const auto result_mcpp = mcpp_lib_main( static_cast<int>( argumentCount ), arguments );
			if ( result_mcpp == EXIT_SUCCESS )
			{
				const auto* const shaderSource_preProcessed = mcpp_get_mem_buffer( static_cast<OUTDEST>( eae6320::mcpp::OUTDEST::Out ) );
				o_shaderSource_preProcessed = shaderSource_preProcessed ? shaderSource_preProcessed : "";
			}
			else
			{
				result = eae6320::Results::Failure;
				std::cerr << mcpp_get_mem_buffer( static_cast<OUTDEST>( eae6320::mcpp::OUTDEST::Err ) );
				return result;
			}
		}

#ifndef EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
		// Remove extra new lines
		try
		{
			std::regex pattern_match( R"(((\r\n)|(\n))+)" );
			const std::string pattern_replace( "\n" );
			o_shaderSource_preProcessed = std::regex_replace( o_shaderSource_preProcessed, pattern_match, pattern_replace );
		}
		catch ( std::regex_error& )
		{

		}
#endif

		return result;
	}

	eae6320::cResult SaveGeneratedShaderSource( const char* const i_path, const std::string& i_shader )
	{
		eae6320::cResult result;
		std::string errorMessage;
		if ( !( result = eae6320::Platform::WriteBinaryFile( i_path, i_shader.c_str(), i_shader.length(), &errorMessage ) ) )
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo( i_path, errorMessage.c_str() );
		}
		return result;
	}
}
