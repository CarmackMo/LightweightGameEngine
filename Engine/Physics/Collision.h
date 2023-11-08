#pragma once

// Includes
//=========

#include <Engine/Physics/cColliderBase.h>
#include <Engine/Results/Results.h>


#include <unordered_map>
#include <vector>

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







	void Initialize_SweepAndPrune(const std::vector<eae6320::Physics::cCollider*>& i_allColliderList);

	void AddCollider_SweepAndPrune(cCollider* i_collider);

	cResult RemoveCollider_SweepAndPrune(cCollider* i_collider);

	void CollisionDetection_BroadPhase_SweepAndPrune();

	void CollisionDetection_NarrowPhase_Overlap(std::unordered_map<cCollider*, std::vector<cCollider*>>& i_CollisionMap_broadPhase);

	void InvokeCollisionCallback(std::unordered_map<cCollider*, std::vector<cCollider*>>& i_newCollisionMap);

}// Namespace Collision
}// Namespace Physics
}// Namespace eae6320
