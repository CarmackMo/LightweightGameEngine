// Includes
//=========

#include "cMatrix_transformation.h"

#include "cQuaternion.h"
#include "sVector.h"

#include <cmath>

// Interface
//==========

// Access
//-------

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetRightDirection() const
{
	return *reinterpret_cast<const sVector*>( &m_00 );
}

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetUpDirection() const
{
	return *reinterpret_cast<const sVector*>( &m_01 );
}

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetBackDirection() const
{
	return *reinterpret_cast<const sVector*>( &m_02 );
}

const eae6320::Math::sVector& eae6320::Math::cMatrix_transformation::GetTranslation() const
{
	return *reinterpret_cast<const sVector*>( &m_03 );
}

// Camera
//-------

eae6320::Math::cMatrix_transformation eae6320::Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(
	const float i_verticalFieldOfView_inRadians,
	const float i_aspectRatio,
	const float i_z_nearPlane, const float i_z_farPlane )
{
	const auto yScale = 1.0f / std::tan( i_verticalFieldOfView_inRadians * 0.5f );
	const auto xScale = yScale / i_aspectRatio;
#if defined( EAE6320_PLATFORM_D3D )
	const auto zDistanceScale = i_z_farPlane / ( i_z_nearPlane - i_z_farPlane );
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zDistanceScale, -1.0f,
		0.0f, 0.0f, i_z_nearPlane * zDistanceScale, 0.0f );
#elif defined( EAE6320_PLATFORM_GL )
	const auto zDistanceScale = 1.0f / ( i_z_nearPlane - i_z_farPlane );
	return cMatrix_transformation(
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, ( i_z_nearPlane + i_z_farPlane ) * zDistanceScale, -1.0f,
		0.0f, 0.0f, ( 2.0f * i_z_nearPlane * i_z_farPlane ) * zDistanceScale, 0.0f );
#endif
}

// Initialize / Clean Up
//----------------------

eae6320::Math::cMatrix_transformation::cMatrix_transformation( const cQuaternion& i_rotation, const sVector& i_translation )
	:
	m_30( 0.0f ), m_31( 0.0f ), m_32( 0.0f ),
	m_03( i_translation.x ), m_13( i_translation.y ), m_23( i_translation.z ),
	m_33( 1.0f )
{
	const auto _2x = i_rotation.m_x + i_rotation.m_x;
	const auto _2y = i_rotation.m_y + i_rotation.m_y;
	const auto _2z = i_rotation.m_z + i_rotation.m_z;
	const auto _2xx = i_rotation.m_x * _2x;
	const auto _2xy = _2x * i_rotation.m_y;
	const auto _2xz = _2x * i_rotation.m_z;
	const auto _2xw = _2x * i_rotation.m_w;
	const auto _2yy = _2y * i_rotation.m_y;
	const auto _2yz = _2y * i_rotation.m_z;
	const auto _2yw = _2y * i_rotation.m_w;
	const auto _2zz = _2z * i_rotation.m_z;
	const auto _2zw = _2z * i_rotation.m_w;

	m_00 = 1.0f - _2yy - _2zz;
	m_01 = _2xy - _2zw;
	m_02 = _2xz + _2yw;

	m_10 = _2xy + _2zw;
	m_11 = 1.0f - _2xx - _2zz;
	m_12 = _2yz - _2xw;

	m_20 = _2xz - _2yw;
	m_21 = _2yz + _2xw;
	m_22 = 1.0f - _2xx - _2yy;
}
