// Includes
//=========

#include "Functions.h"

#include <cstdarg>
#include <cstdio>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <External/Lua/Includes.h>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Functions.h>
#endif

// Helper Class Declaration
//=========================

namespace
{
	class cLuaState
	{
		// Interface
		//----------

	public:

		// Lua Functions
		eae6320::cResult BuildAssets( const char* const i_path_assetsToBuild );
		eae6320::cResult ConvertSourceRelativePathToBuiltRelativePath( const char* const i_sourceRelativePath, const char* const i_assetType,
			std::string& o_builtRelativePath, std::string* o_errorMessage );

		// Access
		lua_State* Get();

		// Initialize / Clean Up
		~cLuaState();

		// Data
		//-----

	private:

		lua_State* luaState = nullptr;

		// References to Lua functions that are saved in the Lua "registry"
		int m_fBuildAssets = LUA_NOREF;
		int m_fConvertSourceRelativePathToBuiltRelativePath = LUA_NOREF;

		// Implementation
		//---------------

	private:

		// Initialize / Clean Up
		eae6320::cResult Initialize();
		eae6320::cResult CleanUp();

		eae6320::cResult SaveReferenceToGlobalFunctionInRegistry( const char* const i_functionName, int &o_functionReference );
	};
}

// Static Data
//============

namespace
{
	cLuaState s_luaState;
}

// Helper Declarations
//====================

namespace
{
	// Error / Warning Output
	//-----------------------

	eae6320::cResult FormatErrorOrWarningMessage( const char* const i_message, std::string &o_message, va_list io_insertions );

	void OutputErrorMessage_platformSpecific( const char* const i_errorMessage, const char* const i_optionalFilePath,
		const unsigned int* const i_optionalLineNumber, const unsigned int* const i_optionalColumnNumber );
	void OutputWarningMessage_platformSpecific( const char* const i_warningMessage, const char* const i_optionalFilePath,
		const unsigned int* const i_optionalLineNumber, const unsigned int* const i_optionalColumnNumber );

	// Lua Wrapper Functions
	//----------------------

	int luaCopyFile( lua_State* io_luaState );
	int luaCreateDirectoryIfItDoesntExist( lua_State* io_luaState );
	int luaDoesFileExist( lua_State* io_luaState );
	int luaExecuteCommand( lua_State* io_luaState );
	int luaGetEnvironmentVariable( lua_State* io_luaState );
	int LuaGetFilesInDirectory( lua_State* io_luaState );
	int luaGetLastWriteTime( lua_State* io_luaState );
	int luaInvalidateLastWriteTime( lua_State* io_luaState );
	int luaOutputErrorMessage( lua_State* io_luaState );
	int luaOutputWarningMessage( lua_State* io_luaState );
}

// Interface
//==========

eae6320::cResult eae6320::Assets::BuildAssets( const char* const i_path_assetsToBuild )
{
	return s_luaState.BuildAssets( i_path_assetsToBuild );
}

eae6320::cResult eae6320::Assets::ConvertSourceRelativePathToBuiltRelativePath( const char* const i_sourceRelativePath, const char* const i_assetType,
	std::string& o_builtRelativePath, std::string* o_errorMessage )
{
	return s_luaState.ConvertSourceRelativePathToBuiltRelativePath( i_sourceRelativePath, i_assetType, o_builtRelativePath, o_errorMessage );
}

// Error / Warning Output
//-----------------------

void eae6320::Assets::OutputErrorMessage( const char* const i_errorMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_errorMessage );
		result = FormatErrorOrWarningMessage( i_errorMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		const char* const noPath = nullptr;
		const unsigned int* const noFileInfo = nullptr;
		OutputErrorMessage_platformSpecific( formattedMessage.c_str(), noPath, noFileInfo, noFileInfo );
	}
}
void eae6320::Assets::OutputErrorMessageWithFileInfo( const char* const i_filePath,
	const char* const i_errorMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_errorMessage );
		result = FormatErrorOrWarningMessage( i_errorMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		const unsigned int* const noFileInfo = nullptr;
		OutputErrorMessage_platformSpecific( formattedMessage.c_str(), i_filePath, noFileInfo, noFileInfo );
	}
}
void eae6320::Assets::OutputErrorMessageWithFileInfo( const char* const i_filePath,
	const unsigned int i_lineNumber,
	const char* const i_errorMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_errorMessage );
		result = FormatErrorOrWarningMessage( i_errorMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		const unsigned int* const noFileInfo = nullptr;
		OutputErrorMessage_platformSpecific( formattedMessage.c_str(), i_filePath, &i_lineNumber, noFileInfo );
	}
}
void eae6320::Assets::OutputErrorMessageWithFileInfo( const char* const i_filePath,
	const unsigned int i_lineNumber, const unsigned int i_columnNumber,
	const char* const i_errorMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_errorMessage );
		result = FormatErrorOrWarningMessage( i_errorMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		OutputErrorMessage_platformSpecific( formattedMessage.c_str(), i_filePath, &i_lineNumber, &i_columnNumber );
	}
}

void eae6320::Assets::OutputWarningMessage( const char* const i_warningMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_warningMessage );
		result = FormatErrorOrWarningMessage( i_warningMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		const char* const noPath = nullptr;
		const unsigned int* const noFileInfo = nullptr;
		OutputWarningMessage_platformSpecific( formattedMessage.c_str(), noPath, noFileInfo, noFileInfo );
	}
}
void eae6320::Assets::OutputWarningMessageWithFileInfo( const char* const i_filePath,
	const char* const i_warningMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_warningMessage );
		result = FormatErrorOrWarningMessage( i_warningMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		const unsigned int* const noFileInfo = nullptr;
		OutputWarningMessage_platformSpecific( formattedMessage.c_str(), i_filePath, noFileInfo, noFileInfo );
	}
}
void eae6320::Assets::OutputWarningMessageWithFileInfo( const char* const i_filePath,
	const unsigned int i_lineNumber,
	const char* const i_warningMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_warningMessage );
		result = FormatErrorOrWarningMessage( i_warningMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		const unsigned int* const noFileInfo = nullptr;
		OutputWarningMessage_platformSpecific( formattedMessage.c_str(), i_filePath, &i_lineNumber, noFileInfo );
	}
}
void eae6320::Assets::OutputWarningMessageWithFileInfo( const char* const i_filePath,
	const unsigned int i_lineNumber, const unsigned int i_columnNumber,
	const char* const i_warningMessage, ... )
{
	std::string formattedMessage;
	eae6320::cResult result;
	{
		va_list insertions;
		va_start( insertions, i_warningMessage );
		result = FormatErrorOrWarningMessage( i_warningMessage, formattedMessage, insertions );
		va_end( insertions );
	}
	if ( result )
	{
		OutputWarningMessage_platformSpecific( formattedMessage.c_str(), i_filePath, &i_lineNumber, &i_columnNumber );
	}
}

// Helper Class Declaration
//=========================

namespace
{
	// Interface
	//----------

	// Lua Functions

	eae6320::cResult cLuaState::BuildAssets( const char* const i_path_assetsToBuild )
	{
		auto result = eae6320::Results::Success;

		eae6320::cScopeGuard scopeGuard( [this]
			{
				EAE6320_ASSERT( !luaState || ( lua_gettop( luaState ) == 0 ) );
			} );

		// Initialize the Lua environment if necessary
		if ( luaState || ( result = Initialize() ) )
		{
			EAE6320_ASSERT( lua_gettop( luaState ) == 0 );

			// Push the Lua function on the stack
			EAE6320_ASSERT( ( m_fBuildAssets != LUA_NOREF ) && ( m_fBuildAssets != LUA_REFNIL ) );
			const auto type = lua_rawgeti( luaState, LUA_REGISTRYINDEX, m_fBuildAssets );
			if ( type == LUA_TFUNCTION )
			{
				// Call the function
				constexpr int returnValueCount = 1;
				{
					constexpr int argumentCount = 1;
					{
						lua_pushstring( luaState, i_path_assetsToBuild );
					}
					constexpr int noErrorHandler = 0;
					const auto luaResult = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
					if ( luaResult != LUA_OK )
					{
						// The error message should already be formatted for output
						std::cerr << lua_tostring( luaState, -1 ) << "\n";
						// Pop the error message
						lua_pop( luaState, 1 );
						switch ( luaResult )
						{
						case LUA_ERRMEM:
							{
								result = eae6320::Results::OutOfMemory;
							}
							break;
						default:
							{
								result = eae6320::Results::Failure;
							}
						}

						return result;
					}
				}
				// Check the return values
				{
					if ( lua_isboolean( luaState, -1 ) )
					{
						result = lua_toboolean( luaState, -1 ) ? eae6320::Results::Success : eae6320::Results::Failure;
					}
					else
					{
						result = eae6320::Results::Failure;
						{
							eae6320::Assets::OutputErrorMessage( "BuildAssets() should return a boolean as return value #1, not a %s",
								luaL_typename( luaState, -1 ) );
						}
					}
					// Pop the returned values
					lua_pop( luaState, returnValueCount );
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__,
					"The reference for the Lua BuildAssets() function returned a %s instead of a function."
					" This shouldn't happen if the asset build library was initialized successfully",
					lua_typename( luaState, type ) );
				return result;
			}
		}

		return result;
	}

	eae6320::cResult cLuaState::ConvertSourceRelativePathToBuiltRelativePath( const char* const i_sourceRelativePath, const char* const i_assetType,
		std::string& o_builtRelativePath, std::string* o_errorMessage )
	{
		auto result = eae6320::Results::Success;

		eae6320::cScopeGuard scopeGuard( [this]
			{
				EAE6320_ASSERT( !luaState || ( lua_gettop( luaState ) == 0 ) );
			} );

		// Initialize the Lua environment if necessary
		if ( luaState || ( result = Initialize() ) )
		{
			EAE6320_ASSERT( lua_gettop( luaState ) == 0 );

			// Push the Lua function on the stack
			EAE6320_ASSERT( ( m_fConvertSourceRelativePathToBuiltRelativePath != LUA_NOREF ) && ( m_fConvertSourceRelativePathToBuiltRelativePath != LUA_REFNIL ) );
			const auto type = lua_rawgeti( luaState, LUA_REGISTRYINDEX, m_fConvertSourceRelativePathToBuiltRelativePath );
			if ( type == LUA_TFUNCTION )
			{
				// Call the function
				constexpr int returnValueCount = 2;
				{
					constexpr int argumentCount = 2;
					{
						lua_pushstring( luaState, i_sourceRelativePath );
						lua_pushstring( luaState, i_assetType );
					}
					constexpr int noErrorHandler = 0;
					const auto luaResult = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
					if ( luaResult != LUA_OK )
					{
						// The error message should already be formatted for output
						std::cerr << lua_tostring( luaState, -1 ) << std::endl;
						// Pop the error message
						lua_pop( luaState, 1 );
						switch ( luaResult )
						{
						case LUA_ERRMEM:
							{
								result = eae6320::Results::OutOfMemory;
							}
							break;
						default:
							{
								result = eae6320::Results::Failure;
							}
						}

						return result;
					}
				}
				// Check the return values
				{
					if ( lua_isboolean( luaState, -2 ) )
					{
						const auto wereThereErrors = !lua_toboolean( luaState, -2 );
						if ( !wereThereErrors )
						{
							if ( lua_isstring( luaState, -1 ) )
							{
								o_builtRelativePath = lua_tostring( luaState, -1 );
							}
							else
							{
								result = eae6320::Results::Failure;
								if ( o_errorMessage )
								{
									std::ostringstream errorMessage;
									errorMessage << "ConvertSourceRelativePathToBuiltRelativePath() returned success but then a "
										<< luaL_typename( luaState, -1 ) << " instead of a string";
									*o_errorMessage = errorMessage.str();
								}
								else
								{
									eae6320::Assets::OutputErrorMessage(
										"ConvertSourceRelativePathToBuiltRelativePath() returned success but then a %s instead of a string",
										luaL_typename( luaState, -1 ) );
								}
							}
						}
						else
						{
							result = eae6320::Results::Failure;
							if ( lua_isstring( luaState, -1 ) )
							{
								if ( o_errorMessage )
								{
									*o_errorMessage = lua_tostring( luaState, -1 );
								}
								else
								{
									eae6320::Assets::OutputErrorMessage( lua_tostring( luaState, -1 ) );
								}
							}
							else
							{
								if ( o_errorMessage )
								{
									std::ostringstream errorMessage;
									errorMessage << "On failure ConvertSourceRelativePathToBuiltRelativePath() should return a string error message as return value #2, not a "
										<< luaL_typename( luaState, -1 );
									*o_errorMessage = errorMessage.str();
								}
								else
								{
									eae6320::Assets::OutputErrorMessage(
										"On failure ConvertSourceRelativePathToBuiltRelativePath() should return a string error message as return value #2, not a %s",
										luaL_typename( luaState, -1 ) );
								}
							}
						}
					}
					else
					{
						result = eae6320::Results::Failure;
						if ( o_errorMessage )
						{
							std::ostringstream errorMessage;
							errorMessage << "On failure ConvertSourceRelativePathToBuiltRelativePath() should return a boolean as return value #1, not a "
								<< luaL_typename( luaState, -2 );
							*o_errorMessage = errorMessage.str();
						}
						else
						{
							eae6320::Assets::OutputErrorMessage( "ConvertSourceRelativePathToBuiltRelativePath() should return a boolean as return value #1, not a %s",
								luaL_typename( luaState, -2 ) );
						}
					}
					// Pop the returned values
					lua_pop( luaState, returnValueCount );
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				if ( o_errorMessage )
				{
					std::ostringstream errorMessage;
					errorMessage << "The reference for the Lua ConvertSourceRelativePathToBuiltRelativePath() function returned a "
						<< lua_typename( luaState, type ) << " instead of a function."
						" This shouldn't happen if the asset build library was initialized successfully";
					*o_errorMessage = errorMessage.str();
				}
				else
				{
					eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__,
						"The reference for the Lua ConvertSourceRelativePathToBuiltRelativePath() function returned a %s instead of a function."
						" This shouldn't happen if the asset build library was initialized successfully",
						lua_typename( luaState, type ) );
				}
				return result;
			}
		}

		return result;
	}

	// Access

	lua_State *cLuaState::Get()
	{
		if ( !luaState )
		{
			Initialize();
		}
		return luaState;
	}

	// Initialize / Clean Up

	cLuaState::~cLuaState()
	{
		CleanUp();
	}

	// Implementation
	//---------------

	// Initialize / Clean Up

	eae6320::cResult cLuaState::Initialize()
	{
		auto result = eae6320::Results::Success;

		eae6320::cScopeGuard scopeGuard( [this, &result]
			{
				if ( luaState )
				{
					EAE6320_ASSERT( lua_gettop( luaState ) == 0 );
					if ( !result )
					{
						CleanUp();
					}
				}
			} );

		// Create a new Lua state
		{
			luaState = luaL_newstate();
			if ( !luaState )
			{
				result = eae6320::Results::OutOfMemory;
				eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__,
					"Memory allocation error creating Lua state" );
				return result;
			}
		}
		// Open the standard libraries
		luaL_openlibs( luaState );
		// Register the custom functions
		{
			lua_register( luaState, "CopyFile", luaCopyFile );
			lua_register( luaState, "CreateDirectoryIfItDoesntExist", luaCreateDirectoryIfItDoesntExist );
			lua_register( luaState, "DoesFileExist", luaDoesFileExist );
			lua_register( luaState, "ExecuteCommand", luaExecuteCommand );
			lua_register( luaState, "GetEnvironmentVariable", luaGetEnvironmentVariable );
			lua_register( luaState, "GetFilesInDirectory", LuaGetFilesInDirectory );
			lua_register( luaState, "GetLastWriteTime", luaGetLastWriteTime );
			lua_register( luaState, "InvalidateLastWriteTime", luaInvalidateLastWriteTime );
			lua_register( luaState, "OutputErrorMessage", luaOutputErrorMessage );
			lua_register( luaState, "OutputWarningMessage", luaOutputWarningMessage );
		}
		// Set the platform #defines
		{
		#if defined( EAE6320_PLATFORM_WINDOWS )
			lua_pushboolean( luaState, true );
			lua_setglobal( luaState, "EAE6320_PLATFORM_WINDOWS" );

			#if defined( EAE6320_PLATFORM_D3D )
				lua_pushboolean( luaState, true );
				lua_setglobal( luaState, "EAE6320_PLATFORM_D3D" );
			#elif defined( EAE6320_PLATFORM_GL )
				lua_pushboolean( luaState, true );
				lua_setglobal( luaState, "EAE6320_PLATFORM_GL" );
			#endif
		#endif
		}
		// Load the Lua asset build functions
		{
			// Get the path
			std::string path;
			{
				// Get the output directory
				{
					constexpr char* const key = "OutputDir";
					std::string errorMessage;
					if ( !( result = eae6320::Platform::GetEnvironmentVariable( key, path, &errorMessage ) ) )
					{
						eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__, errorMessage.c_str() );
						return result;
					}
				}
				path += "AssetBuildFunctions.lua";
			}
			// Load and execute it
			{
				std::string errorMessage;
				if ( eae6320::Platform::DoesFileExist( path.c_str(), &errorMessage ) )
				{
					// Load the build script file as a "chunk",
					// meaning there will be a callable function at the top of the stack
					const auto luaResult = luaL_loadfile( luaState, path.c_str() );
					if ( luaResult == LUA_OK )
					{
						// Execute the "chunk" so that the Lua functions are available in the Lua environment
						constexpr int argumentCount = 0;
						constexpr int returnValueCount = 0;
						constexpr int noErrorHandler = 0;
						const auto luaResult = lua_pcall( luaState, argumentCount, returnValueCount, noErrorHandler );
						if ( luaResult != LUA_OK )
						{
							// The error message should already be formatted for output
							std::cerr << lua_tostring( luaState, -1 ) << std::endl;
							// Pop the error message
							lua_pop( luaState, 1 );
							switch ( luaResult )
							{
							case LUA_ERRMEM:
								{
									result = eae6320::Results::OutOfMemory;
								}
								break;
							default:
								{
									result = eae6320::Results::InvalidFile;
								}
							}
							return result;
						}
					}
					else
					{
						// The error message should already be formatted for output
						std::cerr << lua_tostring( luaState, -1 ) << std::endl;
						// Pop the error message
						lua_pop( luaState, 1 );
						switch ( luaResult )
						{
						case LUA_ERRMEM:
							{
								result = eae6320::Results::OutOfMemory;
							}
							break;
						default:
							{
								result = eae6320::Results::Failure;
							}
						}
						return result;
					}
				}
				else
				{
					result = eae6320::Results::FileDoesntExist;
					eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__, errorMessage.c_str() );
					return result;
				}
			}
		}
		// Save references to the external Lua functions
		{
			if ( !( result = SaveReferenceToGlobalFunctionInRegistry( "BuildAssets", m_fBuildAssets ) ) )
			{
				return result;
			}
			if ( !( result = SaveReferenceToGlobalFunctionInRegistry( "ConvertSourceRelativePathToBuiltRelativePath", m_fConvertSourceRelativePathToBuiltRelativePath ) ) )
			{
				return result;
			}
		}

		return result;
	}

	eae6320::cResult cLuaState::CleanUp()
	{
		auto result = eae6320::Results::Success;

		if ( luaState )
		{
			// Un-reference functions
			{
				luaL_unref( luaState, LUA_REGISTRYINDEX, m_fBuildAssets );
			}
			// Close the Lua state
			{
				EAE6320_ASSERT( lua_gettop( luaState ) == 0 );
				lua_close( luaState );
				luaState = nullptr;
			}
		}

		return result;
	}

	eae6320::cResult cLuaState::SaveReferenceToGlobalFunctionInRegistry( const char* const i_functionName, int &o_functionReference )
	{
		auto result = eae6320::Results::Success;

		const auto type = lua_getglobal( luaState, i_functionName );
		if ( type == LUA_TFUNCTION )
		{
			o_functionReference = luaL_ref( luaState, LUA_REGISTRYINDEX );
			EAE6320_ASSERT( o_functionReference != LUA_REFNIL );
		}
		else
		{
			result = eae6320::Results::Failure;
			{
				std::ostringstream errorMessage;
				switch ( type )
				{
				case LUA_TNIL:
					{
						errorMessage << "The global function named \"" << i_functionName << "\" does not exist in the Lua environment";
					}
					break;
				default:
					{
						errorMessage << "The global variable \"" << i_functionName << "\""
							" is a " << lua_typename( luaState, type ) << ", not a function";
					}
					break;
				}
				eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, errorMessage.str().c_str() );
			}
			lua_pop( luaState, 1 );
		}

		return result;
	}
}

// Helper Definitions
//===================

namespace
{
	// Error / Warning Output
	//-----------------------

	eae6320::cResult FormatErrorOrWarningMessage( const char* const i_message, std::string &o_message, va_list io_insertions )
	{
		constexpr size_t bufferSize = 1024;
		char buffer[bufferSize];
		const auto formattingResult = vsnprintf( buffer, bufferSize, i_message, io_insertions );
		if ( formattingResult >= 0 )
		{
			if ( formattingResult < bufferSize )
			{
				o_message = buffer;
				return eae6320::Results::Success;
			}
			else
			{
				EAE6320_ASSERTF( false, "The internal asset build buffer of size %u isn't big enough to hold the formatted message of length %i",
					bufferSize, formattingResult + 1 );
				std::ostringstream errorMessage;
				errorMessage << "The internal buffer of size " << bufferSize
					<< " isn't big enough to hold the formmated message of length " << ( formattingResult + 1 ) << "!"
					" The unformatted message is: " << i_message;
				const unsigned int lineNumber = __LINE__;
				constexpr unsigned int* const noColumnNumber = nullptr;
				OutputErrorMessage_platformSpecific( errorMessage.str().c_str(), __FILE__, &lineNumber, noColumnNumber );
				return eae6320::Results::Failure;
			}
		}
		else
		{
			EAE6320_ASSERTF( false, "An encoding error occurred while formatting the asset build message \"%s\"", i_message );
			std::ostringstream errorMessage;
			errorMessage << "An encoding error was encountered! The unformatted message was: " << i_message;
			const unsigned int lineNumber = __LINE__;
			constexpr unsigned int* const noColumnNumber = nullptr;
			OutputErrorMessage_platformSpecific( errorMessage.str().c_str(), __FILE__, &lineNumber, noColumnNumber );
			return eae6320::Results::Failure;
		}
	}

	void OutputErrorMessage_platformSpecific( const char* const i_errorMessage, const char* const i_optionalFilePath,
		const unsigned int* const i_optionalLineNumber, const unsigned int* const i_optionalColumnNumber )
	{
#if defined( EAE6320_PLATFORM_WINDOWS )
		eae6320::Windows::OutputErrorMessageForVisualStudio( i_errorMessage, i_optionalFilePath, i_optionalLineNumber, i_optionalColumnNumber );
#else
	#error "No implementation exists for outputting asset build error messages!"
#endif
	}

	void OutputWarningMessage_platformSpecific( const char* const i_warningMessage, const char* const i_optionalFilePath,
		const unsigned int* const i_optionalLineNumber, const unsigned int* const i_optionalColumnNumber )
	{
#if defined( EAE6320_PLATFORM_WINDOWS )
		eae6320::Windows::OutputWarningMessageForVisualStudio( i_warningMessage, i_optionalFilePath, i_optionalLineNumber, i_optionalColumnNumber );
#else
	#error "No implementation exists for outputting asset build warning messages!"
#endif
	}

	// Lua Wrapper Functions
	//----------------------

	int luaCopyFile( lua_State* io_luaState )
	{
		// Argument #1: The source path
		const char* i_path_source;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_path_source = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}
		// Argument #2: The target path
		const char* i_path_target;
		if ( lua_isstring( io_luaState, 2 ) )
		{
			i_path_target = lua_tostring( io_luaState, 2 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #2 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 2 ) );
		}

		// Copy the file
		{
			std::string errorMessage;
			// There are many reasons that a source should be rebuilt,
			// and so even if the target already exists it should just be written over
			constexpr auto noErrorIfTargetAlreadyExists = false;
			// Since we rely on timestamps to determine when a target was built
			// its file time should be updated when the source gets copied
			constexpr auto updateTheTargetFileTime = true;
			if ( eae6320::Platform::CopyFile( i_path_source, i_path_target, noErrorIfTargetAlreadyExists, updateTheTargetFileTime, &errorMessage ) )
			{
				lua_pushboolean( io_luaState, true );
				constexpr int returnValueCount = 1;
				return returnValueCount;
			}
			else
			{
				lua_pushboolean( io_luaState, false );
				lua_pushstring( io_luaState, errorMessage.c_str() );
				constexpr int returnValueCount = 2;
				return returnValueCount;
			}
		}
	}

	int luaCreateDirectoryIfItDoesntExist( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_path = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}

		std::string errorMessage;
		if ( eae6320::Platform::CreateDirectoryIfItDoesntExist( i_path, &errorMessage ) )
		{
			constexpr int returnValueCount = 0;
			return returnValueCount;
		}
		else
		{
			return luaL_error( io_luaState, errorMessage.c_str() );
		}
	}

	int luaDoesFileExist( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_path = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}

		std::string errorMessage;
		if ( eae6320::Platform::DoesFileExist( i_path, &errorMessage ) )
		{
			lua_pushboolean( io_luaState, true );
			constexpr int returnValueCount = 1;
			return returnValueCount;
		}
		else
		{
			lua_pushboolean( io_luaState, false );
			lua_pushstring( io_luaState, errorMessage.c_str() );
			constexpr int returnValueCount = 2;
			return returnValueCount;
		}
	}

	int luaExecuteCommand( lua_State* io_luaState )
	{
		// Argument #1: The command
		const char* i_command;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_command = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}

		int exitCode;
		std::string errorMessage;
		if ( eae6320::Platform::ExecuteCommand( i_command, &exitCode, &errorMessage ) )
		{
			lua_pushboolean( io_luaState, true );
			lua_pushinteger( io_luaState, exitCode );
			constexpr int returnValueCount = 2;
			return returnValueCount;
		}
		else
		{
			lua_pushboolean( io_luaState, false );
			lua_pushstring( io_luaState, errorMessage.c_str() );
			constexpr int returnValueCount = 2;
			return returnValueCount;
		}
	}

	int luaGetEnvironmentVariable( lua_State* io_luaState )
	{
		// Argument #1: The key
		const char* i_key;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_key = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}

		std::string value;
		std::string errorMessage;
		if ( eae6320::Platform::GetEnvironmentVariable( i_key, value, &errorMessage ) )
		{
			lua_pushstring( io_luaState, value.c_str() );
			constexpr int returnValueCount = 1;
			return returnValueCount;
		}
		else
		{
			lua_pushnil( io_luaState );
			lua_pushstring( io_luaState, errorMessage.c_str() );
			constexpr int returnValueCount = 2;
			return returnValueCount;
		}
	}

	int LuaGetFilesInDirectory( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_path = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}
		// Argument #2: An optional indication of whether the directory's subdirectories should be searched recursively
		auto i_shouldSubdirectoriesBeSearchedRecursively = true;
		if ( !lua_isnoneornil( io_luaState, 2 ) )
		{
			if ( lua_isboolean( io_luaState, 2 ) )
			{
				i_shouldSubdirectoriesBeSearchedRecursively = lua_toboolean( io_luaState, 2 ) != 0;
			}
			else
			{
				return luaL_error( io_luaState,
					"Argument #2 must be a boolean (instead of a %s)",
					luaL_typename( io_luaState, 2 ) );
			}
		}

		std::vector<std::string> paths;
		std::string errorMessage;
		if ( eae6320::Platform::GetFilesInDirectory( i_path, paths, i_shouldSubdirectoriesBeSearchedRecursively, &errorMessage ) )
		{
			const auto arraySize = paths.size();
			EAE6320_ASSERT( arraySize <= static_cast<decltype( arraySize )>( std::numeric_limits<int>::max() ) );
			constexpr int noDictionaryEntries = 0;
			lua_createtable( io_luaState, static_cast<int>( arraySize ), noDictionaryEntries );
			for ( size_t i = 0; i < arraySize; ++i )
			{
				lua_pushinteger( io_luaState, static_cast<lua_Integer>( i ) + 1 );
				lua_pushstring( io_luaState, paths[i].c_str() );
				lua_settable( io_luaState, -3 );
			}
			constexpr int returnValueCount = 1;
			return returnValueCount;
		}
		else
		{
			return luaL_error( io_luaState, errorMessage.c_str() );
		}
	}

	int luaGetLastWriteTime( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_path = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}

		// Get the last time that the file was written to
		uint64_t lastWriteTime;
		std::string errorMessage;
		if ( eae6320::Platform::GetLastWriteTime( i_path, lastWriteTime, &errorMessage ) )
		{
			lua_pushnumber( io_luaState, static_cast<lua_Number>( lastWriteTime ) );
			constexpr int returnValueCount = 1;
			return returnValueCount;
		}
		else
		{
			return luaL_error( io_luaState, errorMessage.c_str() );
		}
	}

	int luaInvalidateLastWriteTime( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_path = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}

		// Invalidate the last time that the file was written to
		std::string errorMessage;
		if ( eae6320::Platform::InvalidateLastWriteTime( i_path, &errorMessage ) )
		{
			constexpr int returnValueCount = 0;
			return returnValueCount;
		}
		else
		{
			return luaL_error( io_luaState, errorMessage.c_str() );
		}
	}

	int luaOutputErrorMessage( lua_State* io_luaState )
	{
		// Argument #1: The error message
		const char* i_errorMessage;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_errorMessage = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}
		// Argument #2: An optional file name
		const char* i_optionalFileName = nullptr;
		if ( !lua_isnoneornil( io_luaState, 2 ) )
		{
			if ( lua_isstring( io_luaState, 2 ) )
			{
				i_optionalFileName = lua_tostring( io_luaState, 2 );
			}
			else
			{
				return luaL_error( io_luaState,
					"Argument #2 must be a string (instead of a %s)",
					luaL_typename( io_luaState, 2 ) );
			}
		}

		// Output the error message
		if ( i_optionalFileName )
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo( i_optionalFileName, i_errorMessage );
		}
		else
		{
			eae6320::Assets::OutputErrorMessage( i_errorMessage );
		}

		constexpr int returnValueCount = 0;
		return returnValueCount;
	}

	int luaOutputWarningMessage( lua_State* io_luaState )
	{
		// Argument #1: The warning message
		const char* i_warningMessage;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_warningMessage = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}
		// Argument #2: An optional file name
		const char* i_optionalFileName = nullptr;
		if ( !lua_isnoneornil( io_luaState, 2 ) )
		{
			if ( lua_isstring( io_luaState, 2 ) )
			{
				i_optionalFileName = lua_tostring( io_luaState, 2 );
			}
			else
			{
				return luaL_error( io_luaState,
					"Argument #2 must be a string (instead of a %s)",
					luaL_typename( io_luaState, 2 ) );
			}
		}

		// Output the warning message
		if ( i_optionalFileName )
		{
			eae6320::Assets::OutputWarningMessageWithFileInfo( i_optionalFileName, i_warningMessage );
		}
		else
		{
			eae6320::Assets::OutputWarningMessage( i_warningMessage );
		}

		constexpr int returnValueCount = 0;
		return returnValueCount;
	}
}
