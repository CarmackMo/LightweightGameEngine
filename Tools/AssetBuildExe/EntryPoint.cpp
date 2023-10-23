/*
	The main() function is where the program starts execution
*/

// Includes
//=========

#include <cstdlib>
#include <Engine/Results/Results.h>
#include <Tools/AssetBuildLibrary/Functions.h>

// Entry Point
//============

int main( int i_argumentCount, char** i_arguments )
{
	auto result = eae6320::Results::Success;

	// The command line should have a path to the list of assets to build
	if ( i_argumentCount == 2 )
	{
		const auto* const path_assetsToBuild = i_arguments[1];
		result = eae6320::Assets::BuildAssets( path_assetsToBuild );
	}
	else
	{
		result = eae6320::Results::Failure;
		eae6320::Assets::OutputErrorMessageWithFileInfo( __FILE__, __LINE__,
			"AssetBuild.exe must be run with a single command line argument which is the path to the list of assets to build"
			" (the invalid argument count being passed to main is %u)", i_argumentCount );
	}

	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
