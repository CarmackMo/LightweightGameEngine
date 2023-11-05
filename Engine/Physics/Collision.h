#pragma once

// Includes
//=========

#include <Engine/Physics/cColliderBase.h>


#include <vector>


// Interface
//==========

namespace eae6320
{
namespace Physics
{

	bool IsOverlaps(cCollider* i_lhs, cCollider* i_rhs);





	void Initialize_SweepAndPrune(const std::vector<eae6320::Physics::cCollider*>& i_allColliderList);


	void DetectCollision_BroadPhase_SweepAndPrune();


}// Namespace Physics
}// Namespace eae6320
