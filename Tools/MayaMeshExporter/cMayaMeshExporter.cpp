// Includes
//=========

#include "cMayaMeshExporter.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <map>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItDag.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// Vertex Definition
//==================

namespace
{
	// This is the vertex struct that will hold the Maya data.
	// Note that this is independent of the eae6320::Graphics::VertexFormats::sMesh struct
	// that is used by the engine to store geometry data!
	// This struct is used to store Maya-specific vertex information
	// and then output it to the human-readable source mesh file.
	struct sVertex_maya
	{
		// Position
		const float x, y, z;
		// Normal
		const float nx, ny, nz;
		// Tangent
		const float tx, ty, tz;
		// Bitangent
		const float btx, bty, btz;
		// Texture coordinates
		const float u, v;
		// Color
		const float r, g, b, a;

		sVertex_maya( const MPoint& i_position, const MFloatVector& i_normal,
			const MFloatVector& i_tangent, const MFloatVector& i_bitangent,
			const float i_texcoordU, const float i_texcoordV,
			const MColor& i_vertexColor )
			:
			x( static_cast<float>( i_position.x ) ), y( static_cast<float>( i_position.y ) ), z( static_cast<float>( i_position.z ) ),
			nx( i_normal.x ), ny( i_normal.y ), nz( i_normal.z ),
			tx( i_tangent.x ), ty( i_tangent.y ), tz( i_tangent.z ),
			btx( i_bitangent.x ), bty( i_bitangent.y ), btz( i_bitangent.z ),
			u( i_texcoordU ), v( i_texcoordV ),
			r( i_vertexColor.r ), g( i_vertexColor.g ), b( i_vertexColor.b ), a( i_vertexColor.a )
		{

		}
	};

	// This stores any information associated with a vertex that is necessary to export the mesh.
	// Most of the information is in the sVertex_maya itself,
	// but there are a few other things needed during processing that shouldn't be exported.
	struct sVertexInfo
	{
		const sVertex_maya vertex;

		// A Maya "shading group" is similar to what we call a "material" in our class
		const size_t shadingGroup;
		// This unique key is calculated in order to decide whether a new vertex should be created or not,
		// and that calculated key is assigned to the vertex so that it can be sorted uniquely
		const std::string uniqueKey;

		sVertexInfo( const MPoint& i_position, const MFloatVector& i_normal,
			const MFloatVector& i_tangent, const MFloatVector& i_bitangent,
			const float i_texcoordU, const float i_texcoordV,
			const MColor& i_vertexColor,
			const size_t i_shadingGroup,
			const std::string& i_uniqueKey )
			:
			vertex( i_position, i_normal, i_tangent, i_bitangent, i_texcoordU, i_texcoordV, i_vertexColor ),
			shadingGroup( i_shadingGroup ),
			uniqueKey( i_uniqueKey )
		{

		}
	};
}

// Static Data
//============

namespace
{
	constexpr size_t s_vertexCountPerTriangle = 3;

	struct sTriangle
	{
		std::string vertexKeys[s_vertexCountPerTriangle];
		size_t shadingGroup;

		static bool CompareTriangles( const sTriangle& i_lhs, const sTriangle& i_rhs )
		{
			// Sort the triangles by shading group
			// (so that a single draw call can work with a single contiguous block of vertex and index data)
			if ( i_lhs.shadingGroup != i_rhs.shadingGroup )
			{
				return i_lhs.shadingGroup < i_rhs.shadingGroup;
			}
			else
			{
				// If two triangles use the same shading group the order doesn't matter,
				// but it's nice to have the exported files be deterministic
				for ( size_t i = 0; i < s_vertexCountPerTriangle; ++i )
				{
					if ( i_lhs.vertexKeys[i] != i_rhs.vertexKeys[i] )
					{
						return i_lhs.vertexKeys[i] < i_rhs.vertexKeys[i];
					}
				}
			}
			// If there are two identical triangles it means that i_lhs isn't less than i_rhs
			return false;
		}
	};

	struct sMaterialInfo
	{
		// As an example, the material node's name (which is useless) is currently stored
		MString nodeName;

		// Keep track of the the range of vertices and indices that use this material
		// (the stored indices are for the final vertex buffer and index buffer vectors)
		struct
		{
			size_t first = std::numeric_limits<size_t>::max(), last = 0;
		} vertexRange;
		struct
		{
			size_t first = std::numeric_limits<size_t>::max(), last = 0;
		} indexRange;
	};
}

// Helper Declarations
//====================

namespace
{
	std::string CreateUniqueVertexKey( const int i_positionIndex, const int i_normalIndex, const int i_tangentIndex,
		const int i_texcoordIndex, const int i_vertexColorIndex, const size_t i_shadingGroupIndex, const char* i_transformName );
	MStatus FillVertexAndIndexArrays( const std::map<std::string, sVertexInfo>& i_uniqueVertices, const std::vector<MObject>& i_shadingGroups,
		std::vector<sTriangle>& io_triangles,
		std::vector<sVertexInfo>& o_vertexArray, std::vector<size_t>& o_indexArray,
		std::vector<sMaterialInfo>& o_materialInfo );
	MStatus ProcessAllObjects( std::map<std::string, sVertexInfo>& o_uniqueVertices, std::vector<sTriangle>& o_triangles,
		std::vector<MObject>& o_shadingGroups );
	MStatus ProcessSelectedObjects( std::map<std::string, sVertexInfo>& o_uniqueVertices, std::vector<sTriangle>& o_triangles,
		std::vector<MObject>& o_shadingGroups );
	MStatus ProcessSingleDagNode( const MDagPath& i_dagPath,
		std::map<std::string, sVertexInfo>& io_uniqueVertices, std::vector<sTriangle>& io_triangles,
		std::vector<MObject>& io_shadingGroups, std::map<std::string, size_t>& io_map_shadingGroupNamesToIndices );
	MStatus WriteMeshToFile( const MString& i_fileName, const std::vector<sVertexInfo>& i_vertexArray, const std::vector<size_t>& i_indexArray,
		const std::vector<sMaterialInfo>& i_materialInfo );

	std::string CreateVertexInfo(const sVertexInfo& i_vertex);
	std::string CreateIndexInfo(const std::vector<size_t>& i_indexArray, size_t index);
}

// Inherited Interface
//====================

MStatus eae6320::cMayaMeshExporter::writer( const MFileObject& i_file, const MString& i_options, FileAccessMode i_mode )
{
	MStatus status;

	// Calculate the vertex and index data
	std::vector<sVertexInfo> vertexArray;
	std::vector<size_t> indexArray;
	std::vector<sMaterialInfo> materialInfo;
	{
		// Gather the vertex and triangle information
		std::map<std::string, sVertexInfo> uniqueVertices;
		std::vector<sTriangle> triangles;
		std::vector<MObject> shadingGroups;
		{
			// The user decides whether to export the entire scene or just a selection
			if ( i_mode == MPxFileTranslator::kExportAccessMode )
			{
				if ( !( status = ProcessAllObjects( uniqueVertices, triangles, shadingGroups ) ) )
				{
					return status;
				}
			}
			else if ( i_mode == MPxFileTranslator::kExportActiveAccessMode )
			{
				if ( !( status = ProcessSelectedObjects( uniqueVertices, triangles, shadingGroups ) ) )
				{
					return status;
				}
			}
			else
			{
				MGlobal::displayError( "Unexpected file access mode" );
				return MStatus::kFailure;
			}
		}
		// Convert the mesh information to vertex and index arrays
		if ( !( status = FillVertexAndIndexArrays( uniqueVertices, shadingGroups, triangles, vertexArray, indexArray, materialInfo ) ) )
		{
			return status;
		}
	}
	// Write the mesh data to the requested file
	{
		const auto filePath = i_file.resolvedFullName();
		return WriteMeshToFile( filePath, vertexArray, indexArray, materialInfo );
	}
}

// Helper Definitions
//===================

namespace
{
	std::string CreateUniqueVertexKey( const int i_positionIndex, const int i_normalIndex, const int i_tangentIndex,
		const int i_texcoordIndex, const int i_vertexColorIndex, const size_t i_shadingGroupIndex, const char* i_transformName )
	{
		std::ostringstream vertexKey;
		vertexKey << i_positionIndex << "_" << i_normalIndex << "_" << i_tangentIndex
			<< "_" << i_texcoordIndex << "_" << i_vertexColorIndex << "_" << i_shadingGroupIndex;
		if ( i_transformName )
		{
			vertexKey << "_" << i_transformName;
		}
		return vertexKey.str();
	}

	MStatus FillVertexAndIndexArrays( const std::map<std::string, sVertexInfo>& i_uniqueVertices, const std::vector<MObject>& i_shadingGroups,
		std::vector<sTriangle>& io_triangles,
		std::vector<sVertexInfo>& o_vertexArray, std::vector<size_t>& o_indexArray,
		std::vector<sMaterialInfo>& o_materialInfo )
	{
		MStatus status;

		// Fill in the material info
		{
			const auto shadingGroupCount = i_shadingGroups.size();
			o_materialInfo.resize( shadingGroupCount );
			for ( size_t i = 0; i < shadingGroupCount; ++i )
			{
				const auto& shadingGroup = i_shadingGroups[i];
				const auto findNetworkedPlugIfPossible = true;
				auto surfaceShaderPlug = MFnDependencyNode( shadingGroup ).findPlug( "surfaceShader", findNetworkedPlugIfPossible, &status );
				if ( status )
				{
					MPlugArray connections;
					{
						constexpr auto getConnectionsWithThisAsDestination = true;
						constexpr auto dontGetConnectionsWithThisAsSource = false;
						surfaceShaderPlug.connectedTo( connections, getConnectionsWithThisAsDestination, dontGetConnectionsWithThisAsSource, &status );
						if ( !status )
						{
							MGlobal::displayError( status.errorString() );
							return status;
						}
					}
					if ( connections.length() == 1 )
					{
						// This is where you would put code to extract relevant information from the material
						sMaterialInfo& o_material = o_materialInfo[i];

						// For now this just gets the material node's name (which is useless),
						// but this could be made more sophisticated
						MFnDependencyNode materialNode( connections[0].node() );
						o_material.nodeName = materialNode.name();
					}
					else if ( connections.length() == 0 )
					{
						// This can happen if a material was assigned to a mesh,
						// but then the material was deleted (while the shading group remained).
						// This example code will still work with a missing material,
						// but if you make the material handling more sophisticated
						// you should make sure to handle this case.
						o_materialInfo[i].nodeName = "UNASSIGNED";
					}
					else
					{
						MGlobal::displayError( MString( "A shading group's surface shader had " ) + connections.length() + " connections" );
						return MStatus::kFailure;
					}
				}
				else
				{
					MGlobal::displayError( status.errorString() );
					return status;
				}
			}
		}

		// Fill the vertex buffer with the vertices
		// and create a map from the unique key to the assigned index in the vertex buffer
		std::map<std::string, size_t> vertexKeyToIndexMap;
		{
			// Create a reverse map with a custom sorting order for the vertices
			struct CompareVertices
			{
				bool operator()( const sVertexInfo& i_lhs, const sVertexInfo& i_rhs ) const
				{
					// Sort the vertices by shading group
					// (so that a single draw call can work with a single contiguous block of vertex data)
					if ( i_lhs.shadingGroup != i_rhs.shadingGroup )
					{
						return i_lhs.shadingGroup < i_rhs.shadingGroup;
					}
					else
					{
						// If two vertices use the same shading group the order doesn't matter
						return i_lhs.uniqueKey < i_rhs.uniqueKey;
					}
				}
			};
			std::map<sVertexInfo, std::string, CompareVertices> sortedVertices;
			for ( auto i = std::as_const( i_uniqueVertices ).begin(); i != i_uniqueVertices.end(); ++i )
			{
				sortedVertices.insert( std::make_pair( i->second, i->first ) );
			}
			// Assign the sorted vertices to the buffer
			size_t vertexIndex = 0;
			for ( auto i = sortedVertices.begin(); i != sortedVertices.end(); ++i, ++vertexIndex )
			{
				const auto& vertex = i->first;
				o_vertexArray.push_back( vertex );
				vertexKeyToIndexMap.insert( std::make_pair( i->second, vertexIndex ) );
				// Update the vertex range for the shading group that this material uses
				if ( vertex.shadingGroup < o_materialInfo.size() )
				{
					auto& materialInfo = o_materialInfo[vertex.shadingGroup];
					materialInfo.vertexRange.first = std::min( vertexIndex, materialInfo.vertexRange.first );
					materialInfo.vertexRange.last = std::max( vertexIndex, materialInfo.vertexRange.last );
				}
			}
		}

		// Fill the index buffer with the indices
		{
			// Sort the triangles by shading group
			std::sort( io_triangles.begin(), io_triangles.end(), sTriangle::CompareTriangles );
			// Assign the triangle indices to the index buffer
			const auto triangleCount = io_triangles.size();
			const auto indexCount = triangleCount * s_vertexCountPerTriangle;
			o_indexArray.resize( indexCount );
			for ( size_t i = 0; i < triangleCount; ++i )
			{
				const auto& triangle = io_triangles[i];
				for ( size_t j = 0; j < s_vertexCountPerTriangle; ++j )
				{
					const auto& vertexKey = triangle.vertexKeys[j];
					const auto triangleIndex = vertexKeyToIndexMap.find( vertexKey )->second;
					const auto indexBufferIndex = ( i * s_vertexCountPerTriangle ) + j;
					o_indexArray[indexBufferIndex] = triangleIndex;
					// Update the index range for the shading group that this material uses
					if ( triangle.shadingGroup < o_materialInfo.size() )
					{
						auto& materialInfo = o_materialInfo[triangle.shadingGroup];
						materialInfo.indexRange.first = std::min( indexBufferIndex, materialInfo.indexRange.first );
						materialInfo.indexRange.last = std::max( indexBufferIndex, materialInfo.indexRange.last );
					}
				}
			}
		}

		return MStatus::kSuccess;
	}

	MStatus ProcessAllObjects( std::map<std::string, sVertexInfo>& o_uniqueVertices, std::vector<sTriangle>& o_triangles,
		std::vector<MObject>& o_shadingGroups )
	{
		MStatus status;

		std::map<std::string, size_t> map_shadingGroupNamesToIndices;
		for ( MItDag i( MItDag::kDepthFirst, MFn::kMesh ); !i.isDone(); i.next() )
		{
			MDagPath dagPath;
			i.getPath( dagPath );
			if ( !( status = ProcessSingleDagNode( dagPath, o_uniqueVertices, o_triangles, o_shadingGroups, map_shadingGroupNamesToIndices ) ) )
			{
				return status;
			}
		}

		return status;
	}

	MStatus ProcessSelectedObjects( std::map<std::string, sVertexInfo>& o_uniqueVertices, std::vector<sTriangle>& o_triangles,
		std::vector<MObject>& o_shadingGroups )
	{
		MStatus status;

		// Iterate through each selected object
		MSelectionList selectionList;
		if ( status = MGlobal::getActiveSelectionList( selectionList ) )
		{
			std::map<std::string, size_t> map_shadingGroupNamesToIndices;
			for ( MItSelectionList i( selectionList, MFn::kMesh ); !i.isDone(); i.next() )
			{
				MDagPath dagPath;
				i.getDagPath( dagPath );
				if ( !( status = ProcessSingleDagNode( dagPath, o_uniqueVertices, o_triangles, o_shadingGroups, map_shadingGroupNamesToIndices ) ) )
				{
					return status;
				}
			}
		}
		else
		{
			MGlobal::displayError( MString( "Failed to get active selection list: " ) + status.errorString() );
			return status;
		}

		return status;
	}

	MStatus ProcessSingleDagNode( const MDagPath& i_dagPath,
		std::map<std::string, sVertexInfo>& io_uniqueVertices, std::vector<sTriangle>& io_triangles,
		std::vector<MObject>& io_shadingGroups, std::map<std::string, size_t>& io_map_shadingGroupNamesToIndices )
	{
		// Get the mesh from the DAG path
		MFnMesh mesh( i_dagPath );
		if ( mesh.isIntermediateObject() )
		{
			return MStatus::kSuccess;
		}

		// Get a list of the positions
		MPointArray positions;
		{
			const auto status = mesh.getPoints( positions, MSpace::kWorld );
			if ( !status )
			{
				MGlobal::displayError( status.errorString() );
				return status;
			}
		}

		// Get a list of the normals
		MFloatVectorArray normals;
		{
			const auto status = mesh.getNormals( normals, MSpace::kWorld );
			if ( !status )
			{
				MGlobal::displayError( status.errorString() );
				return status;
			}
		}

		// Get a list of tangents and bitangents
		MFloatVectorArray tangents, bitangents;
		{
			constexpr MString* const useDefaultUvSet = nullptr;	// If more than one UV set exists this code will use the "default" one (as chosen by Maya)
			{
				const auto status = mesh.getTangents( tangents, MSpace::kWorld, useDefaultUvSet );
				if ( !status
					// Tangents may not exist if there are no UVs
					&& ( status != MS::kInvalidParameter ) )
				{
					MGlobal::displayError( status.errorString() );
					return status;
				}
			}
			{
				const auto status = mesh.getBinormals( bitangents, MSpace::kWorld, useDefaultUvSet );
				if ( !status
					// Bitngents may not exist if there are no UVs
					&& ( status != MS::kInvalidParameter ) )
				{
					MGlobal::displayError( status.errorString() );
					return status;
				}
			}
		}

		// Get a list of the texture coordinates
		MFloatArray texcoordUs, texcoordVs;
		{
			MStatus status;
			const auto uvSetCount = mesh.numUVSets( &status );
			if ( status )
			{
				if ( uvSetCount > 0 )
				{
					constexpr MString* const useDefaultUvSet = nullptr;	// If more than one UV set exists this code will use the "default" one (as chosen by Maya)
					status = mesh.getUVs( texcoordUs, texcoordVs, useDefaultUvSet );
					if ( !status )
					{
						MGlobal::displayError( status.errorString() );
						return status;
					}
				}
			}
			else
			{
				MGlobal::displayError( status.errorString() );
				return status;
			}
		}

		// Get a list of the vertex colors
		MColorArray vertexColors;
		{
			MStatus status;
			const auto colorSetCount = mesh.numColorSets( &status );
			if ( status )
			{
				if ( colorSetCount > 0 )
				{
					constexpr MString* const useDefaultColorSet = nullptr;	// If more than one color set exists this code will use the "default" one (as chosen by Maya)
					MColor defaultColor( 1.0f, 1.0f, 1.0f, 1.0f );
					status = mesh.getColors( vertexColors, useDefaultColorSet, &defaultColor );
					if ( !status )
					{
						MGlobal::displayError( status.errorString() );
						return status;
					}
				}
			}
			else
			{
				MGlobal::displayError( status.errorString() );
				return status;
			}
		}

		// A single mesh (i.e. geometric data)
		// can be used by multiple DAG nodes in a Maya scene.
		// (For example, a single sphere mesh could be instanced many times
		// but at different positions, with different orientations, scales, and materials.)
		// An instance ID identifies the specific node that should be processed by this function.
		unsigned int instanceId = 0;
		if ( i_dagPath.isInstanced() )
		{
			MStatus status;
			instanceId = i_dagPath.instanceNumber( &status );
			if ( !status )
			{
				MGlobal::displayError( MString( "Failed to get the DAG path's instance number: " ) + status.errorString() );
				return MStatus::kFailure;
			}
		}

		// Get a list of the shading groups (i.e. materials)
		std::vector<size_t> polygonShadingGroupIndices;
		{
			MObjectArray shadingGroups;
			MIntArray localIndices;
			const auto status = mesh.getConnectedShaders( instanceId, shadingGroups, localIndices );
			if ( status )
			{
				// Remap each local shading group index (i.e. that applies to the array returned by getConnectedShaders())
				// to an index into our static list
				std::vector<size_t> shadingGroupIndices;
				{
					const auto shadingGroupCount = shadingGroups.length();
					shadingGroupIndices.resize( shadingGroupCount );
					for ( std::remove_const<decltype( shadingGroupCount )>::type i = 0; i < shadingGroupCount; ++i )
					{
						size_t shadingGroupIndex;
						{
							const auto shadingGroup = shadingGroups[i];
							const std::string shadingGroupName = MFnDependencyNode( shadingGroup ).name().asChar();
							const auto mapLookUp = io_map_shadingGroupNamesToIndices.find( shadingGroupName );
							if ( mapLookUp != io_map_shadingGroupNamesToIndices.end() )
							{
								shadingGroupIndex = mapLookUp->second;
							}
							else
							{
								const auto newIndex = io_shadingGroups.size();
								io_shadingGroups.push_back( shadingGroup );
								io_map_shadingGroupNamesToIndices.insert( std::make_pair( shadingGroupName, newIndex ) );
								shadingGroupIndex = newIndex;
							}
						}
						shadingGroupIndices[i] = shadingGroupIndex;
					}
				}
				// Convert each polygon shading group index
				{
					const auto polygonCount = localIndices.length();
					if ( polygonCount == mesh.numPolygons() )
					{
						polygonShadingGroupIndices.resize( polygonCount );
						for ( std::remove_const<decltype( polygonCount )>::type i = 0; i < polygonCount; ++i )
						{
							const int localIndex = localIndices[i];
							if ( localIndex >= 0 )
							{
								polygonShadingGroupIndices[i] = shadingGroupIndices[static_cast<size_t>( localIndex )];
							}
							else
							{
								// If a polygon doesn't have a shading group the index will be -1
								polygonShadingGroupIndices[i] = static_cast<size_t>( localIndex );
							}
						}
					}
					else
					{
						MGlobal::displayError( MString( "mesh.numPolygons() returned " ) + mesh.numPolygons()
							+ " but mesh.getConnectedShaders() returned " + polygonCount
							+ " indices! According to my understanding of the Maya API this should never happen" );
					}
				}
			}
			else
			{
				MGlobal::displayError( status.errorString() );
				return status;
			}
		}

		// Gather vertex and triangle information
		{
			// Use the name of the transform to ensure uniqueness
			// (This is necessary because uniqueness is otherwise determined by indices within a given mesh.
			// If the actual data (like the position coordinates) was used instead then this could be ignored
			// and two identical vertices from two completely different meshes could be saved as a single one.
			// This should happen rarely in practice, but a production-quality exporter
			// should probably be more strict about testing equivalence to try and save as much memory as possible.)
			const auto* const transformName = MFnDependencyNode( mesh.parent( instanceId ) ).name().asChar();

			MPointArray trianglePositions;
			MIntArray positionIndices;
			size_t polygonIndex = 0;
			for ( MItMeshPolygon i( mesh.object() ); !i.isDone(); i.next(), ++polygonIndex )
			{
				if ( i.hasValidTriangulation() )
				{
					const auto shadingGroup = polygonShadingGroupIndices[polygonIndex];

					// Store information for each vertex in the polygon
					std::map<int, const std::string> indexToKeyMap;
					{
						MIntArray vertices;
						const auto status = i.getVertices( vertices );
						if ( status )
						{
							const auto vertexCount = vertices.length();
							for ( std::remove_const<decltype( vertexCount )>::type j = 0; j < vertexCount; ++j )
							{
								const auto positionIndex = vertices[j];
								const auto normalIndex = i.normalIndex( j );
								int texcoordIndex = -1;
								struct { float u, v; } texcoord = { 0.0f, 0.0f };
								{
									int potentialTexcoordIndex;
									constexpr MString* const useDefaultUvSet = nullptr;
									const auto status = i.getUVIndex( j, potentialTexcoordIndex, useDefaultUvSet );
									if ( status && ( potentialTexcoordIndex >= 0 )
										&& ( static_cast<unsigned int>( potentialTexcoordIndex ) < texcoordUs.length() )
										&& ( static_cast<unsigned int>( potentialTexcoordIndex ) < texcoordVs.length() ) )
									{
										texcoordIndex = potentialTexcoordIndex;
										texcoord.u = texcoordUs[texcoordIndex];
										texcoord.v = texcoordVs[texcoordIndex];
									}
								}
								int tangentIndex = -1;
								MFloatVector tangent( 0.0f, 0.0f, 0.0f );
								MFloatVector bitangent( 0.0f, 0.0f, 0.0f );
								{
									MStatus status;
									const auto potentialTangentIndex = i.tangentIndex( j, &status );
									if ( status && ( potentialTangentIndex >= 0 )
										&& ( static_cast<unsigned int>( potentialTangentIndex ) < tangents.length() )
										&& ( static_cast<unsigned int>( potentialTangentIndex ) < bitangents.length() ) )
									{
										tangentIndex = potentialTangentIndex;
										tangent = tangents[tangentIndex];
										bitangent = bitangents[tangentIndex];
									}
								}
								int vertexColorIndex = -1;
								MColor vertexColor( 1.0f, 1.0f, 1.0f, 1.0f );
								{
									int potentialColorIndex;
									constexpr MString* const useDefaultColorSet = nullptr;
									const auto status = i.getColorIndex( j, potentialColorIndex, useDefaultColorSet );
									if ( status && ( potentialColorIndex >= 0 )
										&& ( static_cast<unsigned int>( potentialColorIndex ) < vertexColors.length() ) )
									{
										vertexColorIndex = potentialColorIndex;
										vertexColor = vertexColors[vertexColorIndex];
									}
								}
								const auto vertexKey = CreateUniqueVertexKey( positionIndex, normalIndex, tangentIndex,
									texcoordIndex, vertexColorIndex, shadingGroup, transformName );
								indexToKeyMap.insert( std::make_pair( positionIndex, vertexKey ) );
								io_uniqueVertices.insert( std::make_pair( vertexKey,
									sVertexInfo( positions[positionIndex], normals[normalIndex],
										tangent, bitangent,
										texcoord.u, texcoord.v,
										vertexColor,
										shadingGroup, vertexKey )
									) );
							}
						}
						else
						{
							MGlobal::displayError( status.errorString() );
							return status;
						}
					}
					// Store information for each individual triangle in the polygon
					{
						const auto triangleCount = [&i]
						{
							int triangleCount = 0;
							i.numTriangles( triangleCount );
							return triangleCount;
						}();
						for ( std::remove_const<decltype( triangleCount )>::type j = 0; j < triangleCount; ++j )
						{
							i.getTriangle( j, trianglePositions, positionIndices );
							if ( static_cast<size_t>( positionIndices.length() ) == s_vertexCountPerTriangle )
							{
								sTriangle triangle;
								for ( unsigned int k = 0; k < static_cast<unsigned int>( s_vertexCountPerTriangle ); ++k )
								{
									const auto positionIndex = positionIndices[k];
									const auto mapLookUp = indexToKeyMap.find( positionIndex );
									if ( mapLookUp != indexToKeyMap.end() )
									{
										triangle.vertexKeys[k] = mapLookUp->second;
									}
									else
									{
										MGlobal::displayError( "A triangle gave a different vertex index than the polygon gave" );
										return MStatus::kFailure;
									}
								}
								triangle.shadingGroup = shadingGroup;
								io_triangles.push_back( triangle );
							}
							else
							{
								MGlobal::displayError( MString( "Triangle #" ) + j + " reports that it has " +
									positionIndices.length() + "! According to my understanding of Maya this should never happen" );
								return MStatus::kFailure;
							}
						}
					}
				}
				else
				{
					MGlobal::displayError( "This mesh has an invalid triangulation" );
					return MStatus::kFailure;
				}
			}
		}

		return MStatus::kSuccess;
	}

	MStatus WriteMeshToFile( const MString& i_fileName, const std::vector<sVertexInfo>& i_vertexArray, const std::vector<size_t>& i_indexArray,
		const std::vector<sMaterialInfo>& i_materialInfo )
	{
		// Maya's coordinate system is right=handed and UVs have (0,0) at the lower left corner.
		// For our class my advice is to keep things the Maya way in your engine (which matches the default OpenGL behavior),
		// but the following shows one way that the data input to this file could be converted to match standard Direct3D:
		//	* POSITION	-> x, y, -z
		//	* NORMAL	-> nx, ny, -nz
		//	* TANGENT	-> tx, ty, -tz
		//	* BITANGENT	-> -btx, -bty, btz
		//	* TEXCOORD	-> u, 1 - v
		//
		//	* triangle index order	-> index_0, index_2, index_1

		std::ofstream fout( i_fileName.asChar() );
		if ( fout.is_open() )
		{
			// Lua should be able to parse scientific notation,
			// but if you run into problems you can uncomment the line below to always used fixed notation
//			fout << std::fixed;

			// Open table
			// (If you want to use JSON instead of Lua you will have to change this slightly)
			fout << "return \n" "{\n";
			// Write vertex data to mesh file
			{
				fout << "	vertices = \n" "	{" "\n";

				for (const sVertexInfo& vertex : i_vertexArray)
				{
					std::string vertexInfo = CreateVertexInfo(vertex);
					fout << vertexInfo;
				}

				fout << "	},\n";
			}
			// Write index data to mesh file
			{
				fout << "	indices = \n" "	{\n" "		-- Using right-handed winding order\n";

				for (size_t i = 0; i < i_indexArray.size(); i += 3)
				{
					std::string indexInfo = CreateIndexInfo(i_indexArray, i);
					fout << indexInfo;
				}

				fout << "	},\n";
			}
			// Write additional data to mesh file
			{
				fout << "	indexOfFirstIndexToUse = 0, \n";
				fout << "	offsetToAddToEachIndex = 0, \n";
			}
			// Close table
			fout << "}" "\n";
			fout.close();

			return MStatus::kSuccess;
		}
		else
		{
			MGlobal::displayError( MString( "Couldn't open " ) + i_fileName + " for writing" );
			return MStatus::kFailure;
		}
	}


	std::string CreateVertexInfo(const sVertexInfo& i_vertex)
	{
		std::string vertexInfo;

		vertexInfo.append("		{ ");

		// Write vertex position data
		vertexInfo += "x = " + std::to_string(i_vertex.vertex.x) + ", ";
		vertexInfo += "y = " + std::to_string(i_vertex.vertex.y) + ", ";
		vertexInfo += "z = " + std::to_string(i_vertex.vertex.z) + ", ";

		// Write vertex normal data
		vertexInfo += "nx = " + std::to_string(i_vertex.vertex.nx) + ", ";
		vertexInfo += "ny = " + std::to_string(i_vertex.vertex.ny) + ", ";
		vertexInfo += "nz = " + std::to_string(i_vertex.vertex.nz) + ", ";

		// Write vertex tangent data
		vertexInfo += "tx = " + std::to_string(i_vertex.vertex.tx) + ", ";
		vertexInfo += "ty = " + std::to_string(i_vertex.vertex.ty) + ", ";
		vertexInfo += "tz = " + std::to_string(i_vertex.vertex.tz) + ", ";

		// Write vertex uv data
		vertexInfo += "u = " + std::to_string(i_vertex.vertex.u) + ", ";
		vertexInfo += "v = " + std::to_string(i_vertex.vertex.v) + ", ";

		// Write vertex color data
		vertexInfo += "r = " + std::to_string(i_vertex.vertex.r) + ", ";
		vertexInfo += "g = " + std::to_string(i_vertex.vertex.g) + ", ";
		vertexInfo += "b = " + std::to_string(i_vertex.vertex.b) + ", ";
		vertexInfo += "a = " + std::to_string(i_vertex.vertex.a) + ", ";

		vertexInfo.append("}, \n");

		return vertexInfo;
	}


	std::string CreateIndexInfo(const std::vector<size_t>& i_indexArray, size_t index)
	{
		std::string indexInfo;

		indexInfo.append("		{ ");

		indexInfo += "u = " + std::to_string(i_indexArray[index + 0]) + ", ";
		indexInfo += "v = " + std::to_string(i_indexArray[index + 1]) + ", ";
		indexInfo += "w = " + std::to_string(i_indexArray[index + 2]) + ", ";

		indexInfo.append("}, \n");

		return indexInfo;
	}

}
