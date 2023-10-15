/*
	This is the standard vertex shader

	A vertex shader is responsible for two things:
		* Telling the GPU where the vertex (one of the three in a triangle) should be drawn on screen in a given window
			* The GPU will use this to decide which fragments (i.e. pixels) need to be shaded for a given triangle
		* Providing any data that a corresponding fragment shader will need
			* This data will be interpolated across the triangle and thus vary for each fragment of a triangle that gets shaded
*/

#include <Shaders/shaders.inc>

// Constant Buffers
//=================

// Constant buffer frame
DeclareConstantBuffer(g_constantBuffer_frame, 0)
{
	Matrix4 g_transform_worldToCamera;
	Matrix4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	Vector2 g_padding;
};

// Constant buffer draw call
DeclareConstantBuffer(g_constantBuffer_drawCall, 2)
{
	Matrix4 g_transform_localToWorld;
};

#if defined( EAE6320_PLATFORM_D3D )

// Input
//======
// The "semantics" (the keywords in all caps after the colon) are arbitrary,
// but must match the C call to CreateInputLayout()
// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code

// Output
//=======
// An SV_POSITION value must always be output from every vertex shader
// so that the GPU can figure out which fragments need to be shaded

#elif defined( EAE6320_PLATFORM_GL )

// Input
//=======
// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()
// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code

layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in Vector4 i_vertexColor;

layout( location = 0 ) out Vector4 vertexColor;

// Output
//=======
// The vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an automatically-required variable named "gl_Position"

#endif


// Entry Point
//============

VertexMain(i_vertexPosition_local, o_vertexPosition_projected, i_vertexColor, vertexColor)
{
	// Transform the local vertex into world space
	Vector4 vertexPosition_world;
	{
		// This will be done in a future assignment.
		// For now, however, local space is treated as if it is the same as world space.
		Vector4 vertexPosition_local = Vector4( i_vertexPosition_local, 1.0 );
		vertexPosition_world = MatrixMul(g_transform_localToWorld, vertexPosition_local);
	}
	// Calculate the position of this vertex projected onto the display
	{
		// Transform the vertex from world space into camera space
		Vector4 vertexPosition_camera = MatrixMul(g_transform_worldToCamera, vertexPosition_world);

		// Project the vertex from camera space into projected space
#if defined( EAE6320_PLATFORM_D3D )
		o_vertexPosition_projected = MatrixMul(g_transform_cameraToProjected, vertexPosition_camera);
		vertexColor = i_vertexColor;
#elif defined( EAE6320_PLATFORM_GL )
		gl_Position = MatrixMul(g_transform_cameraToProjected, vertexPosition_camera);
		vertexColor = i_vertexColor;
#endif
	}
}




// VertexMain(i_vertexPosition_local, o_vertexPosition_projected)
// {
// 	// Transform the local vertex into world space
// 	Vector4 vertexPosition_world;
// 	{
// 		// This will be done in a future assignment.
// 		// For now, however, local space is treated as if it is the same as world space.
// 		Vector4 vertexPosition_local = Vector4( i_vertexPosition_local, 1.0 );
// 		vertexPosition_world = MatrixMul(g_transform_localToWorld, vertexPosition_local);
// 	}
// 	// Calculate the position of this vertex projected onto the display
// 	{
// 		// Transform the vertex from world space into camera space
// 		Vector4 vertexPosition_camera = MatrixMul(g_transform_worldToCamera, vertexPosition_world);

// 		// Project the vertex from camera space into projected space
// #if defined( EAE6320_PLATFORM_D3D )
// 		o_vertexPosition_projected = MatrixMul(g_transform_cameraToProjected, vertexPosition_camera);
// #elif defined( EAE6320_PLATFORM_GL )
// 		gl_Position = MatrixMul(g_transform_cameraToProjected, vertexPosition_camera);
// #endif
// 	}

// }
