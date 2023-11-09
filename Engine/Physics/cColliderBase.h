#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cRigidBody.h>
#include <Engine/Results/Results.h>

#include <cstdint>
#include <functional>
#include <vector>

// TODO: temporary code for initialize colldier object
#include <string>
#include <memory>


// Forward Declarations
//=====================

namespace eae6320
{
namespace Physics
{
	class cSphereCollider;

	class cAABBCollider;
}
}


// Collider Types
//=============

namespace eae6320
{
namespace Physics
{
	enum class eColliderType : uint8_t
	{
		None	= 0,
		Sphere	= 1,
		AABB	= 2,
		Plane	= 3,
	};
}
}


// Collider Builder
//=============

namespace eae6320
{
namespace Physics
{
	struct sColliderSetting
	{
		eColliderType type = eColliderType::None;

		// Data for sphere collider
		Math::sVector sphere_center;
		float sphere_radius = 0.0f;

		// Data for AABB collider
		Math::sVector AABB_min;
		Math::sVector AABB_max;


		void SettingForAABB(Math::sVector i_min, Math::sVector i_max);
		void SettingForSphere(Math::sVector i_center, float i_radius);
	};
}
}


// Class Declaration
//==================

namespace eae6320
{
namespace Physics
{

	class cCollider
	{
		// Interface
		//=====================

	public:

		// Initialization / Clean Up
		//--------------------------

		static cResult Create(cCollider*& o_collider, const sColliderSetting& i_setting, const sRigidBodyState& i_rigidBody);

		static cResult Create(cCollider*& o_collider, const sColliderSetting& i_setting, sRigidBodyState* i_rigidBody);

		// Property Getters
		//--------------------------

		eColliderType GetType() const;

		virtual Math::sVector GetMinExtent_world() const = 0;

		virtual Math::sVector GetMaxExtent_world() const = 0;

		virtual Math::sVector GetMinExtent_local() const = 0;

		virtual Math::sVector GetMaxExtent_local() const = 0;

		virtual Math::sVector GetCentroid_world() const = 0;

		virtual Math::sVector GetCentroid_local() const = 0;

		virtual Math::sVector GetWorldPosition() const = 0;

		// Update
		//--------------------------

		virtual void Update(const sRigidBodyState& i_rigidBody) = 0;

		// Render / Debug
		//--------------------------

		virtual void GenerateRenderData(
			uint32_t& o_vertexCount, std::vector<Math::sVector>& o_vertexData,
			uint32_t& o_indexCount, std::vector<uint16_t>& o_indexData) = 0;


	protected:

		cCollider() = default;
		cCollider(eColliderType i_type) : m_type(i_type) {};


		// Data
		//=====================

	protected:

		eColliderType m_type = eColliderType::None;


	public:

		sRigidBodyState* m_objectRigidBody = nullptr;

		std::function<void(const cCollider*, const cCollider*)> OnCollisionEnter = nullptr;
		std::function<void(const cCollider*, const cCollider*)> OnCollisionStay = nullptr;
		std::function<void(const cCollider*, const cCollider*)> OnCollisionExit = nullptr;


		// TODO: temporary code for initialize colldier object
	public:
		std::string m_name = "";

	};


}// Namespace Physics
}// Namespace eae6320