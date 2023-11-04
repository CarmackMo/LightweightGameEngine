// Includes
//=========

#include "cAABBCollider.h"


void eae6320::Physics::cAABBCollider::Update(const sRigidBodyState& i_rigidBody)
{
	m_pos = i_rigidBody.position;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetMinExtent_world() const
{
	return m_min + m_pos;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetMaxExtent_world() const
{
	return m_max + m_pos;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetCenter_world() const
{
	return 0.5f * (m_min + m_max) + m_pos;
}


float eae6320::Physics::cAABBCollider::GetSqDistanceTo(Math::sVector i_point) const
{
	Math::sVector closestPoint = GetClosestPoint(i_point);

	return Math::SqDistance(closestPoint, i_point);
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetClosestPoint(Math::sVector i_point) const
{
	return  Math::Min(Math::Max(i_point, GetMinExtent_world()), GetMaxExtent_world());
}


bool eae6320::Physics::cAABBCollider::IsOverlaps(const cAABBCollider& i_other)
{
	// Two AABB are impossible to overlap if there exist any gap in any axis
	Math::sVector distance_inner = GetMinExtent_world() - i_other.GetMaxExtent_world();
	Math::sVector distance_outer = i_other.GetMinExtent_world() - GetMaxExtent_world();
	
	Math::sVector distance = Math::Max(distance_inner, distance_outer);

	return distance.x <= 0 && distance.y <= 0 && distance.z <= 0;
}
