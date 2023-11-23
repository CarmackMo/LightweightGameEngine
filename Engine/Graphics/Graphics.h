/*
	This file declares the external interface for the graphics system
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Includes
//=========

#include <Engine/Graphics/ConstantBufferFormats.h>
#include <Engine/Graphics/Configuration.h>
#include <Engine/Results/Results.h>


#include <cstdint>
#include <functional>

#if defined( EAE6320_PLATFORM_WINDOWS )
	#include <Engine/Windows/Includes.h>
#endif



// Interface
//==========

namespace eae6320
{
namespace Graphics
{
	// Submission
	//-----------

	// These functions should be called from the application (on the application loop thread)

	// As the class progresses you will add your own functions for submitting data,
	// but the following is an example (that gets called automatically)
	// of how the application submits the total elapsed times
	// for the frame currently being submitted
	void SubmitElapsedTime(
		const float i_elapsedSecondCount_systemTime, 
		const float i_elapsedSecondCount_simulationTime);

	// Specify the color data to clear the last frame (also set the background for this frame)
	// Black is the default value
	void SubmitBackgroundColor(float i_colorR, float i_colorG, float i_colorB, float i_alpha = 1.0f);


	void SubmitCameraMatrices(
		Math::cMatrix_transformation i_transform_worldToCameraMatrix,
		Math::cMatrix_transformation i_transform_cameraToProjectedMatrix);


	eae6320::cResult SubmitNormalRenderData(
		ConstantBufferFormats::sNormalRender i_normalDataArray[],
		uint32_t i_normalDataCount);


	eae6320::cResult SubmitDebugRenderData(
		ConstantBufferFormats::sDebugRender i_debugDataArray[],
		uint32_t i_debugDataCount);


	// When the application is ready to submit data for a new frame
	// it should call this before submitting anything
	// (or, said another way, it is not safe to submit data for a new frame
	// until this function returns successfully)
	cResult WaitUntilDataForANewFrameCanBeSubmitted( const unsigned int i_timeToWait_inMilliseconds );
	// When the application has finished submitting data for a frame
	// it must call this function
	cResult SignalThatAllDataForAFrameHasBeenSubmitted();


	cResult WaitUntilContextReleaseByRenderingThread(const unsigned int i_timeToWait_inMilliseconds = ~unsigned int(0u));

	cResult SignalThatContextIsReleasedByRenderingThread();

	cResult ResetThatContextIsClaimedByRenderingThread();

	cResult WaitUntilContextReleaseByApplicationThread(const unsigned int i_timeToWait_inMilliseconds = ~unsigned int(0u));

	cResult SignalThatContextIsReleasedByApplicationThread();

	cResult ResetThatContextIsClaimedByApplicationThread();

	void ReleaseShareResource();
	

	// Proudce
	//-------

	void InitializeRenderObjects();

	DWORD AcquireRenderObjectInitMutex(DWORD i_waitTime_MS = INFINITE);

	void ReleaseRenderObjectInitMutex();

	void AddMeshInitializeTask(std::function<void(cMesh*)> i_callback, std::string i_meshPath);

	void AddEffectInitializeTask(std::function<void(cEffect*)> i_callback, std::string i_vertexPath, std::string i_fragmentPath);


	// Render
	//-------

	// This is called (automatically) from the main/render thread.
	// It will render a submitted frame as soon as it is ready
	// (i.e. as soon as SignalThatAllDataForAFrameHasBeenSubmitted() has been called)
	void RenderFrame();


	// Initialize / Clean Up
	//----------------------

	struct sInitializationParameters
	{
#if defined( EAE6320_PLATFORM_WINDOWS )
		HWND mainWindow = NULL;
#if defined( EAE6320_PLATFORM_D3D )
		uint16_t resolutionWidth = 0, resolutionHeight = 0;
#elif defined( EAE6320_PLATFORM_GL )
		HINSTANCE thisInstanceOfTheApplication = NULL;
#endif
#endif
	};

	cResult Initialize( const sInitializationParameters& i_initializationParameters );
	cResult CleanUp();

}// Namespace Graphics
}// Namespace eae6320

#endif	// EAE6320_GRAPHICS_H
