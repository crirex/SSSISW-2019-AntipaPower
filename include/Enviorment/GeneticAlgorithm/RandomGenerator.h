#pragma once

#include <random>
class RandomGenerator
{
public:
	static double GetRealInRange(double lowerBound, double upperBound);
	static int GetIntegerInRange(int lowerBound, int upperBound);
	static std::vector<double> GenerateRealNumbers(double lowerBound, double upperBound, size_t size);
};

