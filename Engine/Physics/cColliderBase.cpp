// Includes
//=========

#include <Engine/Logging/Logging.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>
#include <Engine/Physics/cSphereCollider.h>
#include <Engine/ScopeGuard/cScopeGuard.h>


// Collider Builder
//=============

void eae6320::Physics::sColliderSetting::SettingForSphere(Math::sVector i_center, float i_radius)
{
	type = eColliderType::Sphere;
	sphere_center = i_center;
	sphere_radius = i_radius;
}


void eae6320::Physics::sColliderSetting::SettingForAABB(Math::sVector i_min, Math::sVector i_max)
{
	type = eColliderType::AABB;
	AABB_min = i_min;
	AABB_max = i_max;
}


// cCollider Implementation
//==================

eae6320::cResult eae6320::Physics::cCollider::Create(cCollider*& o_collider, const sColliderSetting& i_setting)
{

	auto result = Results::Success;

	cCollider* newCollider = nullptr;

	switch (i_setting.type)
	{
		case eColliderType::Sphere:
		{
			// TODO
			newCollider = dynamic_cast<cCollider*>(new cSphereCollider(i_setting.sphere_center, i_setting.sphere_radius));
			break;
		}
		case eColliderType::AABB:
		{
			// TODO
			newCollider = dynamic_cast<cCollider*>(new cAABBCollider(i_setting.AABB_min, i_setting.AABB_max));
			break;
		}
		case eColliderType::Plane:
		{
			// TODO
			break;
		}
		case eColliderType::None:
		{
			break;
		}
	}

	o_collider = newCollider;

	return result;
}


eae6320::Physics::eColliderType eae6320::Physics::cCollider::GetType() const
{
	return m_type;
}

