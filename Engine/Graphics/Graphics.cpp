// Includes
//=========

#include <Engine/Graphics/Graphics.h>

#include <Engine/Concurrency/cEvent.h>
#include <Engine/Concurrency/cMutex.h>
#include <Engine/Graphics/cConstantBuffer.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/ConstantBufferFormats.h>
#include <Engine/Graphics/cView.h>
#include <Engine/Graphics/sContext.h>
#include <Engine/Logging/Logging.h>
#include <Engine/UserOutput/UserOutput.h>

#include <string>
#include <queue>



// Static Data
//============

namespace
{
	// Memory Budget
	//-------------------------
	constexpr uint32_t s_memoryBudget = 100;


	// Constant buffer object
	eae6320::Graphics::cConstantBuffer s_constantBuffer_frame(eae6320::Graphics::ConstantBufferTypes::Frame);
	eae6320::Graphics::cConstantBuffer s_constantBuffer_drawCall(eae6320::Graphics::ConstantBufferTypes::DrawCall);
	

	// Submission Data
	//-------------------------

	// This struct's data is populated at submission time;
	// it must cache whatever is necessary in order to render a frame or perform a draw call
	struct sDataRequiredToRenderAFrame
	{
		eae6320::Graphics::ConstantBufferFormats::sFrame constantData_frame;

		eae6320::Graphics::ConstantBufferFormats::sNormalRender constantData_normalRender[s_memoryBudget];

		eae6320::Graphics::ConstantBufferFormats::sDebugRender constantData_debugRender[s_memoryBudget];

		// Color data to clear the last frame (set background color for this frame)
		// Black is usually used
		float backgroundColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

	// In our class there will be two copies of the data required to render a frame:
	//	* One of them will be in the process of being populated by the data currently being submitted by the application loop thread
	//	* One of them will be fully populated and in the process of being rendered from in the render thread
	// (In other words, one is being produced while the other is being consumed)
	sDataRequiredToRenderAFrame s_dataRequiredToRenderAFrame[2];
	auto* s_dataBeingSubmittedByApplicationThread_frame = &s_dataRequiredToRenderAFrame[0];
	auto* s_dataBeingRenderedByRenderThread_frame = &s_dataRequiredToRenderAFrame[1];

	// The following two events work together to make sure that
	// the main/render thread and the application loop thread can work in parallel but stay in sync:
	// This event is signaled by the application loop thread when it has finished submitting render data for a frame
	// (the main/render thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenAllDataHasBeenSubmittedFromApplicationThread;
	// This event is signaled by the main/render thread when it has swapped render data pointers.
	// This means that the renderer is now working with all the submitted data it needs to render the next frame,
	// and the application loop thread can start submitting data for the following frame
	// (the application loop thread waits for the signal)
	eae6320::Concurrency::cEvent s_whenDataForANewFrameCanBeSubmittedFromApplicationThread;


	// Render Object Builder
	//-------------------------

	struct sMeshBuilder
	{
		eae6320::Graphics::cMesh** meshPtr = nullptr;
		std::string meshPath;
	};

	struct sEffectBuilder
	{
		eae6320::Graphics::cEffect** effectPtr = nullptr;
		std::string vertexShaderPath;
		std::string fragmentShaderPath;
	};

	struct sLineBuilder
	{
		eae6320::Graphics::cLine** linePtr = nullptr;
		eae6320::Graphics::VertexFormats::sVertex_line* vertexData = nullptr;
		uint32_t vertexCount = 0;
		uint16_t* indexData = nullptr;
		uint32_t indexCount = 0;
	};


	// Rendering Object Initialization / Clean Up Queue
	//-------------------------

	std::queue<sMeshBuilder> s_meshInitializeQueue;

	std::queue<sEffectBuilder> s_effectInitializeQueue;

	std::queue<sLineBuilder> s_lineInitializeQueue;


	std::queue<eae6320::Graphics::cMesh**> s_meshCleanUpQueue;
	std::queue<eae6320::Graphics::cEffect**> s_effectCleanUpQueue;
	std::queue<eae6320::Graphics::cLine**> s_lineCleanUpQueue;


	eae6320::Concurrency::cMutex s_renderObjectInitializeMutex;
	eae6320::Concurrency::cMutex s_renderObjectCleanUpMutex;


	// View Data
	//-------------------------

	eae6320::Graphics::cView s_view;

}


// Helper Declarations
//====================

namespace
{
	eae6320::cResult InitializeViews(const eae6320::Graphics::sInitializationParameters& i_initializationParameters);
}


// Interface
//==========

// Submission
//-----------

void eae6320::Graphics::SubmitElapsedTime(
	const float i_elapsedSecondCount_systemTime, 
	const float i_elapsedSecondCount_simulationTime)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread_frame);
	auto& constantData_frame = s_dataBeingSubmittedByApplicationThread_frame->constantData_frame;
	constantData_frame.g_elapsedSecondCount_systemTime = i_elapsedSecondCount_systemTime;
	constantData_frame.g_elapsedSecondCount_simulationTime = i_elapsedSecondCount_simulationTime;
}


void eae6320::Graphics::SubmitBackgroundColor(float i_colorR, float i_colorG, float i_colorB, float i_alpha)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread_frame);

	s_dataBeingSubmittedByApplicationThread_frame->backgroundColor[0] = i_colorR;
	s_dataBeingSubmittedByApplicationThread_frame->backgroundColor[1] = i_colorG;
	s_dataBeingSubmittedByApplicationThread_frame->backgroundColor[2] = i_colorB;
	s_dataBeingSubmittedByApplicationThread_frame->backgroundColor[3] = i_alpha;
}


void eae6320::Graphics::SubmitCameraMatrices(
	Math::cMatrix_transformation i_transform_worldToCameraMatrix,
	Math::cMatrix_transformation i_transform_cameraToProjectedMatrix)
{
	EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread_frame);
	auto& constantData_frame = s_dataBeingSubmittedByApplicationThread_frame->constantData_frame;
	constantData_frame.g_transform_worldToCamera = i_transform_worldToCameraMatrix;
	constantData_frame.g_transform_cameraToProjected = i_transform_cameraToProjectedMatrix;
}


eae6320::cResult eae6320::Graphics::SubmitNormalRenderData(
	ConstantBufferFormats::sNormalRender i_normalDataArray[],
	uint32_t i_normalDataCount = s_memoryBudget)
{
	if (i_normalDataCount < 0 || s_memoryBudget < i_normalDataCount)
	{
		EAE6320_ASSERTF(false, "Mesh-effect data number exceeds memory budget limit: (%u)", s_memoryBudget);
		Logging::OutputError("Mesh-effect data number exceeds memory budget limit: (%u)", s_memoryBudget);
		UserOutput::Print("Mesh-effect data number exceeds memory budget limit: (%u)", s_memoryBudget);

		return Results::Failure;
	}
	else
	{
		EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread_frame);
		auto& constantData_normalRender = s_dataBeingSubmittedByApplicationThread_frame->constantData_normalRender;

		for (uint32_t i = 0; i < i_normalDataCount; i++)
		{
			if (i_normalDataArray[i].IsValid())
			{
				constantData_normalRender[i].Initialize(
					i_normalDataArray[i].mesh, i_normalDataArray[i].effect,
					i_normalDataArray[i].transform_localToWorld);
			}
		}

		return Results::Success;
	}
}


eae6320::cResult eae6320::Graphics::SubmitDebugRenderData(
	ConstantBufferFormats::sDebugRender i_debugDataArray[],
	uint32_t i_debugDataCount = s_memoryBudget)
{
	if (i_debugDataCount < 0 || s_memoryBudget < i_debugDataCount)
	{
		EAE6320_ASSERTF(false, "Mesh-effect data number exceeds memory budget limit: (%u)", s_memoryBudget);
		Logging::OutputError("Mesh-effect data number exceeds memory budget limit: (%u)", s_memoryBudget);
		UserOutput::Print("Mesh-effect data number exceeds memory budget limit: (%u)", s_memoryBudget);

		return Results::Failure;
	}
	else
	{
		EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread_frame);
		auto& constantData_debugRender = s_dataBeingSubmittedByApplicationThread_frame->constantData_debugRender;

		for (uint32_t i = 0; i < i_debugDataCount; i++)
		{
			if (i_debugDataArray[i].IsValid())
			{
				constantData_debugRender[i].Initialize(i_debugDataArray[i].line, i_debugDataArray[i].transform);
			}
		}

		return Results::Success;
	}
}


eae6320::cResult eae6320::Graphics::WaitUntilDataForANewFrameCanBeSubmitted(const unsigned int i_timeToWait_inMilliseconds)
{
	return Concurrency::WaitForEvent(s_whenDataForANewFrameCanBeSubmittedFromApplicationThread, i_timeToWait_inMilliseconds);
}


eae6320::cResult eae6320::Graphics::SignalThatAllDataForAFrameHasBeenSubmitted()
{
	return s_whenAllDataHasBeenSubmittedFromApplicationThread.Signal();
}


// Render Objects Initialize / Clean Up
//-------

void eae6320::Graphics::InitializeRenderObjects()
{
	if (AcquireRenderObjectInitMutex() == WAIT_OBJECT_0)
	{
		// Initialize mesh objects
		{
			while (s_meshInitializeQueue.empty() == false)
			{
				sMeshBuilder builder = s_meshInitializeQueue.front();
				s_meshInitializeQueue.pop();

				cMesh::Create(*(builder.meshPtr), builder.meshPath);
			}
		}
		// Initialize effect objects
		{
			while (s_effectInitializeQueue.empty() == false)
			{
				sEffectBuilder builder = s_effectInitializeQueue.front();
				s_effectInitializeQueue.pop();

				cEffect::Create(*(builder.effectPtr), builder.vertexShaderPath, builder.fragmentShaderPath);
			}
		}
		// Initialize line objects
		{
			while (s_lineInitializeQueue.empty() == false)
			{
				sLineBuilder builder = s_lineInitializeQueue.front();
				s_lineInitializeQueue.pop();

				cLine::Create(*(builder.linePtr), builder.vertexData, builder.vertexCount, builder.indexData, builder.indexCount);

				delete[] builder.vertexData;
				delete[] builder.indexData;
			}
		}

		ReleaseRenderObjectInitMutex();
	}
	else
	{
		EAE6320_ASSERTF(false, "Waiting for the application thread submit render object initialize tasks failed");
		Logging::OutputError("Waiting for the application thread submit render object initialize tasks failed");
	}
}


void eae6320::Graphics::CleanUpRenderObjects()
{
	if (AcquireRenderObjectCleanUpMutex() == WAIT_OBJECT_0)
	{
		// Clean up mesh objects
		{
			while (s_meshCleanUpQueue.empty() == false)
			{
				cMesh** task = s_meshCleanUpQueue.front();
				s_meshCleanUpQueue.pop();

				(*task)->DecrementReferenceCount();
				(*task) = nullptr;
			}
		}
		// Clean up effect objects
		{
			while (s_effectCleanUpQueue.empty() == false)
			{
				cEffect** task = s_effectCleanUpQueue.front();
				s_effectCleanUpQueue.pop();

				(*task)->DecrementReferenceCount();
				(*task) = nullptr;
			}
		}
		// Clean up line objects
		{
			while (s_lineCleanUpQueue.empty() == false)
			{
				cLine** task = s_lineCleanUpQueue.front();
				s_lineCleanUpQueue.pop();

				(*task)->DecrementReferenceCount();
				(*task) = nullptr;
			}
		}

		ReleaseRenderObjectCleanUpMutex();
	}
	else
	{
		EAE6320_ASSERTF(false, "Waiting for the application thread submit render object clean up tasks failed");
		Logging::OutputError("Waiting for the application thread submit render object clean up tasks failed");
	}

}


DWORD eae6320::Graphics::AcquireRenderObjectInitMutex(DWORD i_waitTime_MS)
{
	return s_renderObjectInitializeMutex.Acquire(i_waitTime_MS);
}


void eae6320::Graphics::ReleaseRenderObjectInitMutex()
{
	s_renderObjectInitializeMutex.Release();
}


DWORD eae6320::Graphics::AcquireRenderObjectCleanUpMutex(DWORD i_waitTime_MS)
{
	return s_renderObjectCleanUpMutex.Acquire(i_waitTime_MS);
}


void eae6320::Graphics::ReleaseRenderObjectCleanUpMutex()
{
	s_renderObjectCleanUpMutex.Release();
}


void eae6320::Graphics::AddMeshInitializeTask(cMesh** i_meshPtr, std::string i_meshPath)
{
	s_meshInitializeQueue.push({ i_meshPtr, i_meshPath });
}


void eae6320::Graphics::AddEffectInitializeTask(cEffect** i_effectPtr, std::string i_vertexShaderPath, std::string i_fragmentShaderPath)
{
	s_effectInitializeQueue.push({ i_effectPtr, i_vertexShaderPath , i_fragmentShaderPath });
}


void eae6320::Graphics::AddLineInitializeTask(cLine** i_linePtr, VertexFormats::sVertex_line i_vertexData[], const uint32_t i_vertexCount, uint16_t i_indexData[], const uint32_t i_indexCount)
{
	VertexFormats::sVertex_line* vertexData = new VertexFormats::sVertex_line[i_vertexCount];
	for (uint32_t i = 0; i < i_vertexCount; i++)
	{
		vertexData[i] = i_vertexData[i];
	}
	uint16_t* indexData = new uint16_t[i_indexCount];
	for (uint32_t i = 0; i < i_indexCount; i++)
	{
		indexData[i] = i_indexData[i];
	}

	s_lineInitializeQueue.push({ i_linePtr, vertexData, i_vertexCount, indexData, i_indexCount });
}


void eae6320::Graphics::AddMeshCleanUpTask(cMesh** i_mesh)
{
	s_meshCleanUpQueue.push(i_mesh);
}


void eae6320::Graphics::AddEffectCleanUpTask(cEffect** i_effect)
{
	s_effectCleanUpQueue.push(i_effect);
}


void eae6320::Graphics::AddLineCleanUpTask(cLine** i_line)
{
	s_lineCleanUpQueue.push(i_line);
}


// Render
//-------

void eae6320::Graphics::RenderFrame()
{

	// Wait for the application loop to submit data to be rendered
	{
		if (Concurrency::WaitForEvent(s_whenAllDataHasBeenSubmittedFromApplicationThread))
		{
			// Switch the render data pointers so that
			// the data that the application just submitted becomes the data that will now be rendered
			std::swap(s_dataBeingSubmittedByApplicationThread_frame, s_dataBeingRenderedByRenderThread_frame);

			// Once the pointers have been swapped the application loop can submit new data
			if (!s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Signal())
			{
				EAE6320_ASSERTF(false, "Couldn't signal that new graphics data can be submitted");
				Logging::OutputError("Failed to signal that new render data can be submitted");
				UserOutput::Print("The renderer failed to signal to the application that new graphics data can be submitted."
					" The application is probably in a bad state and should be exited");
				return;
			}
		}
		else
		{
			EAE6320_ASSERTF(false, "Waiting for the graphics data to be submitted failed");
			Logging::OutputError("Waiting for the application loop to submit data to be rendered failed");
			UserOutput::Print("The renderer failed to wait for the application to submit data to be rendered."
				" The application is probably in a bad state and should be exited");
			return;
		}
	}

	EAE6320_ASSERT(s_dataBeingRenderedByRenderThread_frame);
	auto& constantData_frame = s_dataBeingRenderedByRenderThread_frame->constantData_frame;
	auto& constantData_normalRender = s_dataBeingRenderedByRenderThread_frame->constantData_normalRender;
	auto& constantData_debugRender = s_dataBeingRenderedByRenderThread_frame->constantData_debugRender;

	// Clear back buffer
	{
		s_view.ClearBackBuffer(s_dataBeingRenderedByRenderThread_frame->backgroundColor);
	}
	// Update the frame constant buffer
	{
		// Copy the data from the system memory that the application owns to GPU memory
		s_constantBuffer_frame.Update(&constantData_frame);
	}

	// Bind effects and draw meshes
	{
		for (int i = 0; i < s_memoryBudget; i++)
		{
			if (constantData_normalRender[i].IsValid())
			{
				s_constantBuffer_drawCall.Update(&constantData_normalRender[i].transform_localToWorld);

				constantData_normalRender[i].effect->Bind();
				constantData_normalRender[i].mesh->Draw();
			}
		}
	}

	// Drawing debug lines of colliders
	{
		Math::cMatrix_transformation transform = Math::cMatrix_transformation();

		for (int i = 0; i < s_memoryBudget; i++)
		{
			if (constantData_debugRender[i].IsValid())
			{
				s_constantBuffer_drawCall.Update(&constantData_debugRender[i].transform);

				constantData_debugRender[i].line->Draw();
			}
		}
	}

	// Swap buffer
	{
		s_view.SwapBuffer();
	}

	// After all of the data that was submitted for this frame has been used
	// you must make sure that it is all cleaned up and cleared out
	// so that the struct can be re-used (i.e. so that data for a new frame can be submitted to it)
	// We only need to clean up the mesh-effect pair data after each frame is rendered.
	// Because the draw-call data for an old mesh-effect pair will be overwirte by the data form new 
	// mesh-effect pair. Other old draw-call data won't be add to the buffer bacause its corresponding 
	// mesh-effect pair is not exist.
	{
		for (int i = 0; i < s_memoryBudget; i++)
		{
			constantData_normalRender[i].CleanUp();
			constantData_debugRender[i].CleanUp();
		}
	}

}


// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::Graphics::Initialize(const sInitializationParameters& i_initializationParameters)
{
	auto result = Results::Success;

	// Initialize the platform-specific context
	if (!(result = sContext::g_context.Initialize(i_initializationParameters)))
	{
		EAE6320_ASSERTF(false, "Can't initialize Graphics without context");
		return result;
	}
	// Initialize the platform-independent graphics objects
	{
		if (result = s_constantBuffer_frame.Initialize())
		{
			// There is only a single frame constant buffer that is reused
			// and so it can be bound at initialization time and never unbound
			s_constantBuffer_frame.Bind(
				// In our class both vertex and fragment shaders use per-frame constant data
				static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without frame constant buffer");
			return result;
		}

		if (result = s_constantBuffer_drawCall.Initialize())
		{
			s_constantBuffer_drawCall.Bind(
				static_cast<uint_fast8_t>(eShaderType::Vertex) | static_cast<uint_fast8_t>(eShaderType::Fragment));
		}
		else
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without draw call constant buffer");
			return result;
		}
	}
	// Initialize the events
	{
		if (!(result = s_whenAllDataHasBeenSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data has been submitted from the application thread");
			return result;
		}
		if (!(result = s_whenDataForANewFrameCanBeSubmittedFromApplicationThread.Initialize(Concurrency::EventType::ResetAutomaticallyAfterBeingSignaled,
			Concurrency::EventState::Signaled)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without event for when data can be submitted from the application thread");
			return result;
		}
	}
	// Initialize the mutexes
	{
		if (!(result = s_renderObjectInitializeMutex.Initialize()))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without mutex for protecting render object initialization");
			return result;
		}
		if (!(result = s_renderObjectCleanUpMutex.Initialize()))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without mutex for protecting render object clean up");
			return result;
		}
	}
	// Initialize the views
	{
		if (!(result = InitializeViews(i_initializationParameters)))
		{
			EAE6320_ASSERTF(false, "Can't initialize Graphics without the views");
			return result;
		}
	}

	return result;
}


eae6320::cResult eae6320::Graphics::CleanUp()
{
	auto result = Results::Success;

	{
		CleanUpRenderObjects();
	}

	// view clean up
	{
		result = s_view.CleanUp();
	}

	// Constant data in constant buffer clean up
	{
		// Submitted data clean up
		{
			EAE6320_ASSERT(s_dataBeingSubmittedByApplicationThread_frame);
			auto& submitted_normalRender = s_dataBeingSubmittedByApplicationThread_frame->constantData_normalRender;
			auto& submitted_debugRender = s_dataBeingSubmittedByApplicationThread_frame->constantData_debugRender;

			for (int i = 0; i < s_memoryBudget; i++)
			{
				submitted_normalRender[i].CleanUp();
				submitted_debugRender[i].CleanUp();
			}
		}
		//	Render data clean up 
		{
			EAE6320_ASSERT(s_dataBeingRenderedByRenderThread_frame);
			auto& renderData_normalRender = s_dataBeingRenderedByRenderThread_frame->constantData_normalRender;
			auto& renderData_debugRender = s_dataBeingRenderedByRenderThread_frame->constantData_debugRender;

			for (int i = 0; i < s_memoryBudget; i++)
			{
				renderData_normalRender[i].CleanUp();
				renderData_debugRender[i].CleanUp();
			}
		}

	}
	// Constant buffers clean up
	{
		const auto result_constantBuffer_frame = s_constantBuffer_frame.CleanUp();
		if (!result_constantBuffer_frame)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_frame;
			}
		}
		const auto result_constantBuffer_drawCall = s_constantBuffer_drawCall.CleanUp();
		if (!result_constantBuffer_drawCall)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_constantBuffer_drawCall;
			}
		}
	}

	{
		const auto result_context = sContext::g_context.CleanUp();
		if (!result_context)
		{
			EAE6320_ASSERT(false);
			if (result)
			{
				result = result_context;
			}
		}
	}

	return result;
}


// Helper Definitions
//===================

namespace
{

	eae6320::cResult InitializeViews(const eae6320::Graphics::sInitializationParameters& i_initializationParameters)
	{
		// view initialize
		return s_view.Initialize(i_initializationParameters);
	}
}
