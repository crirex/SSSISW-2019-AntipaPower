#pragma once

#include <random>
class RandomGenerator
{
public:
	RandomGenerator() = default;
	~RandomGenerator() = default;

	static double GetRealInRange(const int lowerBound, const int upperBound);
	static int GetIntegerInRange(const double lowerBound, const double upperBound);
	static std::vector<double> GenerateRealNumbers(const int lowerBound, const int upperBound, const int size);
};

