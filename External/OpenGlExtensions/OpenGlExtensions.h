/*
	This file can be used by any project that needs to use OpenGL extensions
*/

#ifndef EAE6320_OPENGLEXTENSIONS_FUNCTIONS_H
#define EAE6320_OPENGLEXTENSIONS_FUNCTIONS_H

// Includes
//=========

// The extension headers rely on GL.h
#include <Engine/Graphics/OpenGL/Includes.h>

// The extension headers come from https://www.khronos.org/registry/OpenGL/index_gl.php
#include "20200815/glext.h"
#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "20200815/wglext.h"
#endif

#include <Engine/Results/Results.h>
#include <string>

// Interface
//==========

// OpenGL Extension Declarations
//------------------------------

extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
extern PFNGLBINDSAMPLERPROC glBindSampler;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESAMPLERSPROC glDeleteSamplers;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLGENSAMPLERSPROC glGenSamplers;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
#if defined( EAE6320_PLATFORM_WINDOWS )
	extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
	extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
#endif

// Initialize / Clean Up
//----------------------

namespace eae6320
{
	namespace OpenGlExtensions
	{
		cResult Load( std::string* const o_errorMessage = nullptr );
	}
}

#endif	// EAE6320_OPENGLEXTENSIONS_FUNCTIONS_H
