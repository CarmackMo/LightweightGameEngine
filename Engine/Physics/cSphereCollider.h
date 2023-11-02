#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>

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
		cSphereCollider(Math::sVector center, float radius) : m_center(center), m_radius(radius) { cCollider::Initialize(eColliderType::Sphere); }
		cSphereCollider(float x, float y, float z, float radius) : m_center(Math::sVector(x,y,z)), m_radius(radius) { cCollider::Initialize(eColliderType::Sphere); }

		~cSphereCollider() = default;

		void Update(const sRigidBodyState& i_rigidBody) final;

		// Property Getters
		//--------------------------

		Math::sVector GetCenter_world() const;

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