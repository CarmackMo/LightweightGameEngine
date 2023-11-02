// Includes
//=========

#include <iostream>

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/Graphics/Graphics.h>
#include <Engine/Graphics/ConstantBufferFormats.h>
#include <Engine/Logging/Logging.h>
#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/sVector.h>
#include <Engine/UserInput/UserInput.h>


// TODO: Tempory code for collider testing
#include <Engine/UserOutput/UserOutput.h>
#include <Engine/Physics/cAABBCollider.h>
#include <iostream>



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

	m_AABBObject1.UpdateBasedOnInput();


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

	m_AABBObject1.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_AABBObject2.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);



	// TODO: Temporary code for collider debug

	Physics::cCollider* temp_0 = m_colliderObject_AABB1.GetCollider();
	if (temp_0->GetType() == Physics::eColliderType::AABB)
	{
		Physics::cAABBCollider* temp_1 = dynamic_cast<Physics::cAABBCollider*> (temp_0);
		auto temp2 = temp_1->GetMaxEntent_world();
	}

	Physics::cCollider* temp_2 = m_colliderObject_sphere1.GetCollider();
	if (temp_2->GetType() == Physics::eColliderType::Sphere)
	{
		Physics::cSphereCollider* temp_3 = dynamic_cast<Physics::cSphereCollider*> (temp_2);
		auto temp4 = temp_3->GetCenter_world();
	}


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
		constexpr uint32_t arraySize = 5;
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


	// TODO: Submit debug for box collider
	{
		constexpr uint32_t arraySize = 2;
		Graphics::ConstantBufferFormats::sDebug debugDataArray[arraySize];

		debugDataArray[0].Initialize(m_AABBObject1.GetAABBLine(), m_AABBObject1.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		debugDataArray[1].Initialize(m_AABBObject2.GetAABBLine(), m_AABBObject2.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));

		Graphics::SubmitDebugData(debugDataArray, arraySize);


		for (uint32_t i = 0; i < arraySize; i++)
		{
			debugDataArray[i].CleanUp();
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
	// Game object initialization
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




	// TODO: temporary code for initialize colldier object
	{
		m_AABBObject1.InitializeAABB(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
		m_AABBObject1.InitializeAABBLine();

		m_AABBObject2.InitializeAABB(-1, -1, -1, 1, 1, 1);
		m_AABBObject2.InitializeAABBLine();

		m_sphereObject1 = Physics::cSphereCollider(0, 0, 0, 0.4f);
		m_sphereObject2 = Physics::cSphereCollider(2.1f, 0, 0, 1);
		m_sphereObject3 = Physics::cSphereCollider(4, 0, 0, 1);

	

		Physics::sColliderSetting setting_AABB1;
		setting_AABB1.type = Physics::eColliderType::AABB;
		setting_AABB1.AABB_max = Math::sVector(2, 3, 4);
		setting_AABB1.AABB_min = Math::sVector(-5, -6, -7);

		Physics::sColliderSetting setting_sphere1;
		setting_sphere1.type = Physics::eColliderType::Sphere;
		setting_sphere1.sphere_center = Math::sVector(0, 1, 2);
		setting_sphere1.sphere_radius = 2.3f;

		m_colliderObject_AABB1.InitializeCollider(setting_AABB1);
		m_colliderObject_sphere1.InitializeCollider(setting_sphere1);
	}

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



	// TODO: temporary code for clean up colldier object
	m_AABBObject1.CleanUp();
	m_AABBObject2.CleanUp();

	m_colliderObject_AABB1.CleanUp();
	m_colliderObject_sphere1.CleanUp();
}


