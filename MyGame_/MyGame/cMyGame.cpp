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
#include <Engine/Physics/Collision.h>
#include <vector>
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


	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter))
	{
		isKeyPressed = true;
	}
	else
	{
		isKeyPressed = false;
	}


	// TODO: Temporary code for collider debug
	m_colliderObject_AABB1.UpdateBasedOnInput();
	//m_colliderObject_sphere2.UpdateBasedOnInput();

	//if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter))
	//{
	//	isENTERKeyActive = !isENTERKeyActive;
	//	UserOutput::ConsolePrint("Current key state: ", isENTERKeyActive ? "TRUE" : "FALSE");


	//	if (isENTERKeyActive)
	//	{
	//		Physics::Collision::UpdateRenderData();
	//	}
	//}

}


void eae6320::cMyGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	m_gameobject_triangle.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_gameobject_Ganyu.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_gameobject_Keqing.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);



	// TODO: Temporary code for collider debug
	m_colliderObject_AABB1.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_colliderObject_AABB2.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);
	m_colliderObject_sphere2.UpdateBasedOnTime(i_elapsedSecondCount_sinceLastUpdate);

	Physics::Collision::Update_CollisionDetection();

	Physics::Collision::Update_CollisionResolution();

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
		auto BVHRenderData = Physics::Collision::GetBVHRenderData();
		uint32_t staticSize = 6;
		uint32_t arraySize = BVHRenderData.size() + staticSize;

		Graphics::ConstantBufferFormats::sDebug* debugDataArray = new Graphics::ConstantBufferFormats::sDebug[arraySize];

		// Render data of hard-coded collider
		debugDataArray[0].Initialize(m_colliderObject_AABB1.GetColliderLine(), m_colliderObject_AABB1.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		debugDataArray[1].Initialize(m_colliderObject_AABB2.GetColliderLine(), m_colliderObject_AABB2.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		debugDataArray[2].Initialize(m_colliderObject_AABB3.GetColliderLine(), m_colliderObject_AABB3.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		debugDataArray[3].Initialize(m_colliderObject_AABB4.GetColliderLine(), m_colliderObject_AABB4.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		debugDataArray[4].Initialize(m_colliderObject_sphere1.GetColliderLine(), m_colliderObject_sphere1.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
		debugDataArray[5].Initialize(m_colliderObject_sphere2.GetColliderLine(), m_colliderObject_sphere2.GetPredictedTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));

		// Render data of BVH tree
		for (uint32_t i = staticSize; i < arraySize; i++)
		{
			debugDataArray[i].Initialize(BVHRenderData[i - staticSize].first, *BVHRenderData[i - staticSize].second);
		}

		Graphics::SubmitDebugData(debugDataArray, arraySize);

		// Clean up
		for (uint32_t i = 0; i < arraySize; i++)
		{
			debugDataArray[i].CleanUp();
		}

		delete[] debugDataArray;
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

	// TODO: temporary code for initialize collision system
	InitializeCollisionSystem();


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
		Physics::sColliderSetting setting_AABB1;
		setting_AABB1.SettingForAABB(Math::sVector(-0.5, -0.5, -0.5), Math::sVector(0.5, 0.5, 0.5));

		Physics::sColliderSetting setting_sphere1;
		setting_sphere1.SettingForSphere(Math::sVector(0, 0, 0), 0.5f);




		m_colliderObject_AABB1.GetRigidBody().position = Math::sVector(0.5f, 0.0f, 0.5f);
		m_colliderObject_AABB1.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB1.InitializeColliderLine();
		m_colliderObject_AABB1.GetCollider()->m_name = "AABB_1";
		m_colliderObject_AABB1.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB1.SetIsCollide(true); };
		m_colliderObject_AABB1.GetCollider()->OnCollisionStay = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : " ).c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB1.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB1.SetIsCollide(false); };

		// TODO: Trying to allocte OpenGL buffer during runtime
		//m_colliderObject_AABB1.GetCollider()->OnCollisionEnter = 
		//	[](const Physics::cCollider* other) -> void 
		//	{ 
		//		UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); 

		//		Graphics::VertexFormats::sVertex_line vertexData[1];
		//		uint16_t indexData[1];
		//		Graphics::cLine* temp;
		//		Graphics::cLine::Create(temp, vertexData, 1, indexData, 1);

		//		temp->DecrementReferenceCount();
		//		return; 
		//	};
		//m_colliderObject_AABB1.GetCollider()->OnCollisionExit = 
		//	[](const Physics::cCollider* other) -> void 
		//	{ 
		//		UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); 
		//		return; 
		//	};


		m_colliderObject_AABB2.GetRigidBody().position = Math::sVector(-1.0f, -0.5f, 1.5f);
		m_colliderObject_AABB2.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB2.InitializeColliderLine();
		m_colliderObject_AABB2.GetCollider()->m_name = "AABB_2";
		m_colliderObject_AABB2.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB2.SetIsCollide(true); };
		m_colliderObject_AABB2.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB2.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB2.SetIsCollide(false); };

		m_colliderObject_AABB3.GetRigidBody().position = Math::sVector(1.0f, -2.0f, 3.0f);
		m_colliderObject_AABB3.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB3.InitializeColliderLine();
		m_colliderObject_AABB3.GetCollider()->m_name = "AABB_3";
		m_colliderObject_AABB3.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB3.SetIsCollide(true); };
		m_colliderObject_AABB3.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB3.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB3.SetIsCollide(false); };

		m_colliderObject_AABB4.GetRigidBody().position = Math::sVector(-1.5f, -2.0f, 1.0f);
		m_colliderObject_AABB4.GetRigidBody().isStatic = true;
		m_colliderObject_AABB4.InitializeCollider(setting_AABB1);
		m_colliderObject_AABB4.InitializeColliderLine();
		m_colliderObject_AABB4.GetCollider()->m_name = "AABB_4";
		m_colliderObject_AABB4.GetCollider()->OnCollisionEnter =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_AABB4.SetIsCollide(true); };
		m_colliderObject_AABB4.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_AABB4.GetCollider()->OnCollisionExit =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_AABB4.SetIsCollide(false); };


		m_colliderObject_sphere1.GetRigidBody().position = Math::sVector(2.0f, -1.0f, 3.0f);
		m_colliderObject_sphere1.InitializeCollider(setting_sphere1);
		m_colliderObject_sphere1.InitializeColliderLine();
		m_colliderObject_sphere1.GetCollider()->m_name = "Sphere_1";
		m_colliderObject_sphere1.GetCollider()->OnCollisionEnter = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_sphere1.SetIsCollide(true); };
		m_colliderObject_sphere1.GetCollider()->OnCollisionStay = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_sphere1.GetCollider()->OnCollisionExit = 
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_sphere1.SetIsCollide(false); };
	
		m_colliderObject_sphere2.GetRigidBody().position = Math::sVector(2.0f, 1.0f, 1.0f);
		m_colliderObject_sphere2.InitializeCollider(setting_sphere1);
		m_colliderObject_sphere2.InitializeColliderLine();
		m_colliderObject_sphere2.GetCollider()->m_name = "Sphere_2";
		m_colliderObject_sphere2.GetCollider()->OnCollisionEnter =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Enter collision, other: ", other->m_name.c_str()); m_colliderObject_sphere2.SetIsCollide(true); };
		m_colliderObject_sphere2.GetCollider()->OnCollisionStay =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(std::string("Stay collision, " + self->m_name + " : ").c_str(), other->m_name.c_str()); };
		m_colliderObject_sphere2.GetCollider()->OnCollisionExit =
			[this](Physics::cCollider* self, Physics::cCollider* other) -> void { UserOutput::ConsolePrint(" Exit collision, other: ", other->m_name.c_str()); m_colliderObject_sphere2.SetIsCollide(false); };


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

	m_colliderObject_AABB1.CleanUp();
	m_colliderObject_AABB2.CleanUp();
	m_colliderObject_AABB3.CleanUp();
	m_colliderObject_AABB4.CleanUp();
	m_colliderObject_sphere1.CleanUp();
	m_colliderObject_sphere2.CleanUp();
}



void eae6320::cMyGame::InitializeCollisionSystem()
{
	std::vector<Physics::cCollider*> colliderList;
	colliderList.push_back(m_colliderObject_AABB1.GetCollider());
	colliderList.push_back(m_colliderObject_AABB2.GetCollider());
	colliderList.push_back(m_colliderObject_AABB3.GetCollider());
	colliderList.push_back(m_colliderObject_AABB4.GetCollider());
	colliderList.push_back(m_colliderObject_sphere1.GetCollider());
	colliderList.push_back(m_colliderObject_sphere2.GetCollider());


	Physics::Collision::Initialize(colliderList, Physics::Collision::BroadPhase_BVH | Physics::Collision::NarrowPhase_Overlaps);
}


 