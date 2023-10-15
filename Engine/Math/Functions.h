/*
	This file contains math-related functions
*/

#ifndef EAE6320_MATH_FUNCTIONS_H
#define EAE6320_MATH_FUNCTIONS_H

#include <cstdint>
#include <type_traits>

namespace eae6320
{
	namespace Math
	{
		// Interface
		//==========

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
