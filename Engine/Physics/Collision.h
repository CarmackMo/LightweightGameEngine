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





	void Initialize_sweepAndPrune(const std::vector<eae6320::Physics::cCollider*>& i_allColliderList);



}// Namespace Physics
}// Namespace eae6320
