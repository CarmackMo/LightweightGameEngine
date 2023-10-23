/*
	This file contains wrappers for Windows functions
*/

#ifndef EAE6320_WINDOWS_FUNCTIONS_H
#define EAE6320_WINDOWS_FUNCTIONS_H

// Includes
//=========

#include "Includes.h"

#include <cstdint>
#include <cstdlib>
#include <Engine/Results/Results.h>
#include <string>
#include <vector>

// Interface
//==========

namespace eae6320
{
	namespace Windows
	{
		// This is used to load binary files
		// (Note that the memory is _not_ automatically freed by the destructor)
		struct sDataFromFile
		{
			void* data = nullptr;
			size_t size = 0;

			void Free()
			{
				if ( data )
				{
					free( data );
					data = nullptr;
				}
			}
		};

		std::wstring ConvertUtf8ToUtf16( const char* const i_string_utf8 );
		std::string ConvertUtf16ToUtf8( const wchar_t* const i_string_utf16 );
		cResult CopyFile( const char* const i_path_source, const char* const i_path_target,
			const bool i_shouldFunctionFailIfTargetAlreadyExists = false, const bool i_shouldTargetFileTimeBeModified = false,
			std::string* o_errorMessage = nullptr );
		cResult CreateDirectoryIfItDoesntExist( const std::string& i_filePath, std::string* const o_errorMessage = nullptr );
		bool DoesFileExist( const char* const i_path, std::string* const o_errorMessage = nullptr );
		cResult ExecuteCommand( const char* const i_command, DWORD* const o_exitCode = nullptr, std::string* const o_errorMessage = nullptr );
		cResult ExecuteCommand( const char* const i_path, const char* const i_optionalArguments = nullptr,
			DWORD* const o_exitCode = nullptr, std::string* const o_errorMessage = nullptr );
		cResult GetFilesInDirectory( const std::string& i_path, std::vector<std::string>& o_paths,
			const bool i_shouldSubdirectoriesBeSearchedRecursively = true, std::string* const o_errorMessage = nullptr );
		cResult GetEnvironmentVariable( const char* const i_key, std::string& o_value, std::string* const o_errorMessage = nullptr );
		std::string GetFormattedSystemMessage( const DWORD i_code );
		std::string GetLastSystemError( DWORD* const o_optionalErrorCode = nullptr );
		cResult GetLastWriteTime( const char* const i_path, uint64_t& o_lastWriteTime, std::string* const o_errorMessage = nullptr );
		cResult InvalidateLastWriteTime( const char* const i_path, std::string* const o_errorMessage = nullptr );
		cResult LoadBinaryFile( const char* const i_path, sDataFromFile& o_data, std::string* const o_errorMessage = nullptr );
		void OutputErrorMessageForVisualStudio( const char* const i_errorMessage, const char* const i_optionalFilePath = nullptr,
			const unsigned int* const i_optionalLineNumber = nullptr, const unsigned int* const i_optionalColumnNumber = nullptr );
		void OutputWarningMessageForVisualStudio( const char* const i_errorMessage, const char* const i_optionalFilePath = nullptr,
			const unsigned int* const i_optionalLineNumber = nullptr, const unsigned int* const i_optionalColumnNumber = nullptr );
		cResult WriteBinaryFile( const char* const i_path, const void* const i_data, const size_t i_size, std::string* const o_errorMessage = nullptr );
	}
}

#endif	// EAE6320_WINDOWS_FUNCTIONS_H
