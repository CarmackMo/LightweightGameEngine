#pragma once
#include "Dependency.h"


using namespace Engine;


#define MAX_DIFF 0.00001f
#define USE_EFFICIENCY
//#define USE_ACCURACY





/** 
 * @brief Compare two float values by checking whether the absolute difference between the two 
 *		  float values is within a specified maximum difference threshold. This comparison method 
 *		  has fastest performance but lowest accuracy 
 */
inline bool AreEqualEps(float lhs, float rhs, float maxDiff = MAX_DIFF)
{
	return fabs(lhs - rhs) < maxDiff;
}

/** 
 * @brief Compare two float values by checking whether the relative difference between the two float 
 *		  values is within a specified maximum difference threshold. 
 *		  The advantage of using relative difference for comparisons is that the significance of a 
 *		  difference between two values may vary depending on the magnitude of the values themselves. 
 *		  A small difference between two large values might be considered negligible, while the same 
 *		  difference between two small values could be significant. 
 *		  This comparison method is balanced between efficiency and accuracy 
 */
inline bool AreEqualRel(float lhs, float rhs, float maxDiff = MAX_DIFF)
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

/** 
 * @brief Compare two float values by chacking whether the difference of binary bits between the two float 
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
inline bool AreEqualAccurate(float lhs, float rhs, float maxDiff = MAX_DIFF, unsigned int maxULPS = 12)
{
	assert(sizeof(float) == sizeof(int));

	if (lhs == rhs)
		return true;

	double diff = fabs(lhs - rhs);

	if (diff <= maxDiff)
		return true;

	unsigned int intDiff = abs(*reinterpret_cast<int*>(&lhs) - *reinterpret_cast<int*>(&rhs));

	return intDiff <= maxULPS;
}

/**
 * @brief General entry point of comparison functions. 
 *		  If "USE_EFFICIENCY" is defined, use "AreEqualEps()" to compare;
 *		  Else if "USE_ACCURACY" is defined, use "AreEqualAccurate()" to compare;
 *		  Otherwise use "AreEqualRel()" to compare.
 */
inline bool AreEqual(float lhs, float rhs, float maxDiff = MAX_DIFF)
{
#if defined(USE_EFFICIENCY)
	return AreEqualEps(lhs, rhs, maxDiff);
#elif defined(USE_ACCURACY)
	return AreEqualAccurate(lhs, rhs, maxDiff);
#else
	return AreEqualRel(lhs, rhs, maxDiff);
#endif
}

/** @brief Check if the given number is not a number */
inline bool IsNAN(float val)
{
	volatile float val_ = val;
	return val != val_;
}

/** @brief Check if the given number equals to zero */
inline bool IsZero(float val)
{
	return AreEqualEps(0.0f, val, .000000001f);
}

/** @brief Swap two numbers by modifying their value in the memory */
template<typename T>
inline void Swap(T& i_Left, T& i_Right)
{
	T temp = i_Left;
	i_Left = i_Right;
	i_Right = temp;
}