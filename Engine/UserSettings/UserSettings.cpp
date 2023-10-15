// Includes
//=========

#include "UserSettings.h"

#include <cmath>
#include <csetjmp>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/UserOutput/UserOutput.h>
#include <External/Lua/Includes.h>
#include <limits>
#include <string>

// Static Data
//============

namespace
{
	uint16_t s_resolutionHeight = 0;
	auto s_resolutionHeight_validity = eae6320::Results::Failure;
	uint16_t s_resolutionWidth = 0;
	auto s_resolutionWidth_validity = eae6320::Results::Failure;

	constexpr auto* const s_userSettingsFileName = "settings.ini";

	// Restore point if Lua panics
	jmp_buf s_jumpBuffer;
}

// Helper Declarations
//====================

namespace
{
	eae6320::cResult InitializeIfNecessary();
	eae6320::cResult LoadUserSettingsIntoLuaTable( lua_State& io_luaState );
	eae6320::cResult PopulateUserSettingsFromLuaTable( lua_State& io_luaState );

	// Called if Lua panics
	// (e.g. when an unhandled error is thrown)
	int OnLuaPanic( lua_State* io_luaState ) noexcept;
}

// Interface
//==========

eae6320::cResult eae6320::UserSettings::GetDesiredInitialResolutionWidth( uint16_t& o_width )
{
	const auto result = InitializeIfNecessary();
	if ( result )
	{
		if ( s_resolutionWidth_validity )
		{
			o_width = s_resolutionWidth;
		}
		return s_resolutionWidth_validity;
	}
	else
	{
		return result;
	}
}

eae6320::cResult eae6320::UserSettings::GetDesiredInitialResolutionHeight( uint16_t& o_height )
{
	const auto result = InitializeIfNecessary();
	if ( result )
	{
		if ( s_resolutionHeight_validity )
		{
			o_height = s_resolutionHeight;
		}
		return s_resolutionHeight_validity;
	}
	else
	{
		return result;
	}
}

// Helper Definitions
//===================

namespace
{
	eae6320::cResult InitializeIfNecessary()
	{
		static eae6320::cResult isInitialized;
		if ( isInitialized != eae6320::Results::Undefined )
		{
			return isInitialized;
		}

		auto result = eae6320::Results::Success;

		// Create a new Lua state
		lua_State* luaState = nullptr;
		auto wasUserSettingsEnvironmentCreated = false;

		const eae6320::cScopeGuard scopeGuard( [&result, &luaState, &wasUserSettingsEnvironmentCreated]()
			{
				// Free the Lua environment
				if ( luaState )
				{
					if ( wasUserSettingsEnvironmentCreated )
					{
						lua_pop( luaState, 1 );
					}
					EAE6320_ASSERTF( lua_gettop( luaState ) == 0, "Lua stack is inconsistent" );
					lua_close( luaState );
					luaState = nullptr;
				}
				// Store the result
				isInitialized = result;
			} );

		int jumpValue = 0;	// 0 means no jump has happened
		{
			luaState = luaL_newstate();
			if ( luaState )
			{
				// Set a function that will be called if Lua is about to abort
				lua_atpanic( luaState, OnLuaPanic );
			}
			else
			{
				result = eae6320::Results::OutOfMemory;
				EAE6320_ASSERTF( false, "Failed to create a new Lua state" );
				eae6320::Logging::OutputError( "Failed to create a new Lua state for the user settings" );
				return result;
			}
		}
		// Set a restore point in case Lua panics
		jumpValue = setjmp( s_jumpBuffer );
		if ( jumpValue == 0 )
		{
			// Create an empty table to be used as the Lua environment for the user settings
			if ( lua_checkstack( luaState, 1 ) )
			{
				lua_newtable( luaState );
				wasUserSettingsEnvironmentCreated = true;
			}
			else
			{
				result = eae6320::Results::OutOfMemory;
				EAE6320_ASSERTF( false, "Lua didn't increase its tack for a new table" );
				eae6320::Logging::OutputError( "User settings files can't be processed"
					" because Lua can't increase its stack for a new table" );
				return result;
			}
			// Load the user settings
			if ( result = LoadUserSettingsIntoLuaTable( *luaState ) )
			{
				// Populate the user settings in C from the user settings in the Lua environment
				if ( !( result = PopulateUserSettingsFromLuaTable( *luaState ) ) )
				{
					return result;
				}
			}
			else
			{
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF( false, "Unhandled Lua error" );
			eae6320::Logging::OutputError( "User settings files can't be processed"
				" because of an unhandled Lua error" );
		}

		return result;
	}

	eae6320::cResult LoadUserSettingsIntoLuaTable( lua_State& io_luaState )
	{
		// Load the user settings file into the Lua environment
		if ( eae6320::Platform::DoesFileExist( s_userSettingsFileName ) )
		{
			constexpr int requiredStackSlotCount = 0
				// The file as a function
				+ 1
				// The environment upvalue
				+ 1
				;
			if ( lua_checkstack( &io_luaState, requiredStackSlotCount ) )
			{
				// Load the file and compile its contents into a Lua function
				const auto luaResult = luaL_loadfile( &io_luaState, s_userSettingsFileName );
				if ( luaResult == LUA_OK )
				{
					// Set the Lua function's environment
					{
						// Push the empty table to the top of the stack
						lua_pushvalue( &io_luaState, -2 );
						// Set the empty table as the function's global environment
						// (this means that anything that the file syntactically adds to the global environment
						// will actually be added to the table)
						constexpr int globalEnvironmentUpvalueIndex = 1;
						const auto* const upvalueName = lua_setupvalue( &io_luaState, -2, globalEnvironmentUpvalueIndex );
						if ( upvalueName )
						{
							EAE6320_ASSERT( strcmp( "_ENV", upvalueName ) == 0 );
						}
						else
						{
							EAE6320_ASSERT( false );
							eae6320::Logging::OutputError( "Internal error setting the Lua environment for the user settings file \"%s\"!"
								" This should never happen", s_userSettingsFileName );
							lua_pop( &io_luaState, 2 );
							return eae6320::Results::Failure;
						}
					}
					// Call the Lua function
					// (this will add anything that the file syntactically sets in the global environment
					// into the empty table that was created)
					{
						constexpr int noArguments = 0;
						constexpr int noReturnValues = 0;
						constexpr int noErrorMessageHandler = 0;
						const auto luaResult = lua_pcall( &io_luaState, noArguments, noReturnValues, noErrorMessageHandler );
						if ( luaResult == LUA_OK )
						{
							return eae6320::Results::Success;
						}
						else
						{
							const std::string luaErrorMessage = lua_tostring( &io_luaState, -1 );
							lua_pop( &io_luaState, 1 );

							EAE6320_ASSERTF( false, "User settings file error: %s", luaErrorMessage.c_str() );
							if ( luaResult == LUA_ERRRUN )
							{
								eae6320::Logging::OutputError( "Error in the user settings file \"%s\": %s",
									s_userSettingsFileName, luaErrorMessage );
							}
							else
							{
								eae6320::Logging::OutputError( "Error processing the user settings file \"%s\": %s",
									s_userSettingsFileName, luaErrorMessage );
							}

							return eae6320::Results::InvalidFile;
						}
					}
				}
				else
				{
					const std::string luaErrorMessage = lua_tostring( &io_luaState, -1 );
					lua_pop( &io_luaState, 1 );

					if ( luaResult == LUA_ERRFILE )
					{
						EAE6320_ASSERTF( false, "Error opening or reading user settings file: %s", luaErrorMessage.c_str() );
						eae6320::Logging::OutputError( "Error opening or reading the user settings file \"%s\" even though it exists: %s",
							s_userSettingsFileName, luaErrorMessage.c_str() );

					}
					else if ( luaResult == LUA_ERRSYNTAX )
					{
						EAE6320_ASSERTF( false, "Syntax error in user settings file: %s", luaErrorMessage.c_str() );
						eae6320::Logging::OutputError( "Syntax error in the user settings file \"%s\": %s",
							s_userSettingsFileName, luaErrorMessage.c_str() );
					}
					else
					{
						EAE6320_ASSERTF( false, "Error loading user settings file: %s", luaErrorMessage.c_str() );
						eae6320::Logging::OutputError( "Error loading the user settings file \"%s\": %s",
							s_userSettingsFileName, luaErrorMessage.c_str() );
					}

					return eae6320::Results::InvalidFile;
				}
			}
			else
			{
				EAE6320_ASSERTF( false, "Not enough stack space to load user settings file" );
				eae6320::Logging::OutputError( "Lua can't allocate enough stack space to load the user settings file \"%s\"",
					s_userSettingsFileName );
				return eae6320::Results::OutOfMemory;
			}
		}
		else
		{
			// If loading the file failed because the file doesn't exist it's ok;
			// default values will be used
			eae6320::Logging::OutputMessage( "The user settings file \"%s\" doesn't exist. Using default settings instead.",
				s_userSettingsFileName );
			return eae6320::Results::FileDoesntExist;
		}
	}

	eae6320::cResult PopulateUserSettingsFromLuaTable( lua_State& io_luaState )
	{
		auto result = eae6320::Results::Success;

		// There should always be enough stack space because the file had to be loaded,
		// but it doesn't hurt to do a sanity check in the context of this function
		if ( !lua_checkstack( &io_luaState, 1 ) )
		{
			EAE6320_ASSERTF( false, "Not enough stack space to read a setting from user settings file" );
			eae6320::Logging::OutputError( "Lua can't allocate enough stack space to read each user setting" );
			return eae6320::Results::OutOfMemory;
		}

		// Resolution Width
		{
			constexpr char* const key_width = "resolutionWidth";

			lua_pushstring( &io_luaState, key_width );
			lua_gettable( &io_luaState, -2 );
			eae6320::cScopeGuard scopeGuard_popWidth( [&io_luaState]
				{
					lua_pop( &io_luaState, 1 );
				} );
			if ( lua_isinteger( &io_luaState, -1 ) )
			{
				const auto luaInteger = lua_tointeger( &io_luaState, -1 );
				if ( luaInteger >= 0 )
				{
					constexpr auto maxWidth = std::numeric_limits<decltype( s_resolutionWidth )>::max();
					if ( luaInteger <= maxWidth )
					{
						s_resolutionWidth = static_cast<uint16_t>( luaInteger );
						s_resolutionWidth_validity = eae6320::Results::Success;
						eae6320::Logging::OutputMessage( "User settings defined resolution width of %u", s_resolutionWidth );
					}
					else
					{
						s_resolutionWidth_validity = eae6320::Results::InvalidFile;
						eae6320::Logging::OutputMessage( "The user settings file %s specifies a resolution width (%i)"
							" that is bigger than the maximum (%u)", s_userSettingsFileName, luaInteger, maxWidth );
					}
				}
				else
				{
					s_resolutionWidth_validity = eae6320::Results::InvalidFile;
					eae6320::Logging::OutputMessage( "The user settings file %s specifies a non-positive resolution width (%i)",
						s_userSettingsFileName, luaInteger );
				}
			}
			else
			{
				s_resolutionWidth_validity = eae6320::Results::InvalidFile;
				eae6320::Logging::OutputMessage( "The user settings file %s specifies a %s for %s instead of an integer",
					s_userSettingsFileName, luaL_typename( &io_luaState, -1 ), key_width );
			}
		}
		// Resolution Height
		{
			const char* key_height = "resolutionHeight";

			lua_pushstring( &io_luaState, key_height );
			lua_gettable( &io_luaState, -2 );
			eae6320::cScopeGuard scopeGuard_popHeight( [&io_luaState]
				{
					lua_pop( &io_luaState, 1 );
				} );
			if ( lua_isinteger( &io_luaState, -1 ) )
			{
				const auto luaInteger = lua_tointeger( &io_luaState, -1 );
				if ( luaInteger >= 0 )
				{
					constexpr auto maxHeight = std::numeric_limits<decltype( s_resolutionHeight )>::max();
					if ( luaInteger <= maxHeight )
					{
						s_resolutionHeight = static_cast<uint16_t>( luaInteger );
						s_resolutionHeight_validity = eae6320::Results::Success;
						eae6320::Logging::OutputMessage( "User settings defined resolution height of %u", s_resolutionHeight );
					}
					else
					{
						s_resolutionHeight_validity = eae6320::Results::InvalidFile;
						eae6320::Logging::OutputMessage( "The user settings file %s specifies a resolution height (%i)"
							" that is bigger than the maximum (%u)", s_userSettingsFileName, luaInteger, maxHeight );
					}
				}
				else
				{
					s_resolutionHeight_validity = eae6320::Results::InvalidFile;
					eae6320::Logging::OutputMessage( "The user settings file %s specifies a non-positive resolution height (%i)",
						s_userSettingsFileName, luaInteger );
				}
			}
			else
			{
				s_resolutionHeight_validity = eae6320::Results::InvalidFile;
				eae6320::Logging::OutputMessage( "The user settings file %s specifies a %s for %s instead of an integer",
					s_userSettingsFileName, luaL_typename( &io_luaState, -1 ), key_height );
			}
		}

		return result;
	}

	int OnLuaPanic( lua_State* io_luaState ) noexcept
	{
		// The error object should be at the top of the stack
		{
			if ( lua_isstring( io_luaState, -1 ) )
			{
				eae6320::Logging::OutputError( "Lua is panicking when processing User Settings: %s", lua_tostring( io_luaState, -1 ) );
			}
			else
			{
				eae6320::Logging::OutputError( "Lua is panicking for an unknown reason when processing User Settings" );
			}
		}
		// Jump back to the restore point
		{
			constexpr int errorCode = -1;	// This value isn't checked
			longjmp( s_jumpBuffer, errorCode );
		}

		// This code should never be reached
		eae6320::Logging::OutputError( "The application was not able to recover from Lua's panicking."
			" It is about to be forcefully exited." );
		eae6320::UserOutput::Print( "An unexpected error occurred when processing User Settings (see the log file for details)."
			" The application is about to exit" );
		return 0;
	}
}
