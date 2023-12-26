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

layout( std140, binding = 0 ) uniform g_constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
// For float4 alignment
	vec2 g_padding;
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
// Output solid white
	o_color = vec4(
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








