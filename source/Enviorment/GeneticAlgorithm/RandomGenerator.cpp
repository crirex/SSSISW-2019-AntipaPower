#include "Enviorment/GeneticAlgorithm/RandomGenerator.h"

double RandomGenerator::GetRealInRange(double lowerBound, double upperBound)
{
	std::random_device m_randomDevice;
	std::mt19937 rng(m_randomDevice());
	std::uniform_real_distribution<> distribution(lowerBound, upperBound);

	return distribution(rng);
}

int RandomGenerator::GetIntegerInRange(int lowerBound, int upperBound)
{
	std::random_device m_randomDevice;
	std::mt19937 rng(m_randomDevice());
	std::uniform_int_distribution<std::mt19937::result_type> distribution(lowerBound, upperBound);

	return distribution(rng);
}

std::vector<double> RandomGenerator::GenerateRealNumbers(double lowerBound, double upperBound, size_t size)
{
	std::vector<double> randomNumbers(size);
	std::generate(randomNumbers.begin(), randomNumbers.end(), [&]() {
		return GetRealInRange(lowerBound, upperBound);
		});

	return randomNumbers;
}
