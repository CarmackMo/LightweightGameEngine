#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/sRigidBodyState.h>

namespace eae6320
{
namespace Physics
{

	class cSphereCollider : public cCollider
	{
		// Interface
		//=====================

	public:

		// Initialization / Clean Up
		//--------------------------

		cSphereCollider() = default;
		cSphereCollider(const Math::sVector& i_center, float i_radius, const Math::sVector& i_worldPos)
			: cCollider(eColliderType::Sphere), m_center(i_center), m_radius(i_radius), m_pos(i_worldPos) { }
		cSphereCollider(float i_x, float i_y, float i_z, float i_radius, const Math::sVector& i_worldPos)
			: cCollider(eColliderType::Sphere), m_center(Math::sVector(i_x, i_y, i_z)), m_radius(i_radius), m_pos(i_worldPos) { }

		~cSphereCollider() = default;

		void Update(const sRigidBodyState& i_rigidBody) final;

		// Property Getters
		//--------------------------

		Math::sVector GetMinExtent_world() const final;

		Math::sVector GetMaxExtent_world() const final;

		Math::sVector GetCenter_world() const final;

		Math::sVector GetCenter_local() const;

		float GetRadius() const;

		// Overlap Detection
		//--------------------------

		bool IsOverlaps(const cSphereCollider& i_other);

		bool IsOverlaps(const cAABBCollider& i_other);


		// Data
		//=====================
	
	private:

		float m_radius = 0.0f;
		Math::sVector m_center;
		Math::sVector m_pos;
	};


}// Namespace Physics
}// Namespace eae6320