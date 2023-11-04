// Includes
//=========

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
		s_collisionPair;

	std::vector<eae6320::Physics::cCollider*> s_orderedColliderList_xAxis;
	std::vector<eae6320::Physics::cCollider*> s_orderedColliderList_yAxis;
	std::vector<eae6320::Physics::cCollider*> s_orderedColliderList_zAxis;




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
//==========

bool eae6320::Physics::IsOverlaps(cCollider* i_lhs, cCollider* i_rhs)
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


void eae6320::Physics::Initialize_sweepAndPrune(const std::vector<cCollider*>& i_allColliderList)
{
	s_orderedColliderList_xAxis = std::vector<cCollider*>(i_allColliderList);
	s_orderedColliderList_yAxis = std::vector<cCollider*>(i_allColliderList);
	s_orderedColliderList_zAxis = std::vector<cCollider*>(i_allColliderList);

	std::sort(s_orderedColliderList_xAxis.begin(), s_orderedColliderList_xAxis.end(), s_comparator_xAxis);
	std::sort(s_orderedColliderList_yAxis.begin(), s_orderedColliderList_yAxis.end(), s_comparator_yAxis);
	std::sort(s_orderedColliderList_zAxis.begin(), s_orderedColliderList_zAxis.end(), s_comparator_zAxis);

	// Sweep and prune the X axis
	{
		for (int i = 0; i < s_orderedColliderList_xAxis.size() - 1; i++)
		{
			for (int j = i + 1; j < s_orderedColliderList_xAxis.size(); j++)
			{
				cCollider* collider_i = s_orderedColliderList_xAxis[i];
				cCollider* collider_j = s_orderedColliderList_xAxis[j];

				// Possible to have collision
				if (collider_i->GetMaxEntent_world().x >= collider_j->GetMinExtent_world().x)
				{
					if (s_collisionPair.find(collider_i) == s_collisionPair.end())
						s_collisionPair[collider_i] = std::vector<eae6320::Physics::cCollider*>(0);

					s_collisionPair[collider_i].push_back(collider_j);
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
		for (int i = 0; i < s_orderedColliderList_yAxis.size() - 1; i++)
		{
			for (int j = i + 1; j < s_orderedColliderList_yAxis.size(); j++)
			{
				cCollider* collider_i = s_orderedColliderList_yAxis[i];
				cCollider* collider_j = s_orderedColliderList_yAxis[j];

				std::vector<cCollider*> targetCollider;

				// Possible to have collision
				if (collider_i->GetMaxEntent_world().y >= collider_j->GetMinExtent_world().y &&
					s_collisionPair.find(collider_i) != s_collisionPair.end() &&
					std::find(s_collisionPair[collider_i].begin(), s_collisionPair[collider_i].end(), collider_j) != s_collisionPair[collider_i].end())
				{
					targetCollider.push_back(collider_j);
				}
				// Impossbile to have collision
				else
				{
					break;
				}

				if (s_collisionPair.find(collider_i) == s_collisionPair.end())
					s_collisionPair[collider_i] = std::vector<eae6320::Physics::cCollider*>(0);

				s_collisionPair[collider_i].swap(targetCollider);
			}
		}
	}
	// Sweep and prune the Z axis
	{
		for (int i = 0; i < s_orderedColliderList_zAxis.size() - 1; i++)
		{
			for (int j = i + 1; j < s_orderedColliderList_zAxis.size(); j++)
			{
				cCollider* collider_i = s_orderedColliderList_zAxis[i];
				cCollider* collider_j = s_orderedColliderList_zAxis[j];

				std::vector<cCollider*> targetCollider;

				// Possible to have collision
				if (collider_i->GetMaxEntent_world().z >= collider_j->GetMinExtent_world().z &&
					s_collisionPair.find(collider_i) != s_collisionPair.end() &&
					std::find(s_collisionPair[collider_i].begin(), s_collisionPair[collider_i].end(), collider_j) != s_collisionPair[collider_i].end())
				{
					targetCollider.push_back(collider_j);
				}
				// Impossbile to have collision
				else
				{
					break;
				}

				if (s_collisionPair.find(collider_i) == s_collisionPair.end())
					s_collisionPair[collider_i] = std::vector<eae6320::Physics::cCollider*>(0);

				s_collisionPair[collider_i].swap(targetCollider);
			}
		}

	}
}
