#pragma once
#include "Dependency.h"
#include "DataType.h"


using namespace Engine;


// fastest
inline bool AreEqual_Eps(float i_lhs, float i_rhs, float i_maxDiff)
{
	return fabs(i_lhs - i_rhs) < i_maxDiff;
}

// balanced
inline bool AreEqual_Rel(float i_lhs, float i_rhs, float i_maxDiff)
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
inline bool AreEqual_Accurate(float i_lhs, float i_rhs, float i_maxDiff, unsigned int i_maxULPS)
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