// Includes
//=========

#include <cmath>
#include "cSphereCollider.h"


void eae6320::Physics::cSphereCollider::Update(const sRigidBodyState& i_rigidBody)
{
	m_pos = i_rigidBody.position;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetMinExtent_world() const
{
	return (m_center + m_pos) - m_radius;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetMaxExtent_world() const
{
	return (m_center + m_pos) + m_radius;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetMinExtent_local() const
{
	return m_center - m_radius;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetMaxExtent_local() const
{
	return m_center + m_radius;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetCentroid_world() const
{
	return m_center + m_pos;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetCentroid_local() const
{
	return m_center;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetWorldPosition() const
{
	return m_pos;
}


float eae6320::Physics::cSphereCollider::GetRadius() const
{
	return m_radius;
}


bool eae6320::Physics::cSphereCollider::IsOverlaps(const cSphereCollider& i_other)
{
	float centerSqDistance = Math::SqDistance(GetCentroid_world(), i_other.GetCentroid_world());
	float radiusSqDistance = powf(m_radius + i_other.m_radius, 2);

	return centerSqDistance <= radiusSqDistance;
}


bool eae6320::Physics::cSphereCollider::IsOverlaps(const cAABBCollider& i_other)
{
	return i_other.GetSqDistanceTo(GetCentroid_world()) <= powf(m_radius, 2);
}

