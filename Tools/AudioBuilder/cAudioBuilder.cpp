// Includes
//=========


#include <Engine/Asserts/Asserts.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <fstream>
#include <string>
#include <Tools/AssetBuildLibrary/Functions.h>
#include <Tools/AudioBuilder/cAudioBuilder.h>


// Static Varialbes
namespace
{
	constexpr unsigned int s_numPosPerVertex = 3;
	constexpr unsigned int s_numColorPerVertex = 4;
	constexpr unsigned int s_numIndexPerTriangle = 3;

	uint32_t s_vertexNum = 0;
	uint32_t s_indexNum = 0;
	eae6320::Graphics::VertexFormats::sVertex_mesh* s_vertexData = nullptr;
	uint16_t* s_indexData = nullptr;
}


// Inherited Implementation
//=========================

eae6320::cResult eae6320::Assets::cAudioBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = Results::Success;

	// Load the source code
	{
		if (!(result = LoadLuaAsset(m_path_source)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Failed load lua mesh file");
			return eae6320::Results::InvalidFile;
		}
	}
	// Write the file to disk
	{
		if (!(result = WriteDataToAsset(m_path_target)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_target, "Failed write index data to binary file");
			return eae6320::Results::Failure;
		}
	}
	// Copy and backup the source code 
	{
		if (!(result = CopyLuaAsset(m_path_source, m_path_target)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Failed copy lua mesh file");
		}
	}

	return result;
}


// Implementation
//=========================

eae6320::cResult eae6320::Assets::cAudioBuilder::CopyLuaAsset(const char* const i_sourcePath, const char* const i_targetPath)
{
	auto result = Results::Success;
	Platform::sDataFromFile dataFromFile;

	std::string targetPath = i_targetPath;
	targetPath = targetPath.substr(0, targetPath.size() - 5);	// Remove the ".mesh" file extension
	targetPath.append(".lua");									// Replace it with ".lua" extension

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
		if (!(result = Platform::WriteBinaryFile(targetPath.c_str(), dataFromFile.data, dataFromFile.size, &errorMessage)))
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(targetPath.c_str(), errorMessage.c_str());
		}
	}

	return result;
}


eae6320::cResult eae6320::Assets::cAudioBuilder::LoadLuaAsset(const char* const i_sourcePath)
{
	auto result = eae6320::Results::Success;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, "Failed to create a new Lua state");
			return eae6320::Results::OutOfMemory;;
		}
	}

	// Load the asset file as a "chunk", meaning there will be a callable function at the top of the stack
	const auto stackTopBeforeLoad = lua_gettop(luaState);
	{
		const auto luaResult = luaL_loadfile(luaState, i_sourcePath);
		if (luaResult != LUA_OK)
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, lua_tostring(luaState, -1));
			lua_pop(luaState, 1);
			return eae6320::Results::Failure;;
		}
	}
	// Execute the "chunk", which should load the asset into a table at the top of the stack
	{
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		constexpr int noMessageHandler = 0;
		const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, "Lua asset files must return a table");
					lua_pop(luaState, 1);
					return eae6320::Results::InvalidFile;
				}
			}
			else
			{
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, "Lua asset files must return a single table");
				lua_pop(luaState, returnedValueCount);
				return eae6320::Results::InvalidFile;
			}
		}
		else
		{
			eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, lua_tostring(luaState, -1));
			lua_pop(luaState, 1);
			return eae6320::Results::InvalidFile;
		}
	}
	// If this code is reached the asset file was loaded successfully, and its table is now at index -1
	// Load the values, assuming that the asset table is at index -1
	{
		LoadLuaVertexData(*luaState, i_sourcePath, "vertices", s_vertexNum, s_vertexData);
		LoadLuaIndexData(*luaState, i_sourcePath, "indices", s_indexNum, s_indexData);
	}
	// Pop the asset and clean up lua state (This can be done by using scope guard)
	{
		lua_pop(luaState, 1);
		if (luaState)
		{
			EAE6320_ASSERT(lua_gettop(luaState) == 0);
			lua_close(luaState);
			luaState = nullptr;
		}
	}

	return result;
}


eae6320::cResult eae6320::Assets::cAudioBuilder::LoadLuaVertexData(
	lua_State& io_luaState, const char* const i_sourcePath, const std::string& i_key,
	uint32_t& o_arrayLen, Graphics::VertexFormats::sVertex_mesh*& o_array)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the array table will be at -1:
	lua_pushstring(&io_luaState, i_key.c_str());
	lua_gettable(&io_luaState, -2);
	eae6320::cScopeGuard scopeGuard_popVertices([&io_luaState]
		{
			lua_pop(&io_luaState, 1);
		});

	// Safety check
	if (!lua_istable(&io_luaState, -1))
	{
		std::string errorMessage = "The value at \"" + i_key + "\" must be a table.";
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, errorMessage.c_str());
		return eae6320::Results::InvalidFile;;
	}

	const auto arraySize = luaL_len(&io_luaState, -1);
	o_arrayLen = static_cast<uint32_t>(arraySize);
	o_array = new eae6320::Graphics::VertexFormats::sVertex_mesh[o_arrayLen];

	// Note that each vertex is represented as a lua table, with vertex's
	// x, y, z positon data and r, g, b color data as the value of the table. 
	// We need to load the top table that store all single vertex tables first,
	// then iterate all vertex tables to load the exact vertex data.
	for (int i = 1; i <= arraySize; i++)
	{
		// Load the table that stores data of a single vertex
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popVertexTable([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});

		lua_Number value;
		std::string key = "";
		float* target = nullptr;
		// Load the position data from the vertex table
		for (int j = 0; j < s_numPosPerVertex; j++)
		{
			switch (j)
			{
			case 0: { key = "x", target = &(o_array[(i - 1)].x); break; }
			case 1: { key = "y", target = &(o_array[(i - 1)].y); break; }
			case 2: { key = "z", target = &(o_array[(i - 1)].z); break; }
			}

			lua_pushstring(&io_luaState, key.c_str());
			lua_gettable(&io_luaState, -2);
			eae6320::cScopeGuard scopeGuard_popPositionData([&io_luaState] { lua_pop(&io_luaState, 1); });
			if (lua_isnil(&io_luaState, -1) || lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::string errorMessage = "The position values at \"" + i_key + "\" must exist and be number values.";
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, errorMessage.c_str());
				return eae6320::Results::InvalidFile;
			}
			value = lua_tonumber(&io_luaState, -1);
			*target = static_cast<float>(value);
		}
		// Load the position data from the vertex table
		for (int j = 0; j < s_numColorPerVertex; j++)
		{
			switch (j)
			{
			case 0: { key = "r", target = &(o_array[(i - 1)].r); break; }
			case 1: { key = "g", target = &(o_array[(i - 1)].g); break; }
			case 2: { key = "b", target = &(o_array[(i - 1)].b); break; }
			case 3: { key = "a", target = &(o_array[(i - 1)].a); break; }
			}

			lua_pushstring(&io_luaState, key.c_str());
			lua_gettable(&io_luaState, -2);
			eae6320::cScopeGuard scopeGuard_popPositionData([&io_luaState] { lua_pop(&io_luaState, 1); });
			if (lua_isnil(&io_luaState, -1) || lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::string errorMessage = "The color values at \"" + i_key + "\" must exist and be number values.";
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, errorMessage.c_str());
				return eae6320::Results::InvalidFile;
			}
			value = lua_tonumber(&io_luaState, -1);
			*target = static_cast<float>(value);
		}
	}

	return result;
}


eae6320::cResult eae6320::Assets::cAudioBuilder::LoadLuaIndexData(
	lua_State& io_luaState, const char* const i_sourcePath, const std::string& i_key,
	uint32_t& o_arrayLen, uint16_t*& o_array)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the array table will be at -1:
	lua_pushstring(&io_luaState, i_key.c_str());
	lua_gettable(&io_luaState, -2);
	eae6320::cScopeGuard scopeGuard_popIndices([&io_luaState]
		{
			lua_pop(&io_luaState, 1);
		});

	// Safety check
	if (!lua_istable(&io_luaState, -1))
	{
		std::string errorMessage = "The value at \"" + i_key + "\" must be a table.";
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, errorMessage.c_str());
		return eae6320::Results::InvalidFile;;
	}

	const auto arraySize = luaL_len(&io_luaState, -1);
	o_arrayLen = static_cast<uint32_t>(arraySize * s_numIndexPerTriangle);
	o_array = new uint16_t[o_arrayLen];

	// Similar with how the vertex data is stored in file, index data 
	// is stored in lua tables in groups of three, as triangles are
	// represented by exactly three indices.
	for (int i = 1; i <= arraySize; i++)
	{
		// Load the table that stores index data of a single triangle
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popIndexTable([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});

		// Load the exact index data from the index table
		lua_Number value;
		for (int j = 0; j < s_numIndexPerTriangle; j++)
		{
			std::string key = "";
			uint16_t* target = nullptr;
			int targetIndex = (i - 1) * s_numIndexPerTriangle;

			// Platform-specific indices winding order
#if defined ( EAE6320_PLATFORM_D3D )
			switch (j)
			{
			case 0: { key = "u", target = &(o_array[targetIndex + j]); break; }
			case 1: { key = "v", target = &(o_array[targetIndex + j + 1]); break; }
			case 2: { key = "w", target = &(o_array[targetIndex + j - 1]); break; }
			}
#elif defined (EAE6320_PLATFORM_GL)
			switch (j)
			{
			case 0: { key = "u", target = &(o_array[targetIndex + j]); break; }
			case 1: { key = "v", target = &(o_array[targetIndex + j]); break; }
			case 2: { key = "w", target = &(o_array[targetIndex + j]); break; }
			}
#endif

			lua_pushstring(&io_luaState, key.c_str());
			lua_gettable(&io_luaState, -2);
			eae6320::cScopeGuard scopeGuard_popVetexData([&io_luaState] { lua_pop(&io_luaState, 1); });
			if (lua_isnil(&io_luaState, -1) || lua_type(&io_luaState, -1) != LUA_TNUMBER)
			{
				std::string errorMessage = "The table values at \"" + i_key + "\" must exist and be number values.";
				eae6320::Assets::OutputErrorMessageWithFileInfo(i_sourcePath, errorMessage.c_str());
				return eae6320::Results::InvalidFile;
			}
			value = lua_tonumber(&io_luaState, -1);
			*target = static_cast<uint16_t>(value);
		}
	}

	return result;
}


eae6320::cResult eae6320::Assets::cAudioBuilder::WriteDataToAsset(const char* const i_targetPath)
{
	//Safety Check
	if (s_vertexData == nullptr)
	{
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_targetPath, "Vertex data not exist, fail to write vertex data to binary file");
		return eae6320::Results::InvalidFile;
	}
	if (s_indexData == nullptr)
	{
		eae6320::Assets::OutputErrorMessageWithFileInfo(i_targetPath, "Index data not exist, fail to write index data to binary file");
		return eae6320::Results::InvalidFile;
	}

	std::ofstream outFile;

	// Write vertex number
	{
		outFile.open(i_targetPath, std::ios::binary);
		outFile.write(reinterpret_cast<char*>(&s_vertexNum), sizeof(uint32_t));
		outFile.close();
	}
	// Write vertex data 
	{
		outFile.open(i_targetPath, std::ios::app | std::ios::binary);
		outFile.write(reinterpret_cast<char*>(s_vertexData), sizeof(Graphics::VertexFormats::sVertex_mesh) * static_cast<uint64_t>(s_vertexNum));
		outFile.close();
	}
	// Write index number
	{
		outFile.open(i_targetPath, std::ios::app | std::ios::binary);
		outFile.write(reinterpret_cast<char*>(&s_indexNum), sizeof(uint32_t));
		outFile.close();
	}
	// Write index data
	{
		outFile.open(i_targetPath, std::ios::app | std::ios::binary);
		outFile.write(reinterpret_cast<char*>(s_indexData), sizeof(uint16_t) * static_cast<uint64_t>(s_indexNum));
		outFile.close();
	}

	// Clean up data
	delete[] s_vertexData;
	delete[] s_indexData;
	s_vertexData = nullptr;
	s_indexData = nullptr;
	return eae6320::Results::Success;
}







