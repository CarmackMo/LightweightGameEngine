#pragma once
#include "Dependency.h"


using namespace Engine;


#define MAX_DIff 0.00001f
#define USE_EFFICIENCY
//#define USE_ACCURACY



/* Fastest but lowest accuracy */
inline bool AreEqual_Eps(float lhs, float rhs, float maxDiff = MAX_DIff)
{
	return fabs(lhs - rhs) < maxDiff;
}

/* Balanced between efficiency and accuracy */
inline bool AreEqual_Rel(float lhs, float rhs, float maxDiff = MAX_DIff)
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

/* Lowest effiency but highest accuracy */
inline bool AreEqual_Accurate(float lhs, float rhs, float maxDiff = MAX_DIff, unsigned int maxULPS = 12)
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

/* General entry point of comparison functions */
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