#pragma once

// Includes
//========

#include <Engine/Results/Results.h>


// Interface
//==========

namespace eae6320
{
namespace Math
{
namespace Random
{

	cResult Initialize();

	/* @brief Return a random float value within the range [0.0, 1.0], inclusive in both sides */
	float Randf();

	/* @brief Return a random integer value within the range [0, INT_MAX], inclusive in both sides */
	int Randi();

	/*	@brief Generate a random integer value within range [lowerBound, upperBound). Note that the range is inclusive
	 *		   in left side and exclusive in right side. */
	int RandInRange(int lowerBound, int upperBound);

	/*	@brief Generate a random float value within range [lowerBound, upperBound]. Note that the range inclusive
	 *		   in both sides. */
	float RandInRange(float lowerBound, float upperBound);

}// Namespace Random
}// Namespace Math
}// Namespace eae6320