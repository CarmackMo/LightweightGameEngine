/*
	This file contains functions used by asset build tools
*/

#ifndef EAE6320_ASSETBUILD_FUNCTIONS_H
#define EAE6320_ASSETBUILD_FUNCTIONS_H

// Includes
//=========

#include <Engine/Results/Results.h>
#include <string>

// Interface
//==========

namespace eae6320
{
	namespace Assets
	{
		eae6320::cResult BuildAssets( const char* const i_path_assetsToBuild );

		// If an asset ("A") references another asset ("B")
		// then that reference to B must be converted from a source path to a built path
		// if it is included in the built A asset
		eae6320::cResult ConvertSourceRelativePathToBuiltRelativePath( const char* const i_sourceRelativePath, const char* const i_assetType,
			std::string& o_builtRelativePath, std::string* o_errorMessage = nullptr );

		// Error / Warning Output
		//-----------------------

		// These functions output asset build errors or warnings in the best way for the platform's build tools
		// (e.g. for Visual Studio the messages are formatted so that they'll show up in the Error List window)

		void OutputErrorMessage( const char* const i_errorMessage, ... );
		void OutputErrorMessageWithFileInfo( const char* const i_filePath,
			const char* const i_errorMessage, ... );
		void OutputErrorMessageWithFileInfo( const char* const i_filePath,
			const unsigned int i_lineNumber,
			const char* const i_errorMessage, ... );
		void OutputErrorMessageWithFileInfo( const char* const i_filePath,
			const unsigned int i_lineNumber, const unsigned int i_columnNumber,
			const char* const i_errorMessage, ... );

		void OutputWarningMessage( const char* const i_warningMessage, ... );
		void OutputWarningMessageWithFileInfo( const char* const i_filePath,
			const char* const i_warningMessage, ... );
		void OutputWarningMessageWithFileInfo( const char* const i_filePath,
			const unsigned int i_lineNumber,
			const char* const i_warningMessage, ... );
		void OutputWarningMessageWithFileInfo( const char* const i_filePath,
			const unsigned int i_lineNumber, const unsigned int i_columnNumber,
			const char* const i_warningMessage, ... );
	}
}

#endif	// EAE6320_ASSETBUILD_FUNCTIONS_H
