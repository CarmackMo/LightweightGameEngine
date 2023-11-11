#pragma once

// Includes
//=========

#include <Engine/Physics/cBVHTree.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Results/Results.h>


#include <unordered_map>
#include <vector>

// TODO: Rendering support for BVH tree
#include <Engine/Graphics/cLine.h>


// Collision Type Bits
//==================

namespace eae6320
{
namespace Physics
{
namespace Collision
{

	// The values below are used as bit masks
	enum eCollisionType : uint8_t
	{
		BroadPhase_SweepAndPrune	= 1 << 0,

		BroadPhase_BVH				= 1 << 1,

		NarrowPhase_Overlaps		= 1 << 2,
	};

}// Namespace Collision
}// Namespace Physics
}// Namespace eae6320


// Interface
//==========

namespace eae6320
{
namespace Physics
{
namespace Collision
{

	bool IsOverlaps(cCollider* i_lhs, cCollider* i_rhs);

	void Initialize(const std::vector<cCollider*>& i_allColliderList, uint8_t i_collisionType);

	void Update_CollisionDetection();

	void Update_CollisionResolution();

	void RegisterCollider(cCollider* i_collider);

	cResult DeregisterCollider(cCollider* i_collider);

	std::vector<std::pair<Graphics::cLine*, Math::cMatrix_transformation*>>& GetBVHRenderData();


}// Namespace Collision
}// Namespace Physics
}// Namespace eae6320
