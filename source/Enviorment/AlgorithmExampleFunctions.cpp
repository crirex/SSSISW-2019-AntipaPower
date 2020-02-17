#include "Enviorment/AlgorithmExampleFunctions.h"


double AlgorithmExampleFunctions::function1(Individual<> individual)
{
	const int minimInterval = -10;
	const int maximInterval = 10;

	const auto& cromosons = individual.GetChromosomes();
	const auto& x = cromosons[0].GetValueInInterval(minimInterval, maximInterval);

	return x * x + 6 * x + 1;
}

double AlgorithmExampleFunctions::function2(Individual<> individual)
{
	const int xMinimInterval = -5;
	const int xMaximInterval = 3;
	const int yMinimInterval = 2;
	const int yMaximInterval = 10;

	const auto& cromosons = individual.GetChromosomes();
	const auto& x = cromosons[0].GetValueInInterval(xMinimInterval, xMaximInterval);
	const auto& y = cromosons[1].GetValueInInterval(yMinimInterval, yMaximInterval);

	return std::sin(std::_Pi * 10 * x + 10 / (1 + y * y)) + std::log(x * x + y * y);
}
