#pragma once

// Includes
//=========
#include <External/Lua/Includes.h>
#include <Tools/AssetBuildLibrary/iBuilder.h>


// Class Declaration
//==================

namespace eae6320
{
namespace Assets
{

	class cAudioBuilder final : public iBuilder
	{
		// Inherited Implementation
		//=========================

	private:

		// Build
		//------

		cResult Build(const std::vector<std::string>& i_arguments) final;

		// Implementation
		//===============

	private:

		// Build
		//------

		cResult CopyAudioAsset(const char* const i_sourcePath, const char* const i_targetPath);

	};

}// Namespace Assets
}// Namespace eae6320