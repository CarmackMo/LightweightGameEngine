// Includes
//=========

#include <cmath>
#include "cSphereCollider.h"


void eae6320::Physics::cSphereCollider::Update(const sRigidBodyState& i_rigidBody)
{
	//m_pos = i_rigidBody.position;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetMinExtent_world() const
{
	return (m_center + m_objectRigidBody->position) - m_radius;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetMaxExtent_world() const
{
	return (m_center + m_objectRigidBody->position) + m_radius;
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
	return m_center + m_objectRigidBody->position;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetCentroid_local() const
{
	return m_center;
}


eae6320::Math::sVector eae6320::Physics::cSphereCollider::GetWorldPosition() const
{
	return m_objectRigidBody->position;
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


void eae6320::Physics::cSphereCollider::GenerateRenderData(
	uint32_t& o_vertexCount, std::vector<Math::sVector>& o_vertexData, 
	uint32_t& o_indexCount, std::vector<uint16_t>& o_indexData)
{
	// Vertex data
	o_vertexCount = 24;
	o_vertexData = std::vector<Math::sVector>(o_vertexCount);
	{
		// Top vertex to side vertices
		// 3 - 1
		o_vertexData[0] = Math::sVector(m_center.x, m_center.y + m_radius, m_center.z);
		o_vertexData[1] = Math::sVector(m_center.x - m_radius, m_center.y, m_center.z);
		// 3 - 5
		o_vertexData[2] = Math::sVector(m_center.x, m_center.y + m_radius, m_center.z);
		o_vertexData[3] = Math::sVector(m_center.x, m_center.y, m_center.z - m_radius);
		// 3 - 2
		o_vertexData[4] = Math::sVector(m_center.x, m_center.y + m_radius, m_center.z);
		o_vertexData[5] = Math::sVector(m_center.x + m_radius, m_center.y, m_center.z);
		// 3 - 6
		o_vertexData[6] = Math::sVector(m_center.x, m_center.y + m_radius, m_center.z);
		o_vertexData[7] = Math::sVector(m_center.x, m_center.y, m_center.z + m_radius);
		// Side vertices
		// 1 - 5
		o_vertexData[8] = Math::sVector(m_center.x - m_radius, m_center.y, m_center.z);
		o_vertexData[9] = Math::sVector(m_center.x, m_center.y, m_center.z - m_radius);
		// 5 - 2
		o_vertexData[10] = Math::sVector(m_center.x, m_center.y, m_center.z - m_radius);
		o_vertexData[11] = Math::sVector(m_center.x + m_radius, m_center.y, m_center.z);
		// 2 - 6
		o_vertexData[12] = Math::sVector(m_center.x + m_radius, m_center.y, m_center.z);
		o_vertexData[13] = Math::sVector(m_center.x, m_center.y, m_center.z + m_radius);
		// 6 - 1
		o_vertexData[14] = Math::sVector(m_center.x, m_center.y, m_center.z + m_radius);
		o_vertexData[15] = Math::sVector(m_center.x - m_radius, m_center.y, m_center.z);
		// Buttom vertex to side vertices
		// 4 - 1
		o_vertexData[16] = Math::sVector(m_center.x, m_center.y - m_radius, m_center.z);
		o_vertexData[17] = Math::sVector(m_center.x - m_radius, m_center.y, m_center.z);
		// 4 - 5
		o_vertexData[18] = Math::sVector(m_center.x, m_center.y - m_radius, m_center.z);
		o_vertexData[19] = Math::sVector(m_center.x, m_center.y, m_center.z - m_radius);
		// 4 - 2
		o_vertexData[20] = Math::sVector(m_center.x, m_center.y - m_radius, m_center.z);
		o_vertexData[21] = Math::sVector(m_center.x + m_radius, m_center.y, m_center.z);
		// 4 - 6
		o_vertexData[22] = Math::sVector(m_center.x, m_center.y - m_radius, m_center.z);
		o_vertexData[23] = Math::sVector(m_center.x, m_center.y, m_center.z + m_radius);
	}

	// Index data
	o_indexCount = o_vertexCount;
	o_indexData = std::vector<uint16_t>(o_indexCount);
	for (uint32_t i = 0; i < o_indexCount; i++)
	{
		o_indexData[i] = i;
	}
}


