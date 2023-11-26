
// Includes
//========

#include <Engine/Asserts/Asserts.h>
#include <Engine/Math/Random.h>

#include <random>


// Static Data
//============

namespace
{
	std::random_device						s_randomDevice;			/* Seed source for the random number engine, unique in the whole engine scope */
	std::mt19937							s_generator;			/* Random number engine */
	std::uniform_real_distribution<float>	s_distf;
	std::uniform_int_distribution<int>		s_disti;
}


// Interface
//============

eae6320::cResult eae6320::Math::Random::Initialize()
{
	s_generator = std::mt19937(s_randomDevice());
	s_distf = std::uniform_real_distribution<float>(0.0f, nextafter(1.0f, FLT_MAX));
	s_disti = std::uniform_int_distribution<int>(0, INT_MAX);

	return Results::Success;
}


float eae6320::Math::Random::Randf()
{
	return s_distf(s_generator);
}


int eae6320::Math::Random::Randi()
{
	return s_disti(s_generator);
}


int eae6320::Math::Random::RandInRange(int lowerBound, int upperBound)
{
	EAE6320_ASSERT(lowerBound < upperBound);
	int range = upperBound - lowerBound;
	return lowerBound + Randi() % range;
}

float eae6320::Math::Random::RandInRange(float lowerBound, float upperBound)
{
	EAE6320_ASSERT(lowerBound < upperBound);
	float diff = upperBound - lowerBound;
	return lowerBound + Randf() * diff;
}
