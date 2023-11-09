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




	// Broad Phase: Sweep and Prune
	//----------------------

	void Initialize_SweepAndPrune(const std::vector<cCollider*>& i_allColliderList);

	void AddCollider_SweepAndPrune(cCollider* i_collider);

	cResult RemoveCollider_SweepAndPrune(cCollider* i_collider);

	void CollisionDetection_BroadPhase_SweepAndPrune();


	// Broad Phase: BVH Tree
	//----------------------
	
	void Initialize_BVH(const std::vector<cCollider*>& i_allColliderList);

	void AddCollider_BVH(cCollider* i_collider);

	cResult RemoveCollider_BVH(cCollider* i_collider);

	void CollisionDetection_BroadPhase_BVH();

	// TODO: rendering debug
	void UpdateRenderData();

	std::vector<Graphics::cLine*>& GetRenderData();



	// Narrow Phase
	//----------------------

	void CollisionDetection_NarrowPhase_Overlap(std::unordered_map<cCollider*, std::vector<cCollider*>>& i_CollisionMap_broadPhase);

	void InvokeCollisionCallback(std::unordered_map<cCollider*, std::vector<cCollider*>>& i_newCollisionMap);


	// Collision Resolution
	//----------------------

	void CollisionResolution(cCollider* i_lhs, cCollider* i_rhs);

	void CollisionResolution(cSphereCollider* i_lhs, cSphereCollider* i_rhs);

	void CollisionResolution(cAABBCollider* i_lhs, cSphereCollider* i_rhs);

}// Namespace Collision
}// Namespace Physics
}// Namespace eae6320
