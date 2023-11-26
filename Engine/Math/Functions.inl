#ifndef EAE6320_MATH_FUNCTIONS_INL
#define EAE6320_MATH_FUNCTIONS_INL

// Includes
//=========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/Constants.h>

#include <cmath>



// Interface
//==========

inline bool eae6320::Math::IsNAN(float val)
{
	volatile float val_ = val;
	return val != val_;
}


inline bool eae6320::Math::IsZero(float val)
{
	return AreEqual_Eps(0.0f, val, .000000001f);
}


inline bool eae6320::Math::AreEqual_Eps(float lhs, float rhs, float maxDiff)
{
	return fabs(lhs - rhs) < maxDiff;
}


inline bool eae6320::Math::AreEqual_Rel(float lhs, float rhs, float maxDiff)
{
	if (lhs == rhs)
		return true;

	float relDiff;

	if (fabs(rhs) > fabs(lhs))
		relDiff = static_cast<float>(fabs((lhs - rhs) / rhs));
	else
		relDiff = static_cast<float>(fabs((lhs - rhs) / lhs));

	return relDiff <= maxDiff;
}


inline bool eae6320::Math::AreEqual_Accurate(float lhs, float rhs, float maxDiff, unsigned int maxULPS)
{
	if (lhs == rhs)
		return true;

	double diff = fabs(lhs - rhs);

	if (diff <= maxDiff)
		return true;

	unsigned int intDiff = abs(*reinterpret_cast<int*>(&lhs) - *reinterpret_cast<int*>(&rhs));

	return intDiff <= maxULPS;
}


inline bool eae6320::Math::AreEqual(float lhs, float rhs, float maxDiff)
{
#if defined(EFFICIENT_COMPARE)
	return AreEqual_Eps(lhs, rhs, maxDiff);
#elif defined(ACCURATE_COMPARE)
	return AreEqual_Accurate(lhs, rhs, maxDiff);
#else
	return AreEqual_Rel(lhs, rhs, maxDiff);
#endif
}


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