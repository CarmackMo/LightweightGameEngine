/*
	This vertex shader is used to create a Direct3D vertex input layout object
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














// Input
//======

// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()

// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code
layout( location = 0 ) in vec3 i_position;

// Output
//=======

// The vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an automatically-required variable named "gl_Position"

// Entry Point
//============

void main()
{
// The shader program is only used by Direct3D
	gl_Position = vec4( i_position, 1.0 );
}


