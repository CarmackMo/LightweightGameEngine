#ifndef EAE6320_MATH_CMATRIX_TRANSFORMATION_INL
#define EAE6320_MATH_CMATRIX_TRANSFORMATION_INL

// Includes
//=========

#include "cMatrix_transformation.h"

#include "sVector.h"

// Interface
//==========

// Multiplication
//---------------

constexpr eae6320::Math::sVector eae6320::Math::cMatrix_transformation::operator *( const sVector& i_rhs ) const
{
	return sVector(
		( m_00 * i_rhs.x ) + ( m_01 * i_rhs.y ) + ( m_02 * i_rhs.z ) + m_03,
		( m_10 * i_rhs.x ) + ( m_11 * i_rhs.y ) + ( m_12 * i_rhs.z ) + m_13,
		( m_20 * i_rhs.x ) + ( m_21 * i_rhs.y ) + ( m_22 * i_rhs.z ) + m_23
	);
}

constexpr eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::operator *( const cMatrix_transformation& i_rhs ) const
{
	return cMatrix_transformation(
		( m_00 * i_rhs.m_00 ) + ( m_01 * i_rhs.m_10 ) + ( m_02 * i_rhs.m_20 ) + ( m_03 * i_rhs.m_30 ),
		( m_10 * i_rhs.m_00 ) + ( m_11 * i_rhs.m_10 ) + ( m_12 * i_rhs.m_20 ) + ( m_13 * i_rhs.m_30 ),
		( m_20 * i_rhs.m_00 ) + ( m_21 * i_rhs.m_10 ) + ( m_22 * i_rhs.m_20 ) + ( m_23 * i_rhs.m_30 ),
		( m_30 * i_rhs.m_00 ) + ( m_31 * i_rhs.m_10 ) + ( m_32 * i_rhs.m_20 ) + ( m_33 * i_rhs.m_30 ),

		( m_00 * i_rhs.m_01 ) + ( m_01 * i_rhs.m_11 ) + ( m_02 * i_rhs.m_21 ) + ( m_03 * i_rhs.m_31 ),
		( m_10 * i_rhs.m_01 ) + ( m_11 * i_rhs.m_11 ) + ( m_12 * i_rhs.m_21 ) + ( m_13 * i_rhs.m_31 ),
		( m_20 * i_rhs.m_01 ) + ( m_21 * i_rhs.m_11 ) + ( m_22 * i_rhs.m_21 ) + ( m_23 * i_rhs.m_31 ),
		( m_30 * i_rhs.m_01 ) + ( m_31 * i_rhs.m_11 ) + ( m_32 * i_rhs.m_21 ) + ( m_33 * i_rhs.m_31 ),

		( m_00 * i_rhs.m_02 ) + ( m_01 * i_rhs.m_12 ) + ( m_02 * i_rhs.m_22 ) + ( m_03 * i_rhs.m_32 ),
		( m_10 * i_rhs.m_02 ) + ( m_11 * i_rhs.m_12 ) + ( m_12 * i_rhs.m_22 ) + ( m_13 * i_rhs.m_32 ),
		( m_20 * i_rhs.m_02 ) + ( m_21 * i_rhs.m_12 ) + ( m_22 * i_rhs.m_22 ) + ( m_23 * i_rhs.m_32 ),
		( m_30 * i_rhs.m_02 ) + ( m_31 * i_rhs.m_12 ) + ( m_32 * i_rhs.m_22 ) + ( m_33 * i_rhs.m_32 ),

		( m_00 * i_rhs.m_03 ) + ( m_01 * i_rhs.m_13 ) + ( m_02 * i_rhs.m_23 ) + ( m_03 * i_rhs.m_33 ),
		( m_10 * i_rhs.m_03 ) + ( m_11 * i_rhs.m_13 ) + ( m_12 * i_rhs.m_23 ) + ( m_13 * i_rhs.m_33 ),
		( m_20 * i_rhs.m_03 ) + ( m_21 * i_rhs.m_13 ) + ( m_22 * i_rhs.m_23 ) + ( m_23 * i_rhs.m_33 ),
		( m_30 * i_rhs.m_03 ) + ( m_31 * i_rhs.m_13 ) + ( m_32 * i_rhs.m_23 ) + ( m_33 * i_rhs.m_33 )
	);
}

constexpr eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::ConcatenateAffine(
	const cMatrix_transformation& i_nextTransform, const cMatrix_transformation& i_firstTransform )
{
	// A few simplifying assumptions can be made for affine transformations vs. general 4x4 matrix multiplication
	return cMatrix_transformation(
		( i_nextTransform.m_00 * i_firstTransform.m_00 ) + ( i_nextTransform.m_01 * i_firstTransform.m_10 ) + ( i_nextTransform.m_02 * i_firstTransform.m_20 ),
		( i_nextTransform.m_10 * i_firstTransform.m_00 ) + ( i_nextTransform.m_11 * i_firstTransform.m_10 ) + ( i_nextTransform.m_12 * i_firstTransform.m_20 ),
		( i_nextTransform.m_20 * i_firstTransform.m_00 ) + ( i_nextTransform.m_21 * i_firstTransform.m_10 ) + ( i_nextTransform.m_22 * i_firstTransform.m_20 ),
		0.0f,

		( i_nextTransform.m_00 * i_firstTransform.m_01 ) + ( i_nextTransform.m_01 * i_firstTransform.m_11 ) + ( i_nextTransform.m_02 * i_firstTransform.m_21 ),
		( i_nextTransform.m_10 * i_firstTransform.m_01 ) + ( i_nextTransform.m_11 * i_firstTransform.m_11 ) + ( i_nextTransform.m_12 * i_firstTransform.m_21 ),
		( i_nextTransform.m_20 * i_firstTransform.m_01 ) + ( i_nextTransform.m_21 * i_firstTransform.m_11 ) + ( i_nextTransform.m_22 * i_firstTransform.m_21 ),
		0.0f,

		( i_nextTransform.m_00 * i_firstTransform.m_02 ) + ( i_nextTransform.m_01 * i_firstTransform.m_12 ) + ( i_nextTransform.m_02 * i_firstTransform.m_22 ),
		( i_nextTransform.m_10 * i_firstTransform.m_02 ) + ( i_nextTransform.m_11 * i_firstTransform.m_12 ) + ( i_nextTransform.m_12 * i_firstTransform.m_22 ),
		( i_nextTransform.m_20 * i_firstTransform.m_02 ) + ( i_nextTransform.m_21 * i_firstTransform.m_12 ) + ( i_nextTransform.m_22 * i_firstTransform.m_22 ),
		0.0f,

		( i_nextTransform.m_00 * i_firstTransform.m_03 ) + ( i_nextTransform.m_01 * i_firstTransform.m_13 ) + ( i_nextTransform.m_02 * i_firstTransform.m_23 ) + i_nextTransform.m_03,
		( i_nextTransform.m_10 * i_firstTransform.m_03 ) + ( i_nextTransform.m_11 * i_firstTransform.m_13 ) + ( i_nextTransform.m_12 * i_firstTransform.m_23 ) + i_nextTransform.m_13,
		( i_nextTransform.m_20 * i_firstTransform.m_03 ) + ( i_nextTransform.m_21 * i_firstTransform.m_13 ) + ( i_nextTransform.m_22 * i_firstTransform.m_23 ) + i_nextTransform.m_23,
		1.0f
	);
}

// Camera
//-------

constexpr eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform(
	const cQuaternion& i_cameraOrientation, const sVector& i_cameraPosition )
{
	return CreateWorldToCameraTransform( cMatrix_transformation( i_cameraOrientation, i_cameraPosition ) );
}

constexpr eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateWorldToCameraTransform( const cMatrix_transformation& i_transform_localCameraToWorld )
{
	// Many simplifying assumptions can be made in order to create the inverse
	// because in our class a camera can only ever have rotation and translation
	// (i.e. it can't be scaled)
	return cMatrix_transformation(
		i_transform_localCameraToWorld.m_00, i_transform_localCameraToWorld.m_01, i_transform_localCameraToWorld.m_02, 0.0f,
		i_transform_localCameraToWorld.m_10, i_transform_localCameraToWorld.m_11, i_transform_localCameraToWorld.m_12, 0.0f,
		i_transform_localCameraToWorld.m_20, i_transform_localCameraToWorld.m_21, i_transform_localCameraToWorld.m_22, 0.0f,

		-( i_transform_localCameraToWorld.m_03 * i_transform_localCameraToWorld.m_00 ) - ( i_transform_localCameraToWorld.m_13 * i_transform_localCameraToWorld.m_10 ) - ( i_transform_localCameraToWorld.m_23 * i_transform_localCameraToWorld.m_20 ),
		-( i_transform_localCameraToWorld.m_03 * i_transform_localCameraToWorld.m_01 ) - ( i_transform_localCameraToWorld.m_13 * i_transform_localCameraToWorld.m_11 ) - ( i_transform_localCameraToWorld.m_23 * i_transform_localCameraToWorld.m_21 ),
		-( i_transform_localCameraToWorld.m_03 * i_transform_localCameraToWorld.m_02 ) - ( i_transform_localCameraToWorld.m_13 * i_transform_localCameraToWorld.m_12 ) - ( i_transform_localCameraToWorld.m_23 * i_transform_localCameraToWorld.m_22 ),

		1.0f );
}

// Implementation
//===============

// Initialization / Clean Up
//--------------------------

constexpr eae6320::Math::cMatrix_transformation::cMatrix_transformation(
	const float i_00, const float i_10, const float i_20, const float i_30,
	const float i_01, const float i_11, const float i_21, const float i_31,
	const float i_02, const float i_12, const float i_22, const float i_32,
	const float i_03, const float i_13, const float i_23, const float i_33 )
	:
	m_00( i_00 ), m_10( i_10 ), m_20( i_20 ), m_30( i_30 ),
	m_01( i_01 ), m_11( i_11 ), m_21( i_21 ), m_31( i_31 ),
	m_02( i_02 ), m_12( i_12 ), m_22( i_22 ), m_32( i_32 ),
	m_03( i_03 ), m_13( i_13 ), m_23( i_23 ), m_33( i_33 )
{

}

#endif	// EAE6320_MATH_CMATRIX_TRANSFORMATION_INL
