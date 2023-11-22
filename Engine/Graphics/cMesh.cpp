// Includes
//=========


#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Platform/Platform.h>
#include <Engine/ScopeGuard/cScopeGuard.h>
#include <Engine/UserOutput/UserOutput.h>
#include <External/Lua/Includes.h>
#include <iostream>
#include <new>


// Helper Function Declarations
//=============================
namespace
{
	constexpr unsigned int s_numPosPerVertex = 3;
	constexpr unsigned int s_numColorPerVertex = 4;
	constexpr unsigned int s_numIndexPerTriangle = 3;

	eae6320::cResult LoadLuaValue(lua_State& io_luaState, const std::string& i_key, uint32_t& o_value);
	eae6320::cResult LoadLuaArray_Vertex(lua_State& io_luaState, const std::string& i_key, eae6320::Graphics::VertexFormats::sVertex_mesh*& o_array, uint32_t& o_arrayLen);
	eae6320::cResult LoadLuaArray_Index(lua_State& io_luaState, const std::string& i_key, uint16_t*& o_array, uint32_t& o_arrayLen);
}


// Member Function Definitions
//=============================

eae6320::cResult eae6320::Graphics::cMesh::Create(
	cMesh*& o_mesh,
	VertexFormats::sVertex_mesh i_vertexData[],
	const uint32_t i_vertexCount,
	uint16_t i_indexData[],
	const uint32_t i_indexCount,
	const uint32_t i_indexCountToRender,
	const uint32_t i_indexOfFirstIndexToUse,
	const uint32_t i_offsetToAddToEachIndex)
{

	// Wait for the graphics thread finishes the rendering of last frame
	{
		Graphics::WaitUntilRenderingOfCurrentFrameIsCompleted(~unsigned int(0u));
		Graphics::ResetThatExistRenderObjectNotInitializedYet();
		UserOutput::ConsolePrint("cMesh: Start Initializeing \n");
	}


	auto result = Results::Success;
	cMesh* newMesh = nullptr;

	// If a new mesh instance is successfully created, pass it out from 
	// the function. Otherwise, clean up the instance.
	// This logic needs to be executed at the end of the initialization process
	// ScopeGuard will make sure it must be executed before exiting current function scope.
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				EAE6320_ASSERT(newMesh != nullptr);
				o_mesh = newMesh;
			}
			else
			{
				if (newMesh)
				{
					newMesh->DecrementReferenceCount();
					newMesh = nullptr;
				}
				o_mesh = nullptr;
			}

			Graphics::SignalThatAllRenderObjectsHaveBeenInitialized();
			UserOutput::ConsolePrint("cMesh: Finish Initializeing \n");
		});

	// Allocate a new mesh
	{
		// Prevent program crash if not enough memory
		newMesh = new (std::nothrow) cMesh();
		if (!newMesh)
		{
			result = eae6320::Results::OutOfMemory;
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			return result;
		}
	}
	// Initialize the platform-specific graphics API mesh object
	if ( !(result = newMesh->Initialize(
		i_vertexData, i_vertexCount, 
		i_indexData, i_indexCount,
		i_indexCountToRender,
		i_indexOfFirstIndexToUse, 
		i_offsetToAddToEachIndex)))
	{
		EAE6320_ASSERTF(false, "Initialization of new mesh failed");
		return result;
	}

	return result;
}


eae6320::cResult eae6320::Graphics::cMesh::Create(
	cMesh*& o_mesh, 
	const std::string& i_meshPath)
{

	// Wait for the graphics thread finishes the rendering of last frame
	{
		Graphics::WaitUntilRenderingOfCurrentFrameIsCompleted(~unsigned int(0u));
		Graphics::ResetThatExistRenderObjectNotInitializedYet();
		UserOutput::ConsolePrint("cMesh: Start Initializeing \n");
	}


	auto result = eae6320::Results::Success;
	cMesh* newMesh = nullptr;

	// If a new mesh instance is successfully created, pass it out from 
	// the function. Otherwise, clean up the instance.
	// This logic needs to be executed at the end of the initialization process
	// ScopeGuard will make sure it must be executed before exiting current function scope.
	cScopeGuard scopeGuard([&o_mesh, &result, &newMesh]
		{
			if (result)
			{
				EAE6320_ASSERT(newMesh != nullptr);
				o_mesh = newMesh;
			}
			else
			{
				if (newMesh)
				{
					newMesh->DecrementReferenceCount();
					newMesh = nullptr;
				}
				o_mesh = nullptr;
			}

			Graphics::SignalThatAllRenderObjectsHaveBeenInitialized();
			UserOutput::ConsolePrint("cMesh: Finish Initializeing \n");
		});

	// Allocate a new mesh
	{
		// Prevent program crash if not enough memory
		newMesh = new (std::nothrow) cMesh();
		if (!newMesh)
		{
			EAE6320_ASSERTF(false, "Couldn't allocate memory for the mesh");
			Logging::OutputError("Failed to allocate memory for the mesh");
			return eae6320::Results::OutOfMemory;
		}
	}


	// Load data from mesh file and create a new mesh object using those data
	// If user provide a path to binary mesh asset file
	if (i_meshPath.find(".mesh") != std::string::npos)
	{
		if (!(result = newMesh->LoadBinaryAsset(i_meshPath.c_str())))
		{
			EAE6320_ASSERTF(false, "Load binary data to create a new mesh failed");
			return result;
		}
	}
	// If user provide a path to Lua mesh asset file
	else if (i_meshPath.find(".lua") != std::string::npos)
	{
		if (!(result = newMesh->LoadLuaAsset(i_meshPath.c_str())))
		{
			EAE6320_ASSERTF(false, "Load lua data to create a new mesh failed");
			return result;
		}
	}
	else
	{
		EAE6320_ASSERTF(false, "Cannot load invalid mesh asset file");
		Logging::OutputError("Cannot load invalid mesh asset file");
		return eae6320::Results::InvalidFile;
	}

	return result;
}


eae6320::cResult eae6320::Graphics::cMesh::LoadLuaAsset(const char* const i_path)
{
	auto result = eae6320::Results::Success;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			eae6320::Logging::OutputMessage("Failed to create a new Lua state");
			std::cerr << "Failed to create a new Lua state" << std::endl;
			return eae6320::Results::OutOfMemory;;
		}
	}

	// Load the asset file as a "chunk", meaning there will be a callable function at the top of the stack
	const auto stackTopBeforeLoad = lua_gettop(luaState);
	{
		const auto luaResult = luaL_loadfile(luaState, i_path);
		if (luaResult != LUA_OK)
		{
			std::cerr << lua_tostring(luaState, -1) << std::endl;
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
					std::cerr << "Asset files must return a table " << std::endl;
					lua_pop(luaState, 1);
					return eae6320::Results::InvalidFile;
				}
			}
			else
			{
				std::cerr << "Asset files must return a single table " << std::endl;
				lua_pop(luaState, returnedValueCount);
				return eae6320::Results::InvalidFile;
			}
		}
		else
		{
			std::cerr << lua_tostring(luaState, -1);
			lua_pop(luaState, 1);
			return eae6320::Results::InvalidFile;
		}
	}

	// If this code is reached the asset file was loaded successfully, and its table is now at index -1
	// Load the values, assuming that the asset table is at index -1
	{
		uint32_t vertexNum = 0;
		uint32_t indexNum = 0;
		VertexFormats::sVertex_mesh* vertexData = nullptr;
		uint16_t* indexData = nullptr;
		uint32_t indexOfFirstIndexToUse = 0;
		uint32_t offsetToAddToEachIndex = 0;

		result = LoadLuaValue(*luaState, "indexOfFirstIndexToUse", indexOfFirstIndexToUse);
		result = LoadLuaValue(*luaState, "offsetToAddToEachIndex", offsetToAddToEachIndex);
		result = LoadLuaArray_Vertex(*luaState, "vertices", vertexData, vertexNum);
		result = LoadLuaArray_Index(*luaState, "indices", indexData, indexNum);

		Initialize(
			vertexData, vertexNum,
			indexData, indexNum,
			indexNum,
			indexOfFirstIndexToUse, 
			offsetToAddToEachIndex);

		delete[] vertexData;
		delete[] indexData;
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


eae6320::cResult eae6320::Graphics::cMesh::LoadBinaryAsset(const char* const i_path)
{
	auto result = Results::Success;
	Platform::sDataFromFile dataFromFile;

	// Load the source code
	{
		std::string errorMessage;
		if (!(result = Platform::LoadBinaryFile(i_path, dataFromFile, &errorMessage)))
		{
			EAE6320_ASSERTF(false, "Failed to load mesh source file: %s", errorMessage.c_str());
			Logging::OutputError("Failed to load mesh source file: %s", errorMessage.c_str());
			return result;
		}
	}

	uint32_t vertexNum = 0;
	uint32_t indexNum = 0;
	VertexFormats::sVertex_mesh* vertexData = nullptr;
	uint16_t* indexData = nullptr;
	uint32_t indexOfFirstIndexToUse = 0;
	uint32_t offsetToAddToEachIndex = 0;
	
	uintptr_t currentOffset = reinterpret_cast<uintptr_t>(dataFromFile.data);
	uintptr_t finalOffset = currentOffset + dataFromFile.size;

	// Load vertex number
	{
		memcpy(&vertexNum, reinterpret_cast<void*>(currentOffset), sizeof(vertexNum));
		currentOffset += sizeof(vertexNum);
	}
	// Load vertex data
	{
		vertexData = new VertexFormats::sVertex_mesh[vertexNum];
		memcpy(reinterpret_cast<void*>(vertexData), reinterpret_cast<void*>(currentOffset), sizeof(VertexFormats::sVertex_mesh) * vertexNum);
		currentOffset += sizeof(VertexFormats::sVertex_mesh) * vertexNum;
	}
	// Load index number
	{
		memcpy(&indexNum, reinterpret_cast<void*>(currentOffset), sizeof(indexNum));
		currentOffset += sizeof(indexNum);
	}
	// Load index data
	{
		indexData = new uint16_t[indexNum];
		memcpy(reinterpret_cast<void*>(indexData), reinterpret_cast<void*>(currentOffset), sizeof(uint16_t) * indexNum);
		currentOffset += sizeof(uint16_t) * indexNum;
	}

	EAE6320_ASSERT(currentOffset == finalOffset);

	Initialize(
		vertexData, vertexNum, 
		indexData, indexNum, 
		indexNum,
		indexOfFirstIndexToUse, 
		offsetToAddToEachIndex);

	delete[] vertexData;
	delete[] indexData;
	return result;
}


eae6320::Graphics::cMesh::~cMesh()
{
	EAE6320_ASSERT(m_referenceCount == 0);
	const auto result = CleanUp();
	EAE6320_ASSERT(result);
}


// Helper Function Definitions
//============================

namespace
{

eae6320::cResult LoadLuaValue(lua_State& io_luaState, const std::string& i_key, uint32_t& o_value)
{
	auto result = eae6320::Results::Success;

	{
		// Remember in Lua you access a table's value like this:
		//	v = t[k]
		// where t is the table, k is the key, and v is the value

		// In C/C++ code you access a table's value like this pseudocode:
		//	lua_push( lua_State*, k );
		//	lua_gettable( lua_State*, t );
		//	v = lua_tovalue( lua_State*, -1 );
		// where t is the _index_ of the table.
		// In words, you:
		//	* Push a table on the stack
		//	* Push the key on the top of the stack
		//	* Call lua_gettable(), providing the index of the table
		//		(the index of the key is required to be -1)
		//	* lua_gettable() will pop the key, and push the value.
		//		In other words, everything on the stack will be as it was before the call
		//		except the key will have been replaced with a value
		//		(which will be nil if you asked for a key that doesn't exist in the table).

		// Right now, the stack only has _one_ object,
		// which is the table from the asset file, at index -1.
		lua_pushstring(&io_luaState, i_key.c_str());

		// Now, the key is at index -1 and the table is at index -2.
		// (Does that make sense why the table is at -2?
		// I think of it like "the table was on top,
		// but when I pushed something new on the top it got pushed down".)

		// We can now call lua_gettable() to get the value associated
		// with the key at index -1:
		constexpr int currentIndexOfTable = -2;
		lua_gettable(&io_luaState, currentIndexOfTable);

		// Now, the value from t[k] is at index -1
		// and the table is still at index -2.

		// The following situations are possible:
		//	* There _was_ a valid value at the key
		//		* It may be the value type that we expect (a string, in this example)
		//		* It may be a different value type (like a boolean, in our current example)
		//	* There was no value at the key

		// When using Lua in C/C++ it is important
		// to always return the stack to its original state.
		// Leaving objects on the stack is kind of like a Lua memory leak.

		// Regardless of what value is currently at index -1
		// it must be popped from the stack when we are done with it.
		// We can use a scope guard to do this automatically when this scope block ends:
		eae6320::cScopeGuard scopeGuard_popName([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});


		// First, make sure that a value (_any_ value) existed for the key:
		if (lua_isnil(&io_luaState, -1))
		{
			std::cerr << "No value for \"" << i_key << "\" was found in the asset table" << std::endl;
			return eae6320::Results::InvalidFile;
		}
		// Make sure the value is the right type
		if (lua_type(&io_luaState, -1) != LUA_TNUMBER)
		{
			std::cerr << "The value for \"" << i_key << "\" must be a number "
				"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
			return eae6320::Results::InvalidFile;
		}
		// Now value stored in the table at the key is valid:
		{
			const auto value = lua_tonumber(&io_luaState, -1);
			o_value = static_cast<uint32_t>(value);
		}
	}

	return result;
}


eae6320::cResult LoadLuaArray_Vertex(lua_State& io_luaState, const std::string& i_key, eae6320::Graphics::VertexFormats::sVertex_mesh*& o_array, uint32_t& o_arrayLen)
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
		std::cerr << "The value at \"" << i_key << "\" must be a table " << std::endl;
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
			if (lua_isnil(&io_luaState, -1) || lua_type(&io_luaState, -1) != LUA_TNUMBER) { return eae6320::Results::InvalidFile; }
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
			if (lua_isnil(&io_luaState, -1) || lua_type(&io_luaState, -1) != LUA_TNUMBER) { return eae6320::Results::InvalidFile; }
			value = lua_tonumber(&io_luaState, -1);
			*target = static_cast<float>(value);
		}
	}


	for (uint32_t i = 0; i < o_arrayLen; i++)
	{
		auto temp1 = o_array[i];
		auto temp2 = 0;
	}



	return result;
}


eae6320::cResult LoadLuaArray_Index(lua_State& io_luaState, const std::string& i_key, uint16_t*& o_array, uint32_t& o_arrayLen)
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
		std::cerr << "The value at \"" << i_key << "\" must be a table " << std::endl;
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
			if (lua_isnil(&io_luaState, -1) || lua_type(&io_luaState, -1) != LUA_TNUMBER) { return eae6320::Results::InvalidFile; }
			value = lua_tonumber(&io_luaState, -1);
			*target = static_cast<uint16_t>(value);
		}

	}

	return result;
}

}