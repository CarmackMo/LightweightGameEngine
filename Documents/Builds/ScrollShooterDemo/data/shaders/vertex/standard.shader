/*
	This is the standard vertex shader

	A vertex shader is responsible for two things:
		* Telling the GPU where the vertex (one of the three in a triangle) should be drawn on screen in a given window
			* The GPU will use this to decide which fragments (i.e. pixels) need to be shaded for a given triangle
		* Providing any data that a corresponding fragment shader will need
			* This data will be interpolated across the triangle and thus vary for each fragment of a triangle that gets shaded
*/



/*
	This file should be #included by all shaders
*/


// Version Information
//====================



// GLSL shaders require the version to be #defined before anything else in the shader

	#version 420




// Variable Types
//=================











// Constant Buffers
//=================








// Matrix Multiplication
//=================








// Main Function
//=================












// Constant Buffers
//=================

// Constant buffer frame
layout( std140, binding = 0 ) uniform g_constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
// For float4 alignment
	vec2 g_padding;
};

// Constant buffer draw call
layout( std140, binding = 2 ) uniform g_constantBuffer_drawCall
{
	mat4 g_transform_localToWorld;
};



// Input
//=======
// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()
// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code

layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec4 i_vertexColor;

layout( location = 0 ) out vec4 vertexColor;

// Output
//=======
// The vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an automatically-required variable named "gl_Position"




// Entry Point
//============

void main()
{
// Transform the local vertex into world space
	vec4 vertexPosition_world;
	{
// This will be done in a future assignment.
// For now, however, local space is treated as if it is the same as world space.
		vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
		vertexPosition_world = g_transform_localToWorld * vertexPosition_local;
	}
// Calculate the position of this vertex projected onto the display
	{
// Transform the vertex from world space into camera space
		vec4 vertexPosition_camera = g_transform_worldToCamera * vertexPosition_world;

// Project the vertex from camera space into projected space




		gl_Position = g_transform_cameraToProjected * vertexPosition_camera;
		vertexColor = i_vertexColor;

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
