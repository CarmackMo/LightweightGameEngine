/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif


// Variable Types
//=================

#if defined (EAE6320_PLATFORM_D3D)

	#define Matrix4 float4x4
	#define Vector2 float2
	#define Vector3 float3
	#define Vector4 float4

#elif defined (EAE6320_PLATFORM_GL)

	#define Matrix4 mat4
	#define Vector2 vec2
	#define Vector3 vec3
	#define Vector4 vec4

#endif


// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )

#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name

#endif


// Matrix Multiplication
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define MatrixMul(i_matrix1, i_matrix2) mul( i_matrix1, i_matrix2 )

#elif defined( EAE6320_PLATFORM_GL )

	#define MatrixMul(i_matrix1, i_matrix2) i_matrix1 * i_matrix2

#endif


// Main Function
//=================



#if defined( EAE6320_PLATFORM_D3D )

	#define VertexMain(input_pos, output_pos, input_color, output_color) void main(in const Vector3 input_pos : POSITION, out Vector4 output_pos : SV_POSITION,\
																				   in const Vector4 input_color : COLOR, out Vector4 output_color : COLOR)
	#define FragmentMain(input_pos, input_color, output) void main(in const Vector4 input_pos : SV_POSITION, in const Vector4 input_color : COLOR, out Vector4 output : SV_TARGET)

#elif defined( EAE6320_PLATFORM_GL )

	#define VertexMain(input_pos, output_pos, input_color, output_color) void main()
	#define FragmentMain(input_pos, input_color, output) void main()

#endif


