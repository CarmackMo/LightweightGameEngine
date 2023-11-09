/*
	This class is your specific game
*/

#ifndef EAE6320_CMYGAME_H
#define EAE6320_CMYGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/Camera/cCamera.h>
#include <Engine/GameObject/cGameObject.h>
#include <Engine/Physics/cRigidBody.h>
#include <Engine/Results/Results.h>


#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif



// TODO: Tempory code for collider testing
#include <Engine/Graphics/cMesh.h>
#include <Engine/Graphics/cEffect.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/cSphereCollider.h>





// Class Declaration
//==================

namespace eae6320
{
	class cMyGame final : public Application::iApplication
	{
		// Inherited Implementation
		//=========================

	private:

		// Configuration
		//--------------

#if defined( EAE6320_PLATFORM_WINDOWS )
		// The main window's name will be displayed as its caption (the text that is displayed in the title bar).
		// You can make it anything that you want, but please keep the platform name and debug configuration at the end
		// so that it's easy to tell at a glance what kind of build is running.
		const char* GetMainWindowName() const final
		{
			return "MoZheng's EAE6320 MyGame"
				" -- "
#if defined( EAE6320_PLATFORM_D3D )
				"Direct3D"
#elif defined( EAE6320_PLATFORM_GL )
				"OpenGL"
#endif
#ifdef _DEBUG
				" -- Debug"
#endif
			;
		}
		// Window classes are almost always identified by name;
		// there is a unique "ATOM" associated with them,
		// but in practice Windows expects to use the class name as an identifier.
		// If you don't change the name below
		// your program could conceivably have problems if it were run at the same time on the same computer
		// as one of your classmate's.
		// You don't need to worry about this for our class,
		// but if you ever ship a real project using this code as a base you should set this to something unique
		// (a generated GUID would be fine since this string is never seen)
		const char* GetMainWindowClassName() const final { return "MoZheng's EAE6320 Main Window Class"; }
		// The following three icons are provided:
		//	* IDI_EAEGAMEPAD
		//	* IDI_EAEALIEN
		//	* IDI_VSDEFAULT_LARGE / IDI_VSDEFAULT_SMALL
		// If you want to try creating your own a convenient website that will help is: http://icoconvert.com/
		const WORD* GetLargeIconId() const final { static constexpr WORD iconId_large = IDI_EAEGAMEPAD; return &iconId_large; }
		const WORD* GetSmallIconId() const final { static constexpr WORD iconId_small = IDI_EAEGAMEPAD; return &iconId_small; }
#endif

		// Run
		//----

		void UpdateBasedOnInput() final;

		void UpdateSimulationBasedOnInput() final;

		void UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate) final;

		void SubmitDataToBeRendered(
			const float i_elapsedSecondCount_systemTime, 
			const float i_elapsedSecondCount_sinceLastSimulationUpdate) final;


		// Initialize / Clean Up
		//----------------------

		cResult Initialize() final;
		cResult CleanUp() final;


		// Data
		//=========================

	private:

		std::string m_vertexShaderPath = "data/Shaders/Vertex/standard.shader";
		std::string m_fragmentShaderPath_animate = ("data/Shaders/Fragment/moziheng.shader");
		std::string m_fragmentShaderPath_standard = ("data/Shaders/Fragment/standard.shader");


		std::string m_triangleMeshPath = "data/meshes/mesh_triangle.mesh";
		std::string m_rectangleMeshPath = "data/meshes/mesh_rectangle.mesh";
		std::string m_planeMeshPath = "data/meshes/mesh_plane.mesh";
		std::string m_cubeMeshPath = "data/meshes/mesh_cube.mesh";
		std::string m_keqingMeshPath = "data/meshes/mesh_keqing.mesh";
		std::string m_keqing_SkinMeshPath = "data/meshes/mesh_keqing_skin.mesh";
		std::string m_ganyuMeshPath = "data/meshes/mesh_ganyu.mesh";

		//std::string m_triangleMeshPath = "data/meshes/mesh_triangle.lua";
		//std::string m_rectangleMeshPath = "data/meshes/mesh_rectangle.lua";
		//std::string m_planeMeshPath = "data/meshes/mesh_plane.lua";
		//std::string m_cubeMeshPath = "data/meshes/mesh_cube.lua";
		//std::string m_keqingMeshPath = "data/meshes/mesh_keqing.lua";
		//std::string m_keqing_SkinMeshPath = "data/meshes/mesh_keqing_skin.lua";
		//std::string m_ganyuMeshPath = "data/meshes/mesh_ganyu.lua";

		Graphics::cEffect* m_effect_animate = nullptr;
		Graphics::cEffect* m_effect_standard = nullptr;

		Camera::cCamera m_camera;

		cGameObject m_gameobject_triangle;
		cGameObject m_gameobject_rectangle;
		cGameObject m_gameobject_plane;
		cGameObject m_gameobject_cube;
		cGameObject m_gameobject_Keqing;
		cGameObject m_gameobject_Keqing_skin;
		cGameObject m_gameobject_Ganyu;


		bool isKeyPressed = false;

		void InitializeMeshData();

		void InitializeCamera();

		void InitializeGameObject();

		void CleanUpGameObject();





		// TODO: temporary colldier object

		cGameObject m_colliderObject_AABB1;
		cGameObject m_colliderObject_AABB2;
		cGameObject m_colliderObject_AABB3;

		cGameObject m_colliderObject_sphere1;

		void InitializeCollisionSystem();

		bool isENTERKeyActive = false;

	};
}

// Result Definitions
//===================

namespace eae6320
{
namespace Results
{
namespace Application
{  
	// You can add specific results for your game here:
	//	* The System should always be Application
	//	* The __LINE__ macro is used to make sure that every result has a unique ID.
	//		That means, however, that all results _must_ be defined in this single file
	//		or else you could have two different ones with equal IDs.
	//	* Note that you can define multiple Success codes.
	//		This can be used if the caller may want to know more about how a function succeeded.
	constexpr cResult ExampleResult( IsFailure, eSystem::Application, __LINE__, Severity::Default );
}
}
}

#endif	// EAE6320_CEXAMPLEGAME_H
