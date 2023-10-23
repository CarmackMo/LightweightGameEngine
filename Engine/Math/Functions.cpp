// Includes
//=========

#include "Functions.h"

#include <cmath>
#include <cstring>
#include <Engine/Asserts/Asserts.h>

// Interface
//==========

uint16_t eae6320::Math::ConvertFloatToHalf( const float i_value )
{
	// Get the individual bits of the floating point value
	const auto bitRepresentation = [i_value]
	{
		uint32_t bitRepresentation;
		{
			EAE6320_ASSERT( sizeof( bitRepresentation ) == sizeof( i_value ) );
			memcpy( &bitRepresentation, &i_value, sizeof( bitRepresentation ) );
		}
		return bitRepresentation;
	}();
	// The first bit is the sign
	const auto sign_float = bitRepresentation & 0x80000000;
	const auto sign_half = static_cast<uint16_t>( sign_float >> 16 );
	// The next 8 bits are the exponent
	// (a half has 5 bits)
	const auto exponent_float = bitRepresentation & 0x7f800000;
	// The final 23 bits are the significand
	// (a half has 10 bits)
	const auto significand_float = bitRepresentation & 0x7fffff;
	// Check for values that are out of range
	const auto absoluteValue_float = bitRepresentation & 0x7fffffff;
	constexpr uint32_t maxHalfValue_float = 0x477FE000;	// Exponent == 15, Significand = 0b1111111111 << (23 - 10)
	if ( absoluteValue_float <= maxHalfValue_float )
	{
		constexpr uint32_t minNormalHalfValue_float = 0x38800000;	// Exponent == -14
		if ( absoluteValue_float >= minNormalHalfValue_float )
		{
			const auto value_half = static_cast<uint16_t>(
				// Shift the value to line up with the half bits
				// (this will truncate the significand)
				(absoluteValue_float >> 13)	// 23 - 10
				// Unbias the float exponent by subtracting 127 and then rebias for half precision by adding 15
				- 0x1c000);	// (127 - 15) << 10
			return value_half;
		}
		else
		{
			constexpr uint32_t minSubNormalHalfValue_float = 0x33800000;
			if ( absoluteValue_float < minSubNormalHalfValue_float )
			{
				// If the value is too small to be represented by a half it is set to zero
				return sign_half;
			}
			else
			{
				// The value is too small to be represented by normal half bit patterns,
				// and so it must be converted to a subnormal representation
				// (Warning! This code is tricky, and I'm not certain that it is correct)

				// Instead of an implicit leading 1 make it explicit
				const auto explicitSignificand_float = significand_float | 0x800000;
				// Half precision denormals always have an exponent of -14,
				// and so the significand must be shifted accordingly
				const auto significand_half = static_cast<uint16_t>(
					explicitSignificand_float >> ( ( 127 - ( exponent_float >> 23 ) ) - 1 ) );
				return sign_half | significand_half;
			}
		}
	}
	else
	{
		// The value is either too large to be represented as a half
		// or it is a NaN
		const auto isNaN =
			// Is the exponent 0xff?
			( exponent_float == 0x7f800000 )
			// If the exponent is 0xff and the significand is zero then the value is infinity.
			// Any other significand value is a NaN.
			&& ( significand_float != 0 );
		if ( !isNaN )
		{
			// If the value is too large then return +/- infinity
			constexpr uint16_t infinity_half = 0x7c00;
			return sign_half | infinity_half;
		}
		else
		{
			// The first bit of the NaN significand determines whether is quiet (vs. signaling)
			const auto isQuiet_float = significand_float & 0x400000;
			const uint16_t isQuiet_half = isQuiet_float ? 0x200 : 0;
			// The remaining bits are the NaN "payload"
			const auto payload_float = significand_float & 0x3fffff;
			// There is no standard of what the payload means,
			// and so it is simply truncated.
			const auto payload_half = static_cast<uint16_t>( significand_float & 0x1ff );
			return sign_half | isQuiet_half | payload_half;
		}
	}
}

float eae6320::Math::ConvertHorizontalFieldOfViewToVerticalFieldOfView( const float i_horizontalFieldOfView_inRadians,
	const float i_aspectRatio )
{
	return 2.0f * std::atan( std::tan( i_horizontalFieldOfView_inRadians * 0.5f ) / i_aspectRatio );
}
