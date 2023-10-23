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
	struct sMeshEffectPair
	{
		cMesh* mesh = nullptr;
		cEffect* effect = nullptr;


		// Initialize / Clean Up
		//----------------------

		sMeshEffectPair() = default;
		~sMeshEffectPair() = default;

		sMeshEffectPair& operator= (sMeshEffectPair& other)
		{
			mesh = other.mesh;
			mesh->IncrementReferenceCount();
			effect = other.effect;
			effect->IncrementReferenceCount();
			return *this;
		}

		sMeshEffectPair(cMesh* i_mesh, cEffect* i_effect) :
			mesh(i_mesh), effect(i_effect)
		{
			mesh->IncrementReferenceCount();
			effect->IncrementReferenceCount();
		}

		void Initialize(cMesh* i_mesh, cEffect* i_effect)
		{
			mesh = i_mesh;
			mesh->IncrementReferenceCount();
			effect = i_effect;
			effect->IncrementReferenceCount();
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

	// Data that is constant for a single draw call
	struct sDrawCall
	{
		Math::cMatrix_transformation g_transform_localToWorld;
	};

}// Namespace ConstantBufferFormats
}// Namespace Graphics
}// Namespace eae6320

#endif	// EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H
