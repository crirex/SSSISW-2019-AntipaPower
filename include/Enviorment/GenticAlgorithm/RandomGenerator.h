#pragma once

#include <random>
class RandomGenerator
{
public:
	RandomGenerator() = default;
	~RandomGenerator() = default;

	static const double getRealInRange(const int lowerBound, const int upperBound);
	static const int getIntegerInRange(const double lowerBound, const double upperBound);

};

