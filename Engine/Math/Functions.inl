#ifndef EAE6320_MATH_FUNCTIONS_INL
#define EAE6320_MATH_FUNCTIONS_INL

// Includes
//=========

#include "Functions.h"

#include "Constants.h"

#include <cmath>
#include <Engine/Asserts/Asserts.h>

// Interface
//==========

constexpr float eae6320::Math::ConvertDegreesToRadians( const float i_degrees )
{
	return i_degrees * ( g_pi / 180.0f );
}

	template<typename tUnsignedInteger, class EnforceUnsigned>
tUnsignedInteger eae6320::Math::RoundUpToMultiple( const tUnsignedInteger i_value, const tUnsignedInteger i_multiple )
{
	EAE6320_ASSERTF( i_multiple != 0, "Zero isn't a valid multiple" );
	EAE6320_ASSERTF( i_multiple > 0, "The multiple must be positive" );
	// Adding one less than the multiple will make the value at or above the next hiehst multiple
	// unless the value was itself a multiple.
	// Dividng and multiplying by the multiple removes any remainder
	const auto returnValue = ( ( i_value + i_multiple - 1 ) / i_multiple ) * i_multiple;
	EAE6320_ASSERT( ( returnValue % i_multiple ) == 0 );
	return returnValue;
}

	template<typename tUnsignedInteger, class EnforceUnsigned>
tUnsignedInteger eae6320::Math::RoundUpToMultiple_powerOf2( const tUnsignedInteger i_value, const tUnsignedInteger i_multipleWhichIsAPowerOf2 )
{
	EAE6320_ASSERTF( i_multipleWhichIsAPowerOf2 != 0, "Zero isn't a valid multiple" );
	EAE6320_ASSERTF( i_multipleWhichIsAPowerOf2 > 0, "The multiple must be positive" );
	// To be a power-of-2 the multiple can only have a single bit set;
	// get a mask of the bits less-significant than that single bit
	const auto nonLeadingBits = i_multipleWhichIsAPowerOf2 - 1;
	EAE6320_ASSERTF( ( i_multipleWhichIsAPowerOf2 && ( i_multipleWhichIsAPowerOf2 & nonLeadingBits ) ) == 0, "The multiple must be a power-of-2" );
	// Adding the non-leading bits will make the value at or above the next highest multiple
	// unless the value was itself a multiple.
	// ANDing with inverse then removes any bits less than the multiple.
	const auto returnValue = ( i_value + nonLeadingBits ) & ~nonLeadingBits;
	EAE6320_ASSERT( ( returnValue % i_multipleWhichIsAPowerOf2 ) == 0 );
	return returnValue;
}

#endif	// EAE6320_MATH_FUNCTIONS_INL