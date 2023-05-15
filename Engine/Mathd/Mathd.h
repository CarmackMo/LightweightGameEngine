#pragma once
#include "Dependency.h"


using namespace Engine;


#define MAX_DIff 0.00001f
#define USE_EFFICIENCY
//#define USE_ACCURACY



// fastest
inline bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff = MAX_DIff)
{
	return fabs(i_lhs - i_rhs) < i_maxDiff;
}

// balanced
inline bool AreEqual_Rel(float i_lhs, float i_rhs, float i_maxDiff = MAX_DIff)
{
	if (i_lhs == i_rhs)
		return true;

	float relDiff;

	if (fabs(i_rhs) > fabs(i_lhs))
		relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_rhs));
	else
		relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_lhs));

	return relDiff <= i_maxDiff;
}

// slow but sure
inline bool AreEqual_Accurate(float i_lhs, float i_rhs, float i_maxDiff = MAX_DIff, unsigned int i_maxULPS = 12)
{
	assert(sizeof(float) == sizeof(int));

	if (i_lhs == i_rhs)
		return true;

	double diff = fabs(i_lhs - i_rhs);

	if (diff <= i_maxDiff)
		return true;

	unsigned int intDiff = abs(*reinterpret_cast<int*>(&i_lhs) - *reinterpret_cast<int*>(&i_rhs));

	return intDiff <= i_maxULPS;
}


inline bool IsEqual(float lhs, float rhs, float maxDiff = MAX_DIff)
{

#if defined(USE_EFFICIENCY)
	return AreEqual_Eps(lhs, rhs, maxDiff);
#elif defined(USE_ACCURACY)
	return AreEqual_Accurate(lhs, rhs, maxDiff);
#else
	return AreEqual_Rel(lhs, rhs, maxDiff);
#endif // USE_EFFICIENCY
}


inline bool IsNAN(float val)
{
	volatile float val_ = val;
	return val != val_;
}

inline bool IsZero(float val)
{
	return AreEqual_Eps(0.0f, val, .000000001f);
}


template<typename T>
inline void Swap(T& i_Left, T& i_Right)
{
	T temp = i_Left;
	i_Left = i_Right;
	i_Right = temp;
}