/*
	This class is your specific game
*/

#ifndef EAE6320_CMYGAME_H
#define EAE6320_CMYGAME_H

// Includes
//=========

#include <Engine/Application/iApplication.h>
#include <Engine/GameObject/cCamera.h>
#include <Engine/GameObject/cGameObject.h>
#include <Engine/Physics/cRigidBody.h>
#include <Engine/Results/Results.h>
#include <Engine/Utilities/cSingleton.h>

#include <ScrollShooterGame_/ScrollShooterGame/Bullet/cBullet.h>
#include <ScrollShooterGame_/ScrollShooterGame/Enemy/cEnemyGenerator.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPhysicsDebugObject.h>
#include <ScrollShooterGame_/ScrollShooterGame/cPlayer.h>

#include <memory>
#include <vector>
#include <queue>


#if defined( EAE6320_PLATFORM_WINDOWS )
	#include "Resource Files/Resource.h"
#endif




// Class Declaration
//==================

namespace ScrollShooterGame
{
	class cScrollShooterGame final : public  eae6320::Application::iApplication, public eae6320::cSingleton<cScrollShooterGame>
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
			return "MoZheng's ScrollShooterGame"
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

		void RuntimeCleanUp() final;



		// Initialize / Clean Up
		//----------------------

		eae6320::cResult Initialize() final;
		eae6320::cResult CleanUp() final;


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


		eae6320::Camera::cCamera m_camera;


		void InitializeCamera();

		void InitializeGameObject();

		void CleanUpGameObject();

		void InitializeCollisionSystem();




	public:

		// TODO: temporary player object
		cPlayer* m_player = nullptr;
		cEnemyGenerator* m_enemyGenerator = nullptr;


		std::vector<cBullet*> m_bulletList;

		//std::vector<eae6320::cGameObject*> m_gameObjectList;

		//std::queue<eae6320::cGameObject*> m_gameObjectCleanUpQueue;


		std::vector<std::shared_ptr<eae6320::cGameObject>> m_gameObjectList_sp;

		std::queue<std::shared_ptr<eae6320::cGameObject>> m_gameObjectCleanUpQueue_sp;





	public:

		//void AddGameObjectCleanUpTask(eae6320::cGameObject* i_gameObject);

		void AddGameObjectCleanUpTask(std::shared_ptr<eae6320::cGameObject> i_gameObject);
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
