// Includes
//=========

#include <Engine/Logging/Logging.h>
#include <Engine/Physics/Collision.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cSphereCollider.h>


#include <unordered_map>
#include <algorithm>



// Static Data
//============

namespace
{
	std::unordered_map<eae6320::Physics::cCollider*, std::vector<eae6320::Physics::cCollider*>>
		s_collisionMap;

	std::vector<eae6320::Physics::cCollider*> s_orderedColliderList_xAxis;
	std::vector<eae6320::Physics::cCollider*> s_orderedColliderList_yAxis;
	std::vector<eae6320::Physics::cCollider*> s_orderedColliderList_zAxis;

	

	eae6320::Physics::cBVHTree s_BVHTree;

}


// Helper Funcitons
//============

namespace
{

	auto s_comparator_xAxis = [](eae6320::Physics::cCollider* i_lhs, eae6320::Physics::cCollider* i_rhs) -> bool
	{
		return i_lhs->GetMinExtent_world().x < i_rhs->GetMinExtent_world().x;
	};

	auto s_comparator_yAxis = [](eae6320::Physics::cCollider* i_lhs, eae6320::Physics::cCollider* i_rhs) -> bool
	{
		return i_lhs->GetMinExtent_world().y < i_rhs->GetMinExtent_world().y;
	};

	auto s_comparator_zAxis = [](eae6320::Physics::cCollider* i_lhs, eae6320::Physics::cCollider* i_rhs) -> bool
	{
		return i_lhs->GetMinExtent_world().z < i_rhs->GetMinExtent_world().z;
	};

}





// Interface
//============


eae6320::Math::sVector eae6320::Physics::Collision::GetSphereCollisionNormal(cSphereCollider* i_lhs, cSphereCollider* i_rhs)
{
	return (i_lhs->GetCentroid_world() - i_rhs->GetCentroid_world()).GetNormalized();
}


bool eae6320::Physics::Collision::IsOverlaps(cCollider* i_lhs, cCollider* i_rhs)
{
	switch (i_lhs->GetType())
	{

	case eColliderType::Sphere:
	{
		if (i_rhs->GetType() == eColliderType::Sphere)
		{
			return dynamic_cast<cSphereCollider*>(i_lhs)->IsOverlaps(*dynamic_cast<cSphereCollider*>(i_rhs));
		}
		else if (i_rhs->GetType() == eColliderType::AABB)
		{
			return dynamic_cast<cSphereCollider*>(i_lhs)->IsOverlaps(*dynamic_cast<cAABBCollider*>(i_rhs));
		}
		else if (i_rhs->GetType() == eColliderType::Plane)
		{
			//TODO
			return false;
		}
		else
			return false;
	}

	case eColliderType::AABB:
	{
		if (i_rhs->GetType() == eColliderType::Sphere)
		{
			return dynamic_cast<cSphereCollider*>(i_rhs)->IsOverlaps(*dynamic_cast<cAABBCollider*>(i_lhs));
		}
		else if (i_rhs->GetType() == eColliderType::AABB)
		{
			return dynamic_cast<cAABBCollider*>(i_lhs)->IsOverlaps(*dynamic_cast<cAABBCollider*>(i_rhs));
		}
		else if (i_rhs->GetType() == eColliderType::Plane)
		{
			// TODO
			return false;
		}
		else
			return false;
	}

	case eColliderType::Plane:
	{
		// TODO
		return false;
	}

	default:
		return false;

	}

}


void eae6320::Physics::Collision::Initialize(const std::vector<cCollider*>& i_allColliderList)
{
	//Initialize_SweepAndPrune(i_allColliderList);

	Initialize_BVH(i_allColliderList);
}


void eae6320::Physics::Collision::UpdateCollision()
{
	//CollisionDetection_BroadPhase_SweepAndPrune();

	CollisionDetection_BroadPhase_BVH();
}


// Broad Phase: Sweep and Prune
//============

void eae6320::Physics::Collision::Initialize_SweepAndPrune(const std::vector<cCollider*>& i_allColliderList)
{
	// Initialize buffers
	{
		s_orderedColliderList_xAxis = std::vector<cCollider*>(i_allColliderList);
		s_orderedColliderList_yAxis = std::vector<cCollider*>(i_allColliderList);
		s_orderedColliderList_zAxis = std::vector<cCollider*>(i_allColliderList);

		for (cCollider* collider : i_allColliderList)
		{
			s_collisionMap[collider] = std::vector<cCollider*>(0);
		}
	}

	// Initial collision detection
	{
		CollisionDetection_BroadPhase_SweepAndPrune();
	}
}


void eae6320::Physics::Collision::AddCollider_SweepAndPrune(cCollider* i_collider)
{
	s_orderedColliderList_xAxis.push_back(i_collider);
	s_orderedColliderList_yAxis.push_back(i_collider);
	s_orderedColliderList_zAxis.push_back(i_collider);

	s_collisionMap[i_collider] = std::vector<cCollider*>(0);

	CollisionDetection_BroadPhase_SweepAndPrune();
}


eae6320::cResult eae6320::Physics::Collision::RemoveCollider_SweepAndPrune(cCollider* i_collider)
{
	// Remove collider from axis order list
	{
		auto iter_xAxis = s_orderedColliderList_xAxis.begin();
		auto iter_yAxis = s_orderedColliderList_yAxis.begin();
		auto iter_zAxis = s_orderedColliderList_zAxis.begin();

		if ((iter_xAxis = std::find(s_orderedColliderList_xAxis.begin(), s_orderedColliderList_xAxis.end(), i_collider)) != s_orderedColliderList_xAxis.end() &&
			(iter_yAxis = std::find(s_orderedColliderList_yAxis.begin(), s_orderedColliderList_yAxis.end(), i_collider)) != s_orderedColliderList_yAxis.end() &&
			(iter_zAxis = std::find(s_orderedColliderList_zAxis.begin(), s_orderedColliderList_zAxis.end(), i_collider)) != s_orderedColliderList_zAxis.end())
		{
			s_orderedColliderList_xAxis.erase(iter_xAxis);
			s_orderedColliderList_yAxis.erase(iter_yAxis);
			s_orderedColliderList_zAxis.erase(iter_zAxis);
		}
		else
		{
			Logging::OutputError("Physics::Collision: Trying to remove a non-existed collider");
			return Results::Failure;
		}
	}

	// Remove collider from collision map
	{
		auto iter = s_collisionMap.begin();

		if ((iter = s_collisionMap.find(i_collider)) != s_collisionMap.end())
		{
			s_collisionMap.erase(i_collider);
		}
		else
		{
			Logging::OutputError("Physics::Collision: Trying to remove a non-existed collider");
			return Results::Failure;
		}
	}

	// Re-calculate collision map
	{
		CollisionDetection_BroadPhase_SweepAndPrune();
	}

	return Results::Success;
}


void eae6320::Physics::Collision::CollisionDetection_BroadPhase_SweepAndPrune()
{
	// Update collider data
	{
		std::sort(s_orderedColliderList_xAxis.begin(), s_orderedColliderList_xAxis.end(), s_comparator_xAxis);
		std::sort(s_orderedColliderList_yAxis.begin(), s_orderedColliderList_yAxis.end(), s_comparator_yAxis);
		std::sort(s_orderedColliderList_zAxis.begin(), s_orderedColliderList_zAxis.end(), s_comparator_zAxis);
	}

	std::unordered_map<cCollider*, std::vector<cCollider*>> collisionMap_broadPhase;
	for (const auto& item : s_collisionMap)
	{
		collisionMap_broadPhase[item.first] = std::vector<cCollider*>(0);
	}

	// Sweep and prune the X axis
	{
		// Iterate X axis, find all potential collision along X axis
		for (size_t i = 0; i < s_orderedColliderList_xAxis.size() - 1; i++)
		{
			for (size_t j = i + 1; j < s_orderedColliderList_xAxis.size(); j++)
			{
				cCollider* collider_i = s_orderedColliderList_xAxis[i];
				cCollider* collider_j = s_orderedColliderList_xAxis[j];

				// Possible to have collision
				if (collider_i->GetMaxExtent_world().x >= collider_j->GetMinExtent_world().x)
				{
					collisionMap_broadPhase[collider_i].push_back(collider_j);
				}
				// Impossbile to have collision
				else
				{
					break;
				}
			}
		}
	}

	// Sweep and prune the Y axis
	{
		// Create buffer
		std::unordered_map<cCollider*, std::vector<cCollider*>> collisionAtYAxis;
		for (const auto& item : collisionMap_broadPhase)
		{
			collisionAtYAxis[item.first] = std::vector<cCollider*>(0);
		}

		// Iterate Y axis, find all potential collision along Y axis, select those who already have potential collision in X axis 
		for (size_t i = 0; i < s_orderedColliderList_yAxis.size() - 1; i++)
		{
			for (size_t j = i + 1; j < s_orderedColliderList_yAxis.size(); j++)
			{
				cCollider* collider_i = s_orderedColliderList_yAxis[i];
				cCollider* collider_j = s_orderedColliderList_yAxis[j];

				// Possible to have collision
				if (collider_i->GetMaxExtent_world().y >= collider_j->GetMinExtent_world().y)
				{
					const auto& collisionList_i = collisionMap_broadPhase[collider_i];
					const auto& collisionList_j = collisionMap_broadPhase[collider_j];

					if (std::find(collisionList_i.begin(), collisionList_i.end(), collider_j) != collisionList_i.end())
						collisionAtYAxis[collider_i].push_back(collider_j);
					else if (std::find(collisionList_j.begin(), collisionList_j.end(), collider_i) != collisionList_j.end())
						collisionAtYAxis[collider_j].push_back(collider_i);
				}
				// Impossbile to have collision
				else
				{
					break;
				}
			}
		}

		for (auto& collision : collisionMap_broadPhase)
		{
			collision.second.swap(collisionAtYAxis[collision.first]);
		}
	}

	// Sweep and prune the Z axis
	{
		// Create buffer
		std::unordered_map<cCollider*, std::vector<cCollider*>> collisionAtZAxis;
		for (const auto& item : collisionMap_broadPhase)
		{
			collisionAtZAxis[item.first] = std::vector<cCollider*>(0);
		}

		// Iterate Z axis, find all potential collision along Z axis, select those who already have potential collision in X and Y axis 
		for (size_t i = 0; i < s_orderedColliderList_zAxis.size() - 1; i++)
		{
			for (size_t j = i + 1; j < s_orderedColliderList_zAxis.size(); j++)
			{
				cCollider* collider_i = s_orderedColliderList_zAxis[i];
				cCollider* collider_j = s_orderedColliderList_zAxis[j];

				// Possible to have collision
				if (collider_i->GetMaxExtent_world().z >= collider_j->GetMinExtent_world().z)
				{
					const auto& collisionList_i = collisionMap_broadPhase[collider_i];
					const auto& collisionList_j = collisionMap_broadPhase[collider_j];

					if (std::find(collisionList_i.begin(), collisionList_i.end(), collider_j) != collisionList_i.end())
						collisionAtZAxis[collider_i].push_back(collider_j);
					else if (std::find(collisionList_j.begin(), collisionList_j.end(), collider_i) != collisionList_j.end())
						collisionAtZAxis[collider_j].push_back(collider_i);
				}
				// Impossbile to have collision
				else
				{
					break;
				}
			}
		}

		for (auto& collision : collisionMap_broadPhase)
		{
			collision.second.swap(collisionAtZAxis[collision.first]);
		}
	}

	// Proceed to narrow phase collision detection
	{
		CollisionDetection_NarrowPhase_Overlap(collisionMap_broadPhase);
	}
	
}


// Broad Phase: BVH Tree
//============

void eae6320::Physics::Collision::Initialize_BVH(const std::vector<cCollider*>& i_allColliderList)
{
	// Initialize buffer
	for (cCollider* collider : i_allColliderList)
	{
		s_collisionMap[collider] = std::vector<cCollider*>(0);
	}

	// Initialize collision detection
	for (cCollider* collider : i_allColliderList)
	{
		s_BVHTree.Add(collider);
	}

	// Initial collision detection
	CollisionDetection_BroadPhase_BVH();

	UpdateRenderData();
}


void eae6320::Physics::Collision::AddCollider_BVH(cCollider* i_collider)
{
	s_BVHTree.Add(i_collider);
	s_collisionMap[i_collider] = std::vector<cCollider*>(0);

	CollisionDetection_BroadPhase_BVH();
}


eae6320::cResult eae6320::Physics::Collision::RemoveCollider_BVH(cCollider* i_collider)
{
	// Remove collider from BVH tree
	s_BVHTree.Remove(i_collider);

	// Remove collider from collision map
	{
		auto iter = s_collisionMap.begin();

		if ((iter = s_collisionMap.find(i_collider)) != s_collisionMap.end())
		{
			s_collisionMap.erase(i_collider);
		}
		else
		{
			Logging::OutputError("Physics::Collision: Trying to remove a non-existed collider");
			return Results::Failure;
		}
	}

	// Re-calculate collision map
	CollisionDetection_BroadPhase_BVH();

	return Results::Success;
}


void eae6320::Physics::Collision::CollisionDetection_BroadPhase_BVH()
{
	// Update collider data
	s_BVHTree.Update();

	// Collision detection for each colliders
	std::unordered_map<cCollider*, std::vector<cCollider*>> collisionMap_broadPhase;
	for (const auto& item : s_collisionMap)
	{
		collisionMap_broadPhase[item.first] = std::vector<cCollider*>(0);

		std::vector<cCollider*> collisionList = s_BVHTree.Query(item.first);

		// Ignore duplicate collision
		for (cCollider* collider : collisionList)
		{
			if (collisionMap_broadPhase.find(collider) == collisionMap_broadPhase.end())
				collisionMap_broadPhase[item.first].push_back(collider);
		}
	}

	// Proceed to narrow phase collision detection
	CollisionDetection_NarrowPhase_Overlap(collisionMap_broadPhase);
}


void eae6320::Physics::Collision::UpdateRenderData()
{
	s_BVHTree.UpdatetRenderData();
}


std::vector<eae6320::Graphics::cLine*>& eae6320::Physics::Collision::GetRenderData()
{
	return s_BVHTree.GetRenderData();
}



// Narrow Phase
//============

void eae6320::Physics::Collision::CollisionDetection_NarrowPhase_Overlap(std::unordered_map<cCollider*, std::vector<cCollider*>>& i_CollisionMap_broadPhase)
{
	// Perform narrow phase collision detection for the data from broad phase
	{
		for (auto& collision : i_CollisionMap_broadPhase)
		{
			cCollider* collider_lhs = collision.first;

			auto iter = collision.second.begin();
			while (iter != collision.second.end())
			{
				if (IsOverlaps(collider_lhs, *iter) == false)
				{
					iter = collision.second.erase(iter);
				}
				else
				{
					iter++;
				}
			}

			collision.second.shrink_to_fit();
		}
	}

	InvokeCollisionCallback(i_CollisionMap_broadPhase);

	s_collisionMap.swap(i_CollisionMap_broadPhase);
}


void eae6320::Physics::Collision::InvokeCollisionCallback(std::unordered_map<cCollider*, std::vector<cCollider*>>& i_newCollisionMap)
{
	// Invoke OnCollisionExit for colliders that on longer overlap in current frame 
	{
		for (const auto& collision : s_collisionMap)
		{
			cCollider* collider_lhs = collision.first;

			for (cCollider* collider_rhs : collision.second)
			{
				if ((i_newCollisionMap.find(collider_lhs) == i_newCollisionMap.end() ||
						std::find(i_newCollisionMap[collider_lhs].begin(), i_newCollisionMap[collider_lhs].end(), collider_rhs) == i_newCollisionMap[collider_lhs].end()) &&
					(i_newCollisionMap.find(collider_rhs) == i_newCollisionMap.end() || 
						std::find(i_newCollisionMap[collider_rhs].begin(), i_newCollisionMap[collider_rhs].end(), collider_lhs) == i_newCollisionMap[collider_rhs].end()))
				{
					if (collider_lhs->OnCollisionExit != nullptr)
						collider_lhs->OnCollisionExit(collider_rhs);
					if (collider_rhs->OnCollisionExit != nullptr)
						collider_rhs->OnCollisionExit(collider_lhs);
				}
			}
		}
	}

	// Invoke OnCollisionEnter and OnCollisionStay for colliders that having overlap in current frame 
	{
		for (const auto& collision : i_newCollisionMap)
		{
			cCollider* collider_lhs = collision.first;

			for (cCollider* collider_rhs : collision.second)
			{
				if ((s_collisionMap.find(collider_lhs) == s_collisionMap.end() ||
						std::find(s_collisionMap[collider_lhs].begin(), s_collisionMap[collider_lhs].end(), collider_rhs) == s_collisionMap[collider_lhs].end()) &&
					(s_collisionMap.find(collider_rhs) == s_collisionMap.end() || 
						std::find(s_collisionMap[collider_rhs].begin(), s_collisionMap[collider_rhs].end(), collider_lhs) == s_collisionMap[collider_rhs].end()))
				{
					if (collider_lhs->OnCollisionEnter != nullptr)
						collider_lhs->OnCollisionEnter(collider_rhs);
					if (collider_rhs->OnCollisionEnter != nullptr)
						collider_rhs->OnCollisionEnter(collider_lhs);
				}
				else
				{
					if (collider_lhs->OnCollisionStay != nullptr)
						collider_lhs->OnCollisionStay(collider_rhs);
					if (collider_rhs->OnCollisionStay != nullptr)
						collider_rhs->OnCollisionStay(collider_lhs);
				}
			}
		}
	}

}



// Collision Resolution
//============

void eae6320::Physics::Collision::CollisionResolution()
{

}


