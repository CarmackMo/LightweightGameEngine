/*
	This file contains math-related functions
*/

#ifndef EAE6320_MATH_FUNCTIONS_H
#define EAE6320_MATH_FUNCTIONS_H

#include <cstdint>
#include <type_traits>


// Constants
//========

namespace eae6320
{
namespace Math
{

#define MAX_DIFF 0.00001f
#define EFFICIENT_COMPARE

}
}


// Interface
//==========

namespace eae6320
{
namespace Math
{
	/** @brief Check if the given number is not a number */
	inline bool IsNAN(float val);

	/** @brief Check if the given number equals to zero */
	inline bool IsZero(float val);

	/* @brief Compare two float values by checking whether the absolute difference between the two
	 *		  float values is within a specified maximum difference threshold. This comparison method
	 *		  has fastest performance but lowest accuracy
	 */
	inline bool AreEqual_Eps(float lhs, float rhs, float maxDiff = MAX_DIFF);

	/* @brief Compare two float values by checking whether the relative difference between the two float
	 *		  values is within a specified maximum difference threshold.
	 *
	 *		  The advantage of using relative difference for comparisons is that the significance of a
	 *		  difference between two values may vary depending on the magnitude of the values themselves.
	 *		  A small difference between two large values might be considered negligible, while the same
	 *		  difference between two small values could be significant.
	 *
	 *		  This comparison method is balanced between efficiency and accuracy
	 */
	inline bool AreEqual_Rel(float lhs, float rhs, float maxDiff = MAX_DIFF);

	/* @brief Compare two float values by chacking whether the difference of binary bits between the two float
	 *		  values is within a specified threshold called "Units In The Last Place (ULPs)". The procedure of
	 *		  comparison shows as below:
	 *		  1: Assert the size of 'float' is equal to the size of 'int'. Guarantee reinterpreting from 'float'
	 *			 to 'int' later in the code is safe.
	 *		  2: If 'lhs' and 'rhs' are equal (using '==' operator) or the absolute difference between them is
	 *			 smaller than threshold, return 'true' immediately, indicating equality.
	 *		  3: If step 2 fails, proceed to calculate the bit difference between the binary formats of 'lhs' and
	 *			 'rhs'. This is done by casting the addresses of 'lhs' and 'rhs' as pointers to integers and then
	 *			 dereferencing them.
	 *		  4: The bit difference is calculated as the absolute value of the difference between the reinterpreted
	 *			 integers. According to the feature of binary format of float-point value (See IEEE 754 for detail),
	 *			 the two values can be considered equal if the difference of their units in the last place is within
	 *			 specified range.
	 *		  This comparison method has lowest performance but highest accuracy.
	 */
	inline bool AreEqual_Accurate(float lhs, float rhs, float maxDiff = MAX_DIFF, unsigned int maxULPS = 12);

	/* @brief General entry point of comparison functions.
	 *		  If "USE_EFFICIENCY" is defined, use "AreEqualEps()" to compare;
	 *		  Else if "USE_ACCURACY" is defined, use "AreEqualAccurate()" to compare;
	 *		  Otherwise use "AreEqualRel()" to compare.
	 */
	inline bool AreEqual(float lhs, float rhs, float maxDiff = MAX_DIFF);


	// The engine uses radians internally, but this function can be used to convert from degrees
	constexpr float ConvertDegreesToRadians( const float i_degrees );

	// Converts from standard 32 bit floats to 16 bit floats
	// (where the bit representation is returned as a uint16_t since C++ doesn't support them natively)
	uint16_t ConvertFloatToHalf( const float i_value );

	// cMatrix_transformation::CreateCameraToProjectedTransform_perspective() expects a vertical FOV,
	// but a horizontal FOV can be converted to vertical using this function
	float ConvertHorizontalFieldOfViewToVerticalFieldOfView( const float i_horizontalFieldOfView_inRadians,
		// aspectRatio = width / height
		const float i_aspectRatio );

	// Rounds up an unsigned integer to the next highest multiple
		template<typename tUnsignedInteger, class EnforceUnsigned = typename std::enable_if_t<std::is_unsigned<tUnsignedInteger>::value>>
	tUnsignedInteger RoundUpToMultiple( const tUnsignedInteger i_value, const tUnsignedInteger i_multiple );

	// If the multiple is known to be a power-of-2 this is cheaper than the previous function
		template<typename tUnsignedInteger, class EnforceUnsigned = typename std::enable_if_t<std::is_unsigned<tUnsignedInteger>::value>>
	tUnsignedInteger RoundUpToMultiple_powerOf2( const tUnsignedInteger i_value, const tUnsignedInteger i_multipleWhichIsAPowerOf2 );
}
}

#include "Functions.inl"

#endif	// EAE6320_MATH_FUNCTIONS_H
