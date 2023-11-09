/*
	This struct can be used to represent the state of a
	rigid body in 3D space
*/

#ifndef EAE6320_PHYSICS_SRIGIDBODYSTATE_H
#define EAE6320_PHYSICS_SRIGIDBODYSTATE_H

// Includes
//=========

#include <Engine/Math/cQuaternion.h>
#include <Engine/Math/sVector.h>


// Forward Declarations
//=====================

namespace eae6320
{
namespace Math
{
	class cMatrix_transformation;
}
}


// sRigidBodyState Struct Declaration
//===================

namespace eae6320
{
namespace Physics
{
	struct sRigidBodyState
	{
		// Data
		//=====

		Math::sVector position;	// In arbitrary units determined by the applicaton's convention
		Math::sVector velocity;	// Distance per second
		Math::sVector acceleration;	// Distance per second^2
		Math::cQuaternion orientation;
		Math::sVector angularVelocity_axis_local = Math::sVector( 0.0f, 0.0f, 0.0f );	// In local space (not world space)
		float angularSpeed = 0.0f;	// Radians per second (positive values rotate right-handed, negative rotate left-handed)

		// Interface
		//==========

		void Update( const float i_secondCountToIntegrate );
		Math::sVector PredictFuturePosition( const float i_secondCountToExtrapolate ) const;
		Math::cQuaternion PredictFutureOrientation( const float i_secondCountToExtrapolate ) const;
		Math::cMatrix_transformation PredictFutureTransform( const float i_secondCountToExtrapolate ) const;

		void Translate(Math::sVector& i_translation);
	};

}// Namespace Physics
}// Namespace eae6320


// cRigidBody Struct Declaration
//===================

namespace eae6320
{
namespace Physics
{

}// Namespace Physics
}// Namespace eae6320






#endif	// EAE6320_PHYSICS_SRIGIDBODYSTATE_H
