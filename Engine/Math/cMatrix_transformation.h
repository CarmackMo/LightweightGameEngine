/*
	This class represents a full transformation
	(i.e. a 4x4 matrix composed of a 3x3 rotation matrix and a 3 element translation vector)

	Matrix Facts:
		* The float elements are stored as columns
			* In other words, the second float stored is the first element in the second row
			* This is so that all 16 floats can be copied directly to Direct3D or OpenGL
				(Both HLSL and GLSL interpret a matrix's registers to be columns)
		* The vectors are the columns
			* In other words, the first three columns are the rotation and the last column is the translation
			* This is an arbitrary choice, but it is by far the most common convention outside of computer graphics
				(within graphics literature it is probably slightly more common to have the vectors be rows)
			* This matches standard OpenGL but is backwards from standard Direct3D
		* The vectors are right-handed
			* This is an arbitrary choice; it matches Maya and standard OpenGL, and is backwards from standard Direct3D
			* The first vector is right, the second vector is up, and the third vector is back
				* This is an arbitrary convention, and not necessarily a common one
					(Maya, for example, has the first vector left and the third vector forward)
*/

#ifndef EAE6320_MATH_CMATRIX_TRANSFORMATION_H
#define EAE6320_MATH_CMATRIX_TRANSFORMATION_H

// Forward Declarations
//=====================

namespace eae6320
{
	namespace Math
	{
		class cQuaternion;
		struct sVector;
	}
}

// Class Declaration
//==================

namespace eae6320
{
	namespace Math
	{
		class cMatrix_transformation
		{
			// Interface
			//==========

		public:

			// Multiplication
			//---------------

			constexpr sVector operator *( const sVector& i_rhs ) const;
			constexpr cMatrix_transformation operator *( const cMatrix_transformation& i_rhs ) const;
			// If both transforms are known to be affine
			// (in our class this should be true of everything except camera-to-projected transforms)
			// then the following function is cheaper than general multiplication
			static constexpr cMatrix_transformation ConcatenateAffine(
				const cMatrix_transformation& i_nextTransform, const cMatrix_transformation& i_firstTransform );

			// Access
			//-------

			const sVector& GetRightDirection() const;
			const sVector& GetUpDirection() const;
			const sVector& GetBackDirection() const;
			const sVector& GetTranslation() const;

			// Camera
			//-------

			// A world-to-camera transform (for rendering) can be created by specifying the relative camera data
			static constexpr cMatrix_transformation CreateWorldToCameraTransform(
				const cQuaternion& i_cameraOrientation, const sVector& i_cameraPosition );
			// If a camera's local-to-world transform has already been created then it can be specified instead to save calculations
			static constexpr cMatrix_transformation CreateWorldToCameraTransform( const cMatrix_transformation& transform_localCameraToWorld );

			// A camera-to-projected transform (for rendering) can be created by specifying the relative data
			static cMatrix_transformation CreateCameraToProjectedTransform_perspective(
				// The vertical field of view is the vertical angle of the view frustum (i.e. how wide the camera can see vertically)
				//	* If you prefer to think in terms of degrees you can use Math::ConvertDegreesToRadians()
				//	* If you prefer to think in terms of horizontal field of view you can use Math::ConvertHorizontalFieldOfViewToVerticalFieldOfView()
				const float i_verticalFieldOfView_inRadians,
				// aspectRatio = width / height
				const float i_aspectRatio,
				// The near and far planes are perpendicular to the camera's forward direction
				// and determine what is drawn vs. what is clipped (not drawn) as well as depth precision:
				//	* Anything at a distance further than the near plane but closer than the far plane will be drawn
				//		* In other words, the greater the distance between the near and far plane the more things will be drawn
				//	* There is a finite range of precision that a depth buffer has, and that precision is spread between the near and far planes
				//		* To maximize precision the near plane should be as far from the camera as is acceptable
				//			(i.e. where you don't notice things disappearing when the camera gets close)
				//			and the far plane should be as near to the camera as is acceptable
				//			(i.e. where you don't notice things disappearing when the camera gets far away)
				const float i_z_nearPlane, const float i_z_farPlane );

			// Initialization / Clean Up
			//--------------------------

			constexpr cMatrix_transformation() = default;	// The default constructor creates a a transform with no rotation and no translation
			cMatrix_transformation( const cQuaternion& i_rotation, const sVector& i_translation );

			// Data
			//=====

		private:

			// Storage is column-major; see notes at the top of the file
			float m_00 = 1.0f, m_10 = 0.0f, m_20 = 0.0f, m_30 = 0.0f,
				m_01 = 0.0f, m_11 = 1.0f, m_21 = 0.0f, m_31 = 0.0f,
				m_02 = 0.0f, m_12 = 0.0f, m_22 = 1.0f, m_32 = 0.0f,
				m_03 = 0.0f, m_13 = 0.0f, m_23 = 0.0f, m_33 = 1.0f;

			// Implementation
			//===============

		private:

			// Initialization / Clean Up
			//--------------------------

			constexpr cMatrix_transformation(
				const float i_00, const float i_10, const float i_20, const float i_30,
				const float i_01, const float i_11, const float i_21, const float i_31,
				const float i_02, const float i_12, const float i_22, const float i_32,
				const float i_03, const float i_13, const float i_23, const float i_33 );
		};
	}
}

#include "cMatrix_transformation.inl"

#endif	// EAE6320_MATH_CMATRIX_TRANSFORMATION_H
