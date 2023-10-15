#pragma once

// Includes
//=========
#include <Engine/Graphics/VertexFormats.h>
#include <External/Lua/Includes.h>
#include <Tools/AssetBuildLibrary/iBuilder.h>


// Class Declaration
//==================

namespace eae6320
{
namespace Assets
{

	class cMeshBuilder final : public iBuilder
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

		cResult CopyLuaAsset(const char* const i_sourcePath, const char* const i_targetPath);

		cResult LoadLuaAsset(const char* const i_sourcePath);
		cResult LoadLuaVertexData(
			lua_State& io_luaState, const char* const i_sourcePath, const std::string& i_key, 
			uint32_t& o_arrayLen, Graphics::VertexFormats::sVertex_mesh*& o_array);
		cResult LoadLuaIndexData(
			lua_State& io_luaState, const char* const i_sourcePath, const std::string& i_key, 
			uint32_t& o_arrayLen, uint16_t*& o_array);

		cResult WriteDataToAsset(const char* const i_targetPath);

	};

}// Namespace Assets
}// Namespace eae6320