// Includes
//=========

#include <Engine/Math/cMatrix_transformation.h>
#include <Engine/Physics/cRigidBody.h>


// Interface
//==========

void eae6320::Physics::sRigidBodyState::Update( const float i_secondCountToIntegrate )
{
	// Update position
	{
		position += velocity * i_secondCountToIntegrate;
	}
	// Update velocity
	{
		velocity += acceleration * i_secondCountToIntegrate;
	}
	// Update orientation
	{
		const auto rotation = Math::cQuaternion( angularSpeed * i_secondCountToIntegrate, angularVelocity_axis_local );
		orientation = orientation * rotation;
		orientation.Normalize();
	}
}


eae6320::Math::sVector eae6320::Physics::sRigidBodyState::GetForwardDirection() const
{
	return orientation.CalculateForwardDirection().GetNormalized();
}


eae6320::Math::sVector eae6320::Physics::sRigidBodyState::PredictFuturePosition( const float i_secondCountToExtrapolate ) const
{
	return position + ( velocity * i_secondCountToExtrapolate );
}


eae6320::Math::cQuaternion eae6320::Physics::sRigidBodyState::PredictFutureOrientation( const float i_secondCountToExtrapolate ) const
{
	const auto rotation = Math::cQuaternion( angularSpeed * i_secondCountToExtrapolate, angularVelocity_axis_local );
	return Math::cQuaternion( orientation * rotation ).GetNormalized();
}


eae6320::Math::cMatrix_transformation eae6320::Physics::sRigidBodyState::PredictFutureTransform( const float i_secondCountToExtrapolate ) const
{
	return Math::cMatrix_transformation( PredictFutureOrientation( i_secondCountToExtrapolate ), PredictFuturePosition( i_secondCountToExtrapolate ) );
}


void eae6320::Physics::sRigidBodyState::Translate(Math::sVector& i_translation)
{
	position += i_translation;
}
