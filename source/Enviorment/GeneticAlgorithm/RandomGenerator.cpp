#include "RandomGenerator.h"

double RandomGenerator::GetRealInRange(const int lowerBound, const int upperBound)
{
	std::random_device m_randomDevice;
	std::mt19937 rng(m_randomDevice());
	std::uniform_real_distribution<> distribution(lowerBound, upperBound);

	return distribution(rng);
}

int RandomGenerator::GetIntegerInRange(const double lowerBound, const double upperBound)
{
	std::random_device m_randomDevice;
	std::mt19937 rng(m_randomDevice());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(lowerBound, upperBound);

	return distribution(rng);
}

std::vector<double> RandomGenerator::GenerateRealNumbers(const int lowerBound, const int upperBound, const int size)
{
	std::random_device m_randomDevice;
	std::mt19937 rng(m_randomDevice());
	std::uniform_real_distribution<> distribution(lowerBound, upperBound);

	std::vector<double> randomNumbers(size);
	std::generate(randomNumbers.begin(), randomNumbers.end(), [&]()mutable {return distribution(rng); });
	return randomNumbers;
}
