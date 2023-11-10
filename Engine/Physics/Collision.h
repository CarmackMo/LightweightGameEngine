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


// Interface
//==========

namespace eae6320
{
namespace Physics
{
namespace Collision
{

	bool IsOverlaps(cCollider* i_lhs, cCollider* i_rhs);

	void Initialize(const std::vector<cCollider*>& i_allColliderList);

	void UpdateCollision();

	void UpdateCollisionResolution();

	std::vector<Graphics::cLine*>& GetBVHRenderData();

}// Namespace Collision
}// Namespace Physics
}// Namespace eae6320
