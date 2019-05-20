#pragma once

#include <functional>
#include <numeric>

#include "Individual.h"

template <size_t size = kDefaultNumberOfGenes>
class GeneticAlgorithm
{
private:
	void InitalizePopulation();
	std::vector<double> CalculateCumulativeProbability(const std::vector<double> & probabilityOfSelection);
	std::vector<double> CalculateProbabilityOfSelection(double sumOfFitness);
	double CalculateSumOfFitnessFunction();
	bool IsInInterval(double number, double lowerBound, double upperBound) const;
	void ChangeGenes(Individual<size> & first, Individual<size> & second, size_t randomSize);
public:
	GeneticAlgorithm(std::function<double(double)> fitnessFunction, const size_t initialPopulationSize = 200, const size_t numberOfEpochs = 300, const double crossoverProb = 0.1, const double mutationProb = 0.01, const size_t chNumber = 1);
	~GeneticAlgorithm() = default;

	std::vector<Individual<size>>& GetIndividuals();
	double GetResult(const double number) const;

	void Selection();
	void Crossover();
	void Mutation();
	void Fit();
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
inline void GeneticAlgorithm<size>::Selection()
{
	std::vector<double> randomNumbers = RandomGenerator::GenerateRealNumbers(0, 1, this->m_individuals.size());
	std::vector<Individual<size>> newPopulation{};

	double sumOfFitness = CalculateSumOfFitnessFunction();
	auto probabilityOfSelection = CalculateProbabilityOfSelection(sumOfFitness);
	auto cumulativeProbabilityVector = CalculateCumulativeProbability(probabilityOfSelection);

	for (size_t index = 0; index < this->m_individuals.size() - 1; index++)
	{
		if (IsInInterval(randomNumbers[index], 0, cumulativeProbabilityVector[index]))
		{
			// delete + 1 for maximization
			newPopulation.emplace_back(this->m_individuals[index + 1]);
		}
		else
		{
			newPopulation.emplace_back(this->m_individuals[index]);

			//this is for maximization
			/*for (size_t index2 = 1; index2 < this->m_individuals.size() - 1; index2++)
			{
				if (IsInInterval(randomNumbers[index], cumulativeProbabilityVector[index2], cumulativeProbabilityVector[index2 + 1]))
				{
					newPopulation.emplace_back(this->m_individuals[index2 + 1]);
					break;
				}
			}*/
		}
	}

	//std::cout << std::accumulate(probabilityOfSelection.begin(), probabilityOfSelection.end(), 0.0) << std::endl;
	this->m_individuals = newPopulation;
}

template<size_t size>
inline void GeneticAlgorithm<size>::Crossover()
{
	std::vector<double> randomNumbers = RandomGenerator::GenerateRealNumbers(0, 1, this->m_individuals.size());
	std::vector<Individual<size>> selectedPopulationForCrossover{};

	for (size_t index = 0; index < this->m_individuals.size(); ++index)
	{
		if (randomNumbers[index] <= this->m_crossoverProbability)
		{
			selectedPopulationForCrossover.emplace_back(this->m_individuals[index]);
		}
	}

	if (selectedPopulationForCrossover.size() % 2 != 0)
	{
		selectedPopulationForCrossover.pop_back();
	}

	if (selectedPopulationForCrossover.empty())
	{
		return;
	}

	size_t randomSize = RandomGenerator::GetIntegerInRange(1, size - 1);

	for (size_t index = 0; index < selectedPopulationForCrossover.size() - 1; index += 2)
	{
		Individual<size>& first = selectedPopulationForCrossover[index];
		Individual<size>& second = selectedPopulationForCrossover[index + 1];

		ChangeGenes(first, second, randomSize);
	}

	this->m_individuals = selectedPopulationForCrossover;
}

template<size_t size>
inline void GeneticAlgorithm<size>::Mutation()
{
	for (auto& individ : this->m_individuals)
	{
		for (auto& ch : individ.GetChromosomes())
		{
			auto& genes = ch.GetGenes();
			for (size_t index = 0; index < size; index++)
			{
				if (RandomGenerator::GetRealInRange(0, 1) <= this->m_mutationProbability)
				{
					genes[index] = 1 - genes[index];
				}
			}
		}
	}
}

template<size_t size>
inline void GeneticAlgorithm<size>::Fit()
{
	for (size_t index = 0; index < this->m_numberOfEpochs && this->m_individuals.size() > 1; ++index)
	{
		std::cout << "Generatia: " << index << "\n";
		Selection();
		Crossover();
		Mutation();
	}

	double decimalValue = this->m_individuals[0].GetChromosomes().front().GetValueInInterval(-10, 10);
	std::cout << "x = " << decimalValue << "\nf(x)= " << this->m_fitnessFunction(decimalValue) << std::endl;
}

template<size_t size>
inline std::vector<double> GeneticAlgorithm<size>::CalculateCumulativeProbability(const std::vector<double> & probabilityOfSelection)
{
	std::vector<double> cumulativeProbabilyVector;
	for (size_t index1 = 0; index1 < this->m_individuals.size(); ++index1)
	{
		double cumulativeProbability{};
		for (size_t index2 = 0; index2 < index1 + 1; ++index2)
		{
			cumulativeProbability += probabilityOfSelection[index2];
		}
		cumulativeProbabilyVector.emplace_back(cumulativeProbability);
	}
	return cumulativeProbabilyVector;
}

template<size_t size>
inline std::vector<double> GeneticAlgorithm<size>::CalculateProbabilityOfSelection(double sumOfFitness)
{
	std::vector<double> probabilityOfSelection;
	std::for_each(this->m_individuals.begin(), this->m_individuals.end(), [&](Individual<size>& individual)
		{
			for (Chromosome<size>& chromosome : individual.GetChromosomes())
			{
				probabilityOfSelection.emplace_back(exp(this->m_fitnessFunction(chromosome.GetValueInInterval(-10, 10))) / sumOfFitness); // de adaugat din param
			}
		});
	return probabilityOfSelection;
}

template<size_t size>
inline double GeneticAlgorithm<size>::CalculateSumOfFitnessFunction()
{
	double sumOfFitness = 0;
	std::for_each(this->m_individuals.begin(), this->m_individuals.end(), [&](Individual<size> & individual)
		{
			for (Chromosome<size>& chromosome : individual.GetChromosomes())
			{
				sumOfFitness += this->m_fitnessFunction(exp(chromosome.GetValueInInterval(-10, 10))); // de adaugat din param
			}
		});
	return sumOfFitness;
}

template<size_t size>
inline bool GeneticAlgorithm<size>::IsInInterval(double number, double lowerBound, double upperBound) const
{
	return number > lowerBound && number <= upperBound;
}

template<size_t size>
inline void GeneticAlgorithm<size>::ChangeGenes(Individual<size> & first, Individual<size> & second, size_t randomSize)
{
	auto & firstGenes = first.GetChromosomes().front().GetGenes();
	auto & secondGenes = second.GetChromosomes().front().GetGenes();

	auto temp = firstGenes;

	for (size_t index = randomSize; index < size; ++index)
	{
		firstGenes[index] = secondGenes[index];
		secondGenes[index] = temp[index];
	}

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
inline std::vector<Individual<size>> & GeneticAlgorithm<size>::GetIndividuals()
{
	return this->m_individuals;
}

template<size_t size>
inline double GeneticAlgorithm<size>::GetResult(const double number) const
{
	return this->m_fitnessFunction(number);
}
