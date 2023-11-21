/*
	This file defines the layout of the constant data
	that the CPU sends to the GPU

	These must exactly match the constant buffer definitions in shader programs.
*/

#ifndef EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H
#define EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H

// Includes
//=========

#include "cEffect.h"
#include "cLine.h"
#include "cMesh.h"
#include "Configuration.h"


#include <Engine/Math/cMatrix_transformation.h>
#include <string>
#include <utility>


// Format Definitions
//===================

namespace eae6320
{
namespace Graphics
{
namespace ConstantBufferFormats
{

	// Data that is constant for every frame
	struct sFrame
	{
		Math::cMatrix_transformation g_transform_worldToCamera;
		Math::cMatrix_transformation g_transform_cameraToProjected;

		float g_elapsedSecondCount_systemTime = 0.0f;
		float g_elapsedSecondCount_simulationTime = 0.0f;
		// For float4 alignment // TODO: What is this for ??
		float padding[2] = { 0.0f, 0.0f };
	};


	// Data for rendering an object 
	struct sNormalRender
	{
		cMesh* mesh = nullptr;
		cEffect* effect = nullptr;
		Math::cMatrix_transformation transform_localToWorld;

		// Initialize / Clean Up
		//----------------------

		sNormalRender() = default;
		~sNormalRender() = default;

		sNormalRender& operator= (sNormalRender& other)
		{
			mesh = other.mesh;
			mesh->IncrementReferenceCount();
			effect = other.effect;
			effect->IncrementReferenceCount();
			transform_localToWorld = other.transform_localToWorld;
			return *this;
		}

		sNormalRender(cMesh* i_mesh, cEffect* i_effect, Math::cMatrix_transformation i_transform) :
			mesh(i_mesh), effect(i_effect), transform_localToWorld(i_transform)
		{
			mesh->IncrementReferenceCount();
			effect->IncrementReferenceCount();
		}

		void Initialize(cMesh* i_mesh, cEffect* i_effect, Math::cMatrix_transformation i_transform)
		{
			mesh = i_mesh;
			mesh->IncrementReferenceCount();
			effect = i_effect;
			effect->IncrementReferenceCount();
			transform_localToWorld = i_transform;
		}

		void CleanUp()
		{
			if (mesh != nullptr)
			{
				mesh->DecrementReferenceCount();
				mesh = nullptr;
			}
			if (effect != nullptr)
			{
				effect->DecrementReferenceCount();
				effect = nullptr;				
			}
		}

		// Implementation 
		//----------------------

		bool IsValid()
		{
			return mesh != nullptr && effect != nullptr;
		}
	};


	// Data for rendering debug information
	struct sDebugRender
	{
		cLine* line = nullptr;
		Math::cMatrix_transformation transform;

		// Initialize / Clean Up
		//----------------------

		sDebugRender() = default;
		~sDebugRender() = default;

		sDebugRender& operator= (sDebugRender& other)
		{
			line = other.line;
			line->IncrementReferenceCount();
			transform = other.transform;
			return *this;
		}

		sDebugRender(cLine* i_line, Math::cMatrix_transformation i_transform) : 
			line(i_line), transform(i_transform)
		{
			line->IncrementReferenceCount();
		}

		void Initialize(cLine* i_line, Math::cMatrix_transformation i_transform)
		{
			line = i_line;
			line->IncrementReferenceCount();
			transform = i_transform;
		}

		void CleanUp()
		{
			if (line != nullptr)
			{
				line->DecrementReferenceCount();
				line = nullptr;
			}
		}

		// Implementation 
		//----------------------

		bool IsValid()
		{
			return line != nullptr;
		}
	};


}// Namespace ConstantBufferFormats
}// Namespace Graphics
}// Namespace eae6320

#endif	// EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H
