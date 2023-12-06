/*
	This file defines the layout of the constant data
	that the CPU sends to the GPU

	These must exactly match the constant buffer definitions in shader programs.
*/

#ifndef EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H
#define EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H

// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cLine.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/Configuration.h>
#include <Engine/Math/cMatrix_transformation.h>

#include <memory>
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
		std::weak_ptr<cMesh> mesh;
		std::weak_ptr<cEffect> effect;
		Math::cMatrix_transformation transform_localToWorld;

		// Initialize / Clean Up
		//----------------------

		sNormalRender() = default;
		~sNormalRender() = default;

		sNormalRender(std::weak_ptr<cMesh> i_mesh, std::weak_ptr<cEffect> i_effect, Math::cMatrix_transformation i_transform) :
			mesh(i_mesh), effect(i_effect), transform_localToWorld(i_transform)
		{ }

		sNormalRender& operator= (sNormalRender& other)
		{
			mesh = other.mesh;
			effect = other.effect;
			transform_localToWorld = other.transform_localToWorld;
			return *this;
		}

		void Initialize(std::weak_ptr<cMesh> i_mesh, std::weak_ptr<cEffect> i_effect, Math::cMatrix_transformation i_transform)
		{
			mesh = i_mesh;
			effect = i_effect;
			transform_localToWorld = i_transform;
		}

		void CleanUp()
		{
			mesh.reset();
			effect.reset();
		}

		// Implementation 
		//----------------------

		bool IsValid()
		{
			return mesh.expired() == false && effect.expired() == false;
		}
	};


	// Data for rendering debug information
	struct sDebugRender
	{
		std::weak_ptr<cLine> line;
		Math::cMatrix_transformation transform;

		// Initialize / Clean Up
		//----------------------

		sDebugRender() = default;
		~sDebugRender() = default;

		sDebugRender& operator= (sDebugRender& other)
		{
			line = other.line;
			transform = other.transform;
			return *this;
		}

		sDebugRender(std::weak_ptr<cLine> i_line, Math::cMatrix_transformation i_transform) :
			line(i_line), transform(i_transform)
		{ }

		void Initialize(std::weak_ptr<cLine> i_line, Math::cMatrix_transformation i_transform)
		{
			line = i_line;
			transform = i_transform;
		}

		void CleanUp()
		{
			line.reset();
		}

		// Implementation 
		//----------------------

		bool IsValid()
		{
			return line.expired() == false;
		}
	};


}// Namespace ConstantBufferFormats
}// Namespace Graphics
}// Namespace eae6320

#endif	// EAE6320_GRAPHICS_CONSTANTBUFFERFORMATS_H
