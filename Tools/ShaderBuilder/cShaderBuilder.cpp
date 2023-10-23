// Includes
//=========

#include "cShaderBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cShaderBuilder::Build( const std::vector<std::string>& i_arguments )
{
	// Decide which kind of shader program to compile
	auto shaderType = Graphics::eShaderType::Unknown;
	{
		if ( i_arguments.size() >= 1 )
		{
			const std::string& argument = i_arguments[0];
			if ( argument == "vertex" )
			{
				shaderType = Graphics::eShaderType::Vertex;
			}
			else if ( argument == "fragment" )
			{
				shaderType = Graphics::eShaderType::Fragment;
			}
			else
			{
				OutputErrorMessageWithFileInfo( m_path_source,
					"\"%s\" is not a valid shader program type", argument );
				return Results::Failure;
			}
		}
		else
		{
			OutputErrorMessageWithFileInfo( m_path_source,
				"A Shader must be built with an argument defining which type of shader program (e.g. \"vertex\" or \"fragment\") to compile" );
			return Results::Failure;
		}
	}

	return Build( shaderType, i_arguments );
}
