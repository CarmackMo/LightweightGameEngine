// Includes
//=========

#include "cAABBCollider.h"


void eae6320::Physics::cAABBCollider::Update(const sRigidBodyState& i_rigidBody)
{
	m_pos = i_rigidBody.position;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetMinExtent_world() const
{
	if (m_objectRigidBody != nullptr)
		return m_min + m_objectRigidBody->position;
	else
		return m_min + m_pos;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetMaxExtent_world() const
{
	if (m_objectRigidBody != nullptr)
		return m_max + m_objectRigidBody->position;
	else
		return m_max + m_pos;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetMinExtent_local() const
{
	return m_min;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetMaxExtent_local() const
{
	return m_max;
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetCentroid_world() const
{
	return 0.5f * (GetMinExtent_world() + GetMaxExtent_world());
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetCentroid_local() const
{
	return 0.5f * (m_min + m_max);
}


eae6320::Math::sVector eae6320::Physics::cAABBCollider::GetWorldPosition() const
{
	if (m_objectRigidBody != nullptr)
		return m_objectRigidBody->position;
	else
		return m_pos;
}


float eae6320::Physics::cAABBCollider::GetVolume() const
{
	Math::sVector size = m_max - m_min;
	return size.x * size.y * size.z;
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


bool eae6320::Physics::cAABBCollider::IsOverlaps(const cAABBCollider& i_other) const
{
	// Two AABB are impossible to overlap if there exist any gap in any axis
	Math::sVector distance_inner = GetMinExtent_world() - i_other.GetMaxExtent_world();
	Math::sVector distance_outer = i_other.GetMinExtent_world() - GetMaxExtent_world();
	
	Math::sVector distance = Math::Max(distance_inner, distance_outer);

	return distance.x <= 0 && distance.y <= 0 && distance.z <= 0;
}


bool eae6320::Physics::cAABBCollider::IsContains(const cCollider& i_other) const
{
	Math::sVector selfMinExtent = GetMinExtent_world();
	Math::sVector selfMaxExtent = GetMaxExtent_world();
	Math::sVector otherMinExtent = i_other.GetMinExtent_world();
	Math::sVector otherMaxExtent = i_other.GetMaxExtent_world();

	return (selfMinExtent.x <= otherMinExtent.x && selfMinExtent.y <= otherMinExtent.y && selfMinExtent.z <= otherMinExtent.z) &&
		   (selfMaxExtent.x <= otherMaxExtent.x && selfMaxExtent.y <= otherMaxExtent.y && selfMaxExtent.z <= otherMaxExtent.z);
}


eae6320::Physics::cAABBCollider eae6320::Physics::cAABBCollider::Union(const cAABBCollider& i_other) const
{
	Math::sVector minExtent = Math::Min(GetMinExtent_world(), i_other.GetMinExtent_world());
	Math::sVector maxExtent = Math::Max(GetMaxExtent_world(), i_other.GetMaxExtent_world());
	
	//Math::sVector center = 0.5f * (minExtent + maxExtent);
	Math::sVector center = Math::sVector();

	return cAABBCollider(minExtent, maxExtent, center);
}


void eae6320::Physics::cAABBCollider::GenerateRenderData(
	uint32_t& o_vertexCount, std::vector<Math::sVector>& o_vertexData, 
	uint32_t& o_indexCount, std::vector<uint16_t>& o_indexData)
{
	// Vertex data
	o_vertexCount = 24;
	o_vertexData = std::vector<Math::sVector>(o_vertexCount);
	{
		// 0-1
		o_vertexData[0] = Math::sVector(m_min.x, m_min.y, m_min.z);
		o_vertexData[1] = Math::sVector(m_min.x, m_max.y, m_min.z);
		// 1-2
		o_vertexData[2] = Math::sVector(m_min.x, m_max.y, m_min.z);
		o_vertexData[3] = Math::sVector(m_min.x, m_max.y, m_max.z);
		// 2-3
		o_vertexData[4] = Math::sVector(m_min.x, m_max.y, m_max.z);
		o_vertexData[5] = Math::sVector(m_min.x, m_min.y, m_max.z);
		// 3-0
		o_vertexData[6] = Math::sVector(m_min.x, m_min.y, m_max.z);
		o_vertexData[7] = Math::sVector(m_min.x, m_min.y, m_min.z);
		// 4-5
		o_vertexData[8] = Math::sVector(m_max.x, m_min.y, m_min.z);
		o_vertexData[9] = Math::sVector(m_max.x, m_max.y, m_min.z);
		// 5-6
		o_vertexData[10] = Math::sVector(m_max.x, m_max.y, m_min.z);
		o_vertexData[11] = Math::sVector(m_max.x, m_max.y, m_max.z);
		// 6-7
		o_vertexData[12] = Math::sVector(m_max.x, m_max.y, m_max.z);
		o_vertexData[13] = Math::sVector(m_max.x, m_min.y, m_max.z);
		// 7-4
		o_vertexData[14] = Math::sVector(m_max.x, m_min.y, m_max.z);
		o_vertexData[15] = Math::sVector(m_max.x, m_min.y, m_min.z);
		// 1-5
		o_vertexData[16] = Math::sVector(m_min.x, m_max.y, m_min.z);
		o_vertexData[17] = Math::sVector(m_max.x, m_max.y, m_min.z);
		// 2-6
		o_vertexData[18] = Math::sVector(m_min.x, m_max.y, m_max.z);
		o_vertexData[19] = Math::sVector(m_max.x, m_max.y, m_max.z);
		// 0-4
		o_vertexData[20] = Math::sVector(m_min.x, m_min.y, m_min.z);
		o_vertexData[21] = Math::sVector(m_max.x, m_min.y, m_min.z);
		// 3-7
		o_vertexData[22] = Math::sVector(m_min.x, m_min.y, m_max.z);
		o_vertexData[23] = Math::sVector(m_max.x, m_min.y, m_max.z);
	}

	// Index data
	o_indexCount = o_vertexCount;
	o_indexData = std::vector<uint16_t>(o_indexCount);
	for (uint32_t i = 0; i < o_indexCount; i++)
	{
		o_indexData[i] = i;
	}
}


