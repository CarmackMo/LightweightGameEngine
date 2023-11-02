/*
	This file defines the layout of the geometric data
	that the CPU sends to the GPU

	These must exactly match the data passed in to vertex shader programs.
*/

#ifndef EAE6320_GRAPHICS_VERTEXBUFFERFORMATS_H
#define EAE6320_GRAPHICS_VERTEXBUFFERFORMATS_H

// Includes
//=========

#include "Configuration.h"

#include <cstdint>

#if defined( EAE6320_PLATFORM_D3D )
	#include <dxgiformat.h>
#elif defined( EAE6320_PLATFORM_GL )
	#include "OpenGL/Includes.h"
#endif

// Format Definitions
//===================

namespace eae6320
{
namespace Graphics
{
namespace VertexFormats
{
	// In our class we will only have a single vertex format for all 3D geometry ("meshes").
	// In a real game it would be more common to have several different formats
	// (with simpler/smaller formats for simpler shading
	// and more complex and bigger formats for more complicated shading).
	struct sVertex_mesh
	{
		// POSITION
		float x = 0, y = 0, z = 0;
		// COLOR
		float r = 1, g = 1, b = 1, a = 1;

		sVertex_mesh() = default;
		sVertex_mesh(float i_x, float i_y, float i_z) : x(i_x), y(i_y), z(i_z) {}
		sVertex_mesh(float i_x, float i_y, float i_z, float i_r, float i_g, float i_b, float i_a) : 
			x(i_x), y(i_y), z(i_z), r(i_r), g(i_g), b(i_b), a(i_a) {}
	};


	struct sVertex_line
	{
		// POSITION
		float x = 0, y = 0, z = 0;
		// COLOR
		float r = 0, g = 1, b = 0, a = 1;

		sVertex_line() = default;
		sVertex_line(float i_x, float i_y, float i_z) : x(i_x), y(i_y), z(i_z) {}
		sVertex_line(float i_x, float i_y, float i_z, float i_r, float i_g, float i_b, float i_a) : 
			x(i_x), y(i_y), z(i_z), r(i_r), g(i_g), b(i_b), a(i_a) {}
	};


}// Namespace VertexFormats
}// Namespace Graphics
}// Namespace eae6320

#endif	// EAE6320_GRAPHICS_VERTEXBUFFERFORMATS_H
