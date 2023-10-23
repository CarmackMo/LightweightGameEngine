// Includes
//=========

#include <iostream>

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Graphics/ConstantBufferFormats.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/sVector.h>
#include <Engine/UserInput/UserInput.h>


// Inherited Implementation
//=========================

// Run
//----

void eae6320::cMyGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}


void eae6320::cMyGame::UpdateSimulationBasedOnInput()
{
	m_camera.UpdateBasedOnInput();

	m_gameobject_triangle.UpdateBasedOnInput();

	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter))
	{
		isKeyPressed = true;
	}
	else
	{
		isKeyPressed = false;
	}
}


void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	m_gameobject_triangle.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_gameobject_Ganyu.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_gameobject_Keqing.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
}


void eae6320::cMyGame::SubmitDataToBeRendered(
	const float i_elapsedSecondCount_systemTime, 
	const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	// Submit camera data
	{
		// aspectRatio = width / height
		uint16_t width, height;
		GetCurrentResolution(width, height);

		Graphics::SubmitCameraMatrices(
			m_camera.CreateWorldToCameraMatrix(i_elapsedSecondCount_sinceLastSimulationUpdate),
			m_camera.CreateCameraToProjectedMatrix(static_cast<float>(width), static_cast<float>(height)));
	}

	// Submit background color
	{
		Graphics::SubmitBackgroundColor(0.5f, 0.5f, 0.5f);
	}

	// Submit mesh-effect pair data
	{
		constexpr uint16_t arraySize = 5;
		Graphics::ConstantBufferFormats::sMeshEffectPair meshEffectPairArray[arraySize];
		Math::cMatrix_transformation transformMatrixArray[arraySize];

		// Triangle
		if (!isKeyPressed)
			meshEffectPairArray[0].Initialize(m_gameobject_triangle.GetMesh(), m_gameobject_triangle.GetEffect());
		else
			meshEffectPairArray[0].Initialize(m_gameobject_rectangle.GetMesh(), m_effect_animate);
		transformMatrixArray[0] = m_gameobject_triangle.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);

		// Plane
		meshEffectPairArray[1].Initialize(m_gameobject_plane.GetMesh(), m_gameobject_plane.GetEffect());
		transformMatrixArray[1] = m_gameobject_plane.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);

		// Cube
		meshEffectPairArray[2].Initialize(m_gameobject_cube.GetMesh(), m_gameobject_cube.GetEffect());
		transformMatrixArray[2] = m_gameobject_cube.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);

		// Ganyu
		meshEffectPairArray[3].Initialize(m_gameobject_Ganyu.GetMesh(), m_gameobject_Ganyu.GetEffect());
		transformMatrixArray[3] = m_gameobject_Ganyu.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);

		// Keqing
		meshEffectPairArray[4].Initialize(m_gameobject_Keqing.GetMesh(), m_gameobject_Keqing.GetEffect());
		transformMatrixArray[4] = m_gameobject_Keqing.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);


		Graphics::SubmitMeshEffectData(meshEffectPairArray, transformMatrixArray, arraySize);

		for (int i = 0; i < arraySize; i++)
		{
			meshEffectPairArray[i].CleanUp();
		}
	}
}


// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cMyGame::Initialize()
{
	eae6320::Logging::OutputMessage("\n --------------MoZiheng MyGame customized log messge--------------");

#if defined( EAE6320_PLATFORM_D3D )
	eae6320::Logging::OutputMessage("Solution Platform: Direct3D");
#elif defined( EAE6320_PLATFORM_GL )
	eae6320::Logging::OutputMessage("Solution Platform: OpenGL");
#endif

#ifdef _WIN64
	eae6320::Logging::OutputMessage("Machine Platform: x64");
#else
	eae6320::Logging::OutputMessage("Machine Platform: x86");
#endif

#ifdef _DEBUG
	eae6320::Logging::OutputMessage("Configuration: Debug");
#else
	eae6320::Logging::OutputMessage("Configuration: Release");
#endif

	eae6320::Logging::OutputMessage("Initialization Successfull!! \n");


	InitializeCamera();

	InitializeMeshData();

	InitializeGameObject();

	return Results::Success;
}


eae6320::cResult eae6320::cMyGame::CleanUp()
{
	eae6320::Logging::OutputMessage("\n --------------MoZiheng MyGame customized log messge--------------");

#if defined( EAE6320_PLATFORM_D3D )
	eae6320::Logging::OutputMessage("Solution Platform: Direct3D");
#elif defined( EAE6320_PLATFORM_GL )
	eae6320::Logging::OutputMessage("Solution Platform: OpenGL");
#endif

#ifdef _WIN64
	eae6320::Logging::OutputMessage("Machine Platform: x64");
#else
	eae6320::Logging::OutputMessage("Machine Platform: x86");
#endif

#ifdef _DEBUG
	eae6320::Logging::OutputMessage("Configuration: Debug");
#else
	eae6320::Logging::OutputMessage("Configuration: Release");
#endif

	eae6320::Logging::OutputMessage("Clan Up Successfull!!\n");

	CleanUpGameObject();

	return Results::Success;
}


void eae6320::cMyGame::InitializeMeshData()
{
	Graphics::cEffect::Create(m_effect_animate, m_vertexShaderPath, m_fragmentShaderPath_animate);

	Graphics::cEffect::Create(m_effect_standard, m_vertexShaderPath, m_fragmentShaderPath_standard);
}


void eae6320::cMyGame::InitializeCamera()
{
	m_camera.Initialize(
		0.0f, 0.0f, 5.0f,
		Math::ConvertDegreesToRadians(45.0f), 
		0.1f, 
		100.0f);

	//m_camera.InitializeMesh(m_rectangleMeshPath);
	//m_camera.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_animate);
}


void eae6320::cMyGame::InitializeGameObject()
{
	// Tirangle
	m_gameobject_triangle.InitializeMesh(m_triangleMeshPath);
	m_gameobject_triangle.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_animate);

	m_gameobject_triangle.GetRigidBody().angularSpeed = 1.0f;
	m_gameobject_triangle.GetRigidBody().angularVelocity_axis_local = Math::sVector(0.0f, 0.0f, 1.0f);

	// Rectangle
	m_gameobject_rectangle.InitializeMesh(m_rectangleMeshPath);
	m_gameobject_rectangle.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);

	// Plane
	m_gameobject_plane.InitializeMesh(m_planeMeshPath);
	m_gameobject_plane.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
	m_gameobject_plane.GetRigidBody().position = Math::sVector(+2.0f, -2.0f, -3.0f);

	// Cube
	m_gameobject_cube.InitializeMesh(m_cubeMeshPath);
	m_gameobject_cube.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
	m_gameobject_cube.GetRigidBody().position = Math::sVector(-2.0f, -2.0f, -3.0f);

	// Keqing
	m_gameobject_Keqing.InitializeMesh(m_keqingMeshPath);
	m_gameobject_Keqing.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
	m_gameobject_Keqing.GetRigidBody().position = Math::sVector(-5.0f, -10.0f, -20.0f);
	m_gameobject_Keqing.GetRigidBody().angularSpeed = 0.5f;

	// Keqing - skin
	m_gameobject_Keqing_skin.InitializeMesh(m_keqing_SkinMeshPath);
	m_gameobject_Keqing_skin.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
	m_gameobject_Keqing_skin.GetRigidBody().position = Math::sVector(-5.0f, -10.0f, -20.0f);
	m_gameobject_Keqing_skin.GetRigidBody().angularSpeed = 0.5f;

	// Ganyu
	m_gameobject_Ganyu.InitializeMesh(m_ganyuMeshPath);
	m_gameobject_Ganyu.InitializeEffect(m_vertexShaderPath, m_fragmentShaderPath_standard);
	m_gameobject_Ganyu.GetRigidBody().position = Math::sVector(5.0f, -10.0f, -20.0f);
	m_gameobject_Ganyu.GetRigidBody().angularSpeed = 0.5f;
}


void eae6320::cMyGame::CleanUpGameObject()
{
	m_gameobject_triangle.CleanUp();
	m_gameobject_rectangle.CleanUp();
	m_gameobject_plane.CleanUp();
	m_gameobject_cube.CleanUp();
	m_gameobject_Keqing.CleanUp();
	m_gameobject_Keqing_skin.CleanUp();
	m_gameobject_Ganyu.CleanUp();

	m_camera.CleanUp();


	m_effect_animate->DecrementReferenceCount();
	m_effect_standard->DecrementReferenceCount();
}


