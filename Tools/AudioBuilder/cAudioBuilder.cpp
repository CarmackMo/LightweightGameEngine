// Includes
//=========

#include <Engine/Platform/Platform.h>
#include <Tools/AssetBuildLibrary/Functions.h>
#include <Tools/AudioBuilder/cAudioBuilder.h>


// Inherited Implementation
//=========================

eae6320::cResult eae6320::Assets::cAudioBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = Results::Success;

	// Copy and backup the source code 
	{
		if (!(result = CopyAudioAsset(m_path_source, m_path_target)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Failed copy lua mesh file");
		}
	}

	return result;
}


// Implementation
//=========================

eae6320::cResult eae6320::Assets::cAudioBuilder::CopyAudioAsset(const char* const i_sourcePath, const char* const i_targetPath)
{
	auto result = Results::Success;
	Platform::sDataFromFile dataFromFile;

	// Load the source code
	{
		std::string errorMessage;
		if (!(result = Platform::LoadBinaryFile(i_sourcePath, dataFromFile, &errorMessage)))
		{
			OutputErrorMessageWithFileInfo(i_sourcePath, "Failed to load mesh source file: %s", errorMessage.c_str());
			return result;
		}
	}
	// Write the file to disk
	{
		std::string errorMessage;
		if (!(result = Platform::WriteBinaryFile(i_targetPath, dataFromFile.data, dataFromFile.size, &errorMessage)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_targetPath, errorMessage.c_str());
			return result;
		}
	}

	return result;
}
