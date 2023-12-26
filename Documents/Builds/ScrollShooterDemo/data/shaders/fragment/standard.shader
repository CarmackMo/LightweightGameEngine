/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
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




// Output
//=======
// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen











layout( location = 0 ) in vec4 vertexColor;

out vec4 o_color;




// Entry Point
//============

void main()
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
