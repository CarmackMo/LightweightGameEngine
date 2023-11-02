// Includes
//=========

#include <Engine/Physics/Physics.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cSphereCollider.h>


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
