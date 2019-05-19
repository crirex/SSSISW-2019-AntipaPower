#pragma once

#include <functional>
#include <numeric>

#include "Individual.h"

template <size_t size>
class GeneticAlgorithm
{
private:
	void InitalizePopulation();
	void CalculateCumulativeProbability(std::vector<double> &probabilityOfSelection, std::vector<double> &cumulativeProbabilyVector);
	void CalculateProbabilityOfSelection(std::vector<double> &probabilityOfSelection, double sumOfFitness);
	void CalculateSumOfFitnessFunction(double& sumOfFitness);
	bool IsInInterval(const double number, const double lowerBound, const double upperBound) const;
public:
	GeneticAlgorithm(std::function<double(double)> fitnessFunction, const size_t initialPopulationSize = 6, const size_t numberOfEpochs = 300, const double crossoverProb = 0.3, const double mutationProb = 0.3, const size_t chNumber = 1);
	~GeneticAlgorithm() = default;

	std::vector<Individual<size>>& GetIndividuals();
	double GetResult(const double number) const;


	std::vector<Individual<size>> Selection();
private:
	std::vector<Individual<size>> m_individuals;
	double m_crossoverProbability;
	double m_mutationProbability;
	size_t m_numberOfEpochs;
	size_t m_populationSize;
	size_t m_chromosomeNumberPerIndividual;
	std::function<double(double)> m_fitnessFunction;
};

template<size_t size>
inline void GeneticAlgorithm<size>::InitalizePopulation()
{
	for (size_t index = 0; index < m_populationSize; index++)
	{
		m_individuals.emplace_back(Individual<size>(m_chromosomeNumberPerIndividual));
	}
}

//gonna make it work on a single variable
template<size_t size>
inline std::vector<Individual<size>> GeneticAlgorithm<size>::Selection()
{
	double sumOfFitness = 0;
	std::vector<double> probabilityOfSelection{};
	std::vector<double> cumulativeProbabilityVector{};
	std::vector<double> randomNumbers = RandomGenerator::GenerateRealNumbers(0, 1, this->m_populationSize);
	std::vector<Individual<size>> newPopulation{};

	CalculateSumOfFitnessFunction(sumOfFitness);
	CalculateProbabilityOfSelection(probabilityOfSelection, sumOfFitness);
	CalculateCumulativeProbability(probabilityOfSelection, cumulativeProbabilityVector);

	for (size_t index = 0; index < this->m_populationSize; index++)
	{
		if (IsInInterval(randomNumbers[index], 0, cumulativeProbabilityVector[index]))
		{
			newPopulation.emplace_back(this->m_individuals[index]);
		}
		else
		{
			for (size_t index2 = 1; index2 < this->m_populationSize - 1; index2++)
			{
				if (IsInInterval(randomNumbers[index], cumulativeProbabilityVector[index2], cumulativeProbabilityVector[index2 + 1]))
				{
					newPopulation.emplace_back(this->m_individuals[index2 + 1]);
					break;
				}
			}
		}
	}

	std::cout << std::accumulate(probabilityOfSelection.begin(), probabilityOfSelection.end(), 0.0) << std::endl;
	return this->m_individuals;
}

template<size_t size>
inline void GeneticAlgorithm<size>::CalculateCumulativeProbability(std::vector<double> &probabilityOfSelection, std::vector<double> &cumulativeProbabilyVector)
{
	for (size_t index1 = 0; index1 < this->m_populationSize; ++index1)
	{
		double cumulativeProbability{};
		for (size_t index2 = 0; index2 < index1 + 1; ++index2)
		{
			cumulativeProbability += probabilityOfSelection[index2];
		}
		cumulativeProbabilyVector.emplace_back(cumulativeProbability);
	}
}

template<size_t size>
inline void GeneticAlgorithm<size>::CalculateProbabilityOfSelection(std::vector<double> &probabilityOfSelection, double sumOfFitness)
{
	std::for_each(this->m_individuals.begin(), this->m_individuals.end(), [&](Individual<size>& individual)
	{
		for (Chromosome<size>& chromosome : individual.GetChromosomes())
		{
			probabilityOfSelection.emplace_back(this->m_fitnessFunction(chromosome.GetValueInInterval(-10, 10)) / sumOfFitness); // de adaugat din param
		}
	});
}

template<size_t size>
inline void GeneticAlgorithm<size>::CalculateSumOfFitnessFunction(double& sumOfFitness)
{
	std::for_each(this->m_individuals.begin(), this->m_individuals.end(), [&](Individual<size>& individual)
	{
		for (Chromosome<size>& chromosome : individual.GetChromosomes())
		{
			sumOfFitness += this->m_fitnessFunction(chromosome.GetValueInInterval(-10, 10)); // de adaugat din param
		}
	});
}

template<size_t size>
inline bool GeneticAlgorithm<size>::IsInInterval(const double number, const double lowerBound, const double upperBound) const
{
	return number > lowerBound && number <= upperBound;
}

template<size_t size>
inline GeneticAlgorithm<size>::GeneticAlgorithm(std::function<double(double)> fitnessFunction, const size_t initialPopulationSize, const size_t numberOfEpochs, const double crossoverProb, const double mutationProb, const size_t chNumber) :
	m_populationSize(initialPopulationSize),
	m_numberOfEpochs(numberOfEpochs),
	m_crossoverProbability(crossoverProb),
	m_mutationProbability(mutationProb),
	m_chromosomeNumberPerIndividual(chNumber)
{
	m_fitnessFunction = fitnessFunction;
	InitalizePopulation();
}

template<size_t size>
inline std::vector<Individual<size>>& GeneticAlgorithm<size>::GetIndividuals()
{
	return this->m_individuals;
}

template<size_t size>
inline double GeneticAlgorithm<size>::GetResult(const double number)const
{
	return this->m_fitnessFunction(number);
}
