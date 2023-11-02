#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/sRigidBodyState.h>
#include <Engine/Results/Results.h>
#include <cstdint>


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


// Collider Types
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

		static cResult Create(cCollider*& o_collider, const sColliderSetting& i_setting);

		void Initialize(eColliderType i_type);

		// Property Getters
		//--------------------------

		eColliderType GetType() const;

		// Update
		//--------------------------

		virtual void Update(const sRigidBodyState& i_rigidBody) = 0;



		// Data
		//=====================

	private:

		eColliderType m_type;

	};

}// Namespace Physics
}// Namespace eae6320