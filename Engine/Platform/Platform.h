/*
	This file provides access to platform-specific functionality
	with a platform-independent interface
*/

#ifndef EAE6320_PLATFORM_H
#define EAE6320_PLATFORM_H

// Includes
//=========

// Windows unfortunately #defines common function names (specifically ones used here),
// and it is simpler to #include windows.h first
// rather than having to do #preprocessor tricks after the fact
#ifdef EAE6320_PLATFORM_WINDOWS
	#include <Engine/Windows/Includes.h>
#endif

#include <cstdint>
#include <cstdlib>
#include <Engine/Results/Results.h>
#include <string>
#include <vector>

// Interface
//==========

namespace eae6320
{
	namespace Platform
	{
		// This is used to load binary files
		struct sDataFromFile
		{
			void* data = nullptr;
			size_t size = 0;

			sDataFromFile() = default;

			void Free() noexcept
			{
				if ( data )
				{
					free( data );
					data = nullptr;
				}
				size = 0;
			}

			~sDataFromFile()
			{
				Free();
			}

			sDataFromFile( const sDataFromFile& ) = delete;
			sDataFromFile( sDataFromFile&& io_movedFrom ) noexcept
				:
				data( io_movedFrom.data ), size( io_movedFrom.size )
			{
				io_movedFrom.data = nullptr;
				io_movedFrom.size = 0;
			}
			sDataFromFile& operator =( const sDataFromFile& ) = delete;
			sDataFromFile& operator =( sDataFromFile&& io_movedFrom ) noexcept
			{
				if ( &io_movedFrom != this )
				{
					Free();
					data = io_movedFrom.data;
					io_movedFrom.data = nullptr;
					size = io_movedFrom.size;
					io_movedFrom.size = 0;
				}
				return *this;
			}
		};

		cResult CopyFile( const char* const i_path_source, const char* const i_path_target,
			const bool i_shouldFunctionFailIfTargetAlreadyExists = false, const bool i_shouldTargetFileTimeBeModified = false,
			std::string* o_errorMessage = nullptr );
		cResult CreateDirectoryIfItDoesntExist( const std::string& i_filePath, std::string* const o_errorMessage = nullptr );
		bool DoesFileExist( const char* const i_path, std::string* const o_errorMessage = nullptr );
		cResult ExecuteCommand( const char* const i_command, int* const o_exitCode = nullptr, std::string* const o_errorMessage = nullptr );
		cResult GetFilesInDirectory( const std::string& i_path, std::vector<std::string>& o_paths,
			const bool i_shouldSubdirectoriesBeSearchedRecursively = true, std::string* const o_errorMessage = nullptr );
		cResult GetEnvironmentVariable( const char* const i_key, std::string& o_value, std::string* const o_errorMessage = nullptr );
		cResult GetLastWriteTime( const char* const i_path, uint64_t& o_lastWriteTime, std::string* const o_errorMessage = nullptr );
		cResult InvalidateLastWriteTime( const char* const i_path, std::string* const o_errorMessage = nullptr );
		cResult LoadBinaryFile( const char* const i_path, sDataFromFile& o_data, std::string* const o_errorMessage = nullptr );
		// This function writes an entire file in a single operation in the most efficient way possible.
		// If you need to write out more than one smaller chunk to a file, however,
		// you should use one of the standard library functions that does buffering.
		cResult WriteBinaryFile( const char* const i_path, const void* const i_data, const size_t i_size, std::string* const o_errorMessage = nullptr );
	}
}

// Result Definitions
//===================

namespace eae6320
{
	namespace Results
	{
		namespace Platform
		{
			// This is returned when an environment variable doesn't exist
			// (i.e. if GetEnvironmentVariable() fails this indicates that there were no platform-specific API errors,
			// but that the requested environment variable doesn't exist)
			constexpr cResult EnvironmentVariableDoesntExist( IsFailure, eSystem::Platform, __LINE__ );
		}
	}
}

#endif	// EAE6320_PLATFORM_H
