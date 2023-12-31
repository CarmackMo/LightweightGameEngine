/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

// Constant Buffers
//=================

DeclareConstantBuffer(g_constantBuffer_frame, 0)
{
	Matrix4 g_transform_worldToCamera;
	Matrix4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	Vector2 g_padding;
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
	// Output solid white
	o_color = Vector4(
		// RGB (color)
		1.0, 1.0, 1.0,
		// Alpha (opacity)
		1.0 );
	
	float x = abs(sin(g_elapsedSecondCount_simulationTime));
	float y = abs(tan(g_elapsedSecondCount_simulationTime));
	float z = abs(cos(g_elapsedSecondCount_simulationTime));

	o_color.r = x;
	o_color.g = y;
	o_color.b = z;

	o_color = o_color * vertexColor;
}








