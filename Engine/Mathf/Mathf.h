#pragma once
#include <random>
#include <cassert>
#include "./Utility/Singleton.h"


namespace Engine
{

#define MAX_DIFF 0.00001f
#define USE_EFFICIENCY
//#define USE_ACCURACY


/* Forward declaration */
class Mathf;

inline bool IsNAN(float val);
inline bool IsZero(float val);

inline bool AreEqualEps(float lhs, float rhs, float maxDiff = MAX_DIFF);
inline bool AreEqualRel(float lhs, float rhs, float maxDiff = MAX_DIFF);
inline bool AreEqualAccurate(float lhs, float rhs, float maxDiff = MAX_DIFF, unsigned int maxULPS = 12);
inline bool AreEqual(float lhs, float rhs, float maxDiff = MAX_DIFF);

inline int RandInRange(int lowerBound, int upperBound);
inline float RandInRange(float lowerBound, float upperBound);

template<typename T> inline void Swap(T& left, T& right);



/**
 *	@brief A helper singleton class that mainly designed for handling random value generation. As a  
 *		   helper class, "Mathf" is suppose to be "private" to engine users (Although they can access
 *		   Mathf's instance). Therefore, all members are labeled as private and only specified engine
 *		   code can access them.
 * 
 *		   "Mathf" implements two helper functions "Randf()" and "Randi()" for random value generation. 
 *		   Both functions generate random value using the uniform distribution. 
 * 
 *		   "Randf()" generates a random float value within the range [0.0, 1.0], both sides are 
 *		   inclusive. According to the formular: lower + (upper - lower) * factor (0 <= factor <= 1), 
 *		   we can generate random number in a specific range with both sides inclusive.
 * 
 *		   "Randi()" generates a random integer value within the range [0, INT_MAX], both sides are 
 *		   inclusive. According to the formular: lower + range % (upper - lower) (0 <= range <= Max),
 *		   we can generate random number in a specific range left side inclusive and right side
 *		   exclusive.
 */
class Mathf : public Singleton<Mathf>
{
private:
	std::random_device						rd;			/* Seed source for the random number engine, unique in the whole engine scope */
	std::mt19937							generator;			/* Random number engine */
	std::uniform_real_distribution<float>	distf;
	std::uniform_int_distribution<int>		disti;

	inline Mathf() { Initialize(); }
	inline ~Mathf() {};

	inline void Initialize()
	{
		generator = std::mt19937(rd());
		distf = std::uniform_real_distribution<float>(0.0f, nextafter(1.0f, FLT_MAX));
		disti = std::uniform_int_distribution<int>(0, INT_MAX);
	}

	/** @brief Return a random float value within the range [0.0, 1.0], inclusive in both sides */
	inline float Randf() { return distf(generator); }

	/** @brief Return a random integer value within the range [0, INT_MAX], inclusive in both sides */
	inline int Randi() { return disti(generator); }

public:
	friend class Singleton<Mathf>;
	friend inline int RandInRange(int lowerBound, int upperBound);
	friend inline float RandInRange(float lowerBound, float upperBound);
};


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


/** 
 * @brief Compare two float values by checking whether the absolute difference between the two 
 *		  float values is within a specified maximum difference threshold. This comparison method 
 *		  has fastest performance but lowest accuracy 
 */
inline bool AreEqualEps(float lhs, float rhs, float maxDiff)
{
	return fabs(lhs - rhs) < maxDiff;
}


/** 
 * @brief Compare two float values by checking whether the relative difference between the two float 
 *		  values is within a specified maximum difference threshold. 
 * 
 *		  The advantage of using relative difference for comparisons is that the significance of a 
 *		  difference between two values may vary depending on the magnitude of the values themselves. 
 *		  A small difference between two large values might be considered negligible, while the same 
 *		  difference between two small values could be significant. 
 * 
 *		  This comparison method is balanced between efficiency and accuracy 
 */
inline bool AreEqualRel(float lhs, float rhs, float maxDiff)
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
inline bool AreEqualAccurate(float lhs, float rhs, float maxDiff, unsigned int maxULPS)
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
inline bool AreEqual(float lhs, float rhs, float maxDiff)
{
#if defined(USE_EFFICIENCY)
	return AreEqualEps(lhs, rhs, maxDiff);
#elif defined(USE_ACCURACY)
	return AreEqualAccurate(lhs, rhs, maxDiff);
#else
	return AreEqualRel(lhs, rhs, maxDiff);
#endif
}


/** 
 *	@brief Generate a random integer value within range [lowerBound, upperBound). Note that the range is inclusive
 *		   in left side and exclusive in right side. See "Mathf" class for more info.
 */
inline int RandInRange(int lowerBound, int upperBound)
{
	assert(lowerBound < upperBound);
	int range = upperBound - lowerBound;
	return lowerBound + Mathf::Instance()->Randi() % range;
}


/**
 *	@brief Generate a random float value within range [lowerBound, upperBound]. Note that the range inclusive
 *		   in both sides. See "Mathf" class for more info.
 */
inline float RandInRange(float lowerBound, float upperBound)
{
	assert(lowerBound < upperBound);
	float diff = upperBound - lowerBound;
	return lowerBound + Mathf::Instance()->Randf() * diff;
}


/** @brief Swap two numbers by modifying their value in the memory */
template<typename T>
inline void Swap(T& left, T& right)
{
	T temp = left;
	left = right;
	right = temp;
}




namespace MathfTest
{

/********************************* Unit tests **************************************/

//TODO: Need to develop a more reliable fucntion to check whether RandInRange() is random
//		enough.

#if defined(_DEBUG)

#include <vector>


inline void RandInRangeIntUnitTest()
{
	/* Generate random values */
	std::vector<int> samples = std::vector<int>(1000);
	for (int i = 0; i < 1000; i++)
	{
		samples[i] = RandInRange(1, 100);
	}

	/* Calculate the mean and variance of the sample values */
	double mean = 0.0;
	double variance = 0.0;
	for (int value : samples) 
	{
		mean += value;
		variance += value * value;
	}
	mean /= samples.size();
	variance = (variance / samples.size()) - (mean * mean);

	/* Check if the variance is large enough, and if the mean is close enough to the expectation */
	double expectedMean = 0.0;		/* Expected mean */ 
	double expectedVariance = 1.0;  /* Expected variance */
	double threshold = 0.01;		/* Acceptable threshold */

	bool res = (variance > expectedVariance - threshold)
		&& (variance < expectedVariance + threshold)
		&& (mean > expectedMean - threshold)
		&& (mean < expectedMean + threshold);
}


inline void RandInRangeFloatUnitTest()
{
	/* Generate random values */
	std::vector<float> samples = std::vector<float>(1000);
	for (int i = 0; i < 1000; i++)
	{
		samples[i] = RandInRange(1.0f, 100.0f);
	}

	/* Calculate the mean and variance of the sample values */
	double mean = 0.0;
	double variance = 0.0;
	for (float value : samples)
	{
		mean += value;
		variance += value * value;
	}
	mean /= samples.size();
	variance = (variance / samples.size()) - (mean * mean);

	/* Check if the variance is large enough, and if the mean is close enough to the expectation */
	double expectedMean = 0.0;		/* Expected mean */
	double expectedVariance = 1.0;  /* Expected variance */
	double threshold = 0.01;		/* Acceptable threshold */

	bool res = (variance > expectedVariance - threshold)
		&& (variance < expectedVariance + threshold)
		&& (mean > expectedMean - threshold)
		&& (mean < expectedMean + threshold);
}

#endif

}//Namespace Mathf
}//Namespace Engine
