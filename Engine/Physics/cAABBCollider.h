#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/sRigidBodyState.h>


namespace eae6320
{
namespace Physics
{

	class cAABBCollider : public cCollider
	{
		// Interface
		//=====================

	public:

		// Initialization / Clean Up
		//--------------------------

		cAABBCollider() = default;
		cAABBCollider(Math::sVector min, Math::sVector max) : m_min(min), m_max(max) { cCollider::Initialize(eColliderType::AABB); }

		~cAABBCollider() = default;

		void Update(const sRigidBodyState& i_rigidBody) final;

		// Property Getters
		//--------------------------

		Math::sVector GetMinExtent_world() const;

		Math::sVector GetMaxEntent_world() const;

		// Get the squared distance between i_point and this box (will be 0 if point is inside the box)
		float GetSqDistanceTo(Math::sVector i_point) const;

		// Get the closest point on or in this box to i_point
		Math::sVector GetClosestPoint(Math::sVector i_point) const;

		// Overlap Detection
		//--------------------------

		bool IsOverlaps(const cAABBCollider& i_other);


		// Data
		//=====================

	public:

		// Model space cooridnate corresponding to object's position
		Math::sVector m_min;
		Math::sVector m_max;
		
		Math::sVector m_pos;
	};

}// Namespace Physics
}// Namespace eae6320
