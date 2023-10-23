/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
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

// Output
//=======
// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen

#elif defined( EAE6320_PLATFORM_GL )

// Output
//=======
// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen

layout( location = 0 ) in Vector4 vertexColor;

out Vector4 o_color;

#endif


// Entry Point
//============

FragmentMain(i_fragmentPosition, vertexColor, o_color)
{
	// // Output solid white
	// o_color = Vector4(
	// 	// RGB (color)
	// 	1.0, 1.0, 1.0,
	// 	// Alpha (opacity)
	// 	1.0 );

	// Output solid white
	o_color = vertexColor;
}
