#pragma once

// Includes
//=========

#include <Engine/Physics/cColliderBase.h>
#include <Engine/Results/Results.h>


#include <vector>


// Interface
//==========

namespace eae6320
{
namespace Physics
{

	bool IsOverlaps(cCollider* i_lhs, cCollider* i_rhs);



	//void Update();




	void Initialize_SweepAndPrune(const std::vector<eae6320::Physics::cCollider*>& i_allColliderList);

	void AddCollider_SweepAndPrune(cCollider* i_collider);

	cResult RemoveCollider_SweepAndPrune(cCollider* i_collider);

	void DetectCollision_BroadPhase_SweepAndPrune();


}// Namespace Physics
}// Namespace eae6320
