#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cColliderBase.h>

#include <vector>


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

		cAABBCollider() : cCollider(eColliderType::AABB) { };
		cAABBCollider(const Math::sVector& i_min, const Math::sVector& i_max)
			: cCollider(eColliderType::AABB), m_min(i_min), m_max(i_max) { }

		~cAABBCollider() = default;


		// Property Getters
		//--------------------------

		Math::sVector GetMinExtent_world() const final;

		Math::sVector GetMaxExtent_world() const final;

		Math::sVector GetMinExtent_local() const final;

		Math::sVector GetMaxExtent_local() const final;

		Math::sVector GetCentroid_world() const final;

		Math::sVector GetCentroid_local() const final;

		Math::sVector GetWorldPosition() const final;

		/* Get volume of bounding box */
		float GetVolume() const;

		/* Get the squared distance between i_point and this box (will be 0 if point is inside the box) */ 
		float GetSqDistanceTo(Math::sVector i_point) const;

		/* Get the closest point on or in this box to i_point */ 
		Math::sVector GetClosestPoint(Math::sVector i_point) const;

		// Operation
		//--------------------------

		bool IsOverlaps(const cAABBCollider& i_other) const;

		bool IsContains(const cCollider& i_other) const;

		/* Return a AABB whose min/max extent is large enough to contain the world extent of self and i_other */
		cAABBCollider Union(const cAABBCollider& i_other) const;

		// Render / Debug
		//--------------------------

		void GenerateRenderData(
			uint32_t& o_vertexCount, std::vector<Math::sVector>& o_vertexData,
			uint32_t& o_indexCount, std::vector<uint16_t>& o_indexData) final;


		// Data
		//=====================

	public:

		// Model space cooridnate corresponding to owner object's position
		Math::sVector m_min;
		Math::sVector m_max;
		
		// World position of the AABB, demanded by BVH tree branch node (it doesn't has rigidBody)
		Math::sVector m_pos;
	};

}// Namespace Physics
}// Namespace eae6320
