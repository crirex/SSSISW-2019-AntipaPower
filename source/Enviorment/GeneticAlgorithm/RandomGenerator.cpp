#include "RandomGenerator.h"

const double RandomGenerator::getRealInRange(const int lowerBound, const int upperBound)
{
	std::random_device m_randomDevice;
	std::mt19937 rng(m_randomDevice());
	std::uniform_real_distribution<> distribution(lowerBound, upperBound);

	return distribution(rng);
}

const int RandomGenerator::getIntegerInRange(const double lowerBound, const double upperBound)
{
	std::random_device m_randomDevice;
	std::mt19937 rng(m_randomDevice());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(lowerBound, upperBound);

	return distribution(rng);
}
