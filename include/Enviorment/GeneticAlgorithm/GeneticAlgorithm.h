#pragma once

#include <functional>
#include <numeric>
#include <fstream>
#include "Individual.h"
#include "Chromosome.h"

template <size_t size = kDefaultNumberOfGenes>
class GeneticAlgorithm
{
private:
	void InitalizePopulation();
	std::vector<double> CalculateCumulativeProbability(const std::vector<double> & probabilityOfSelection);
	std::vector<double> CalculateProbabilityOfSelection(double sumOfFitness);
	double CalculateSumOfFitnessFunction();
	void PrintIndividuals(std::ostream& out, Individual<size>& bestIndividual);
	bool IsInInterval(double number, double lowerBound, double upperBound) const;
	void ChangeGenes(Individual<size> & first, Individual<size> & second, size_t randomSize);
public:
	GeneticAlgorithm(std::function<double(Individual<size>)> fitnessFunction,
		size_t initialPopulationSize = 10,
		size_t numberOfEpochs = 300,
		double crossoverProb = 0.1,
		double mutationProb = 0.01,
		size_t chNumber = 1,
		bool minimization = true);
	~GeneticAlgorithm() = default;

	std::vector<Individual<size>>& GetIndividuals();
	double GetResult(double number) const;

	void Selection();
	void Crossover();
	void Mutation();
	void Fit(const std::string& filename = "output.txt");
private:
	std::vector<Individual<size>> m_individuals;
	double m_crossoverProbability;
	double m_mutationProbability;
	size_t m_numberOfEpochs;
	size_t m_populationSize;
	size_t m_chromosomeNumberPerIndividual;
	bool m_minimization;
	std::function<double(Individual<size>)> m_fitnessFunction;
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

	for (size_t index = 0; index < this->m_individuals.size() - static_cast<int>(m_minimization); index++)
	{
		if (IsInInterval(randomNumbers[index], 0, cumulativeProbabilityVector[index]))
		{
			newPopulation.emplace_back(this->m_individuals[index + static_cast<int>(m_minimization)]);
		}
		else
		{
			if (m_minimization)
			{
				newPopulation.emplace_back(this->m_individuals[index]);
			}
			else
			{
				for (size_t index2 = 1; index2 < this->m_individuals.size() - 1; index2++)
				{
					if (IsInInterval(randomNumbers[index], cumulativeProbabilityVector[index2], cumulativeProbabilityVector[index2 + 1]))
					{
						newPopulation.emplace_back(this->m_individuals[index2 + 1]);
						break;
					}
				}

			}
		}
	}
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
inline void GeneticAlgorithm<size>::Fit(const std::string& filename)
{
	std::ofstream outputFile;
	outputFile.open(filename, std::ios::out | std::ofstream::trunc);
	std::vector<std::vector<Individual<size>>> allIndividualGenerations;
	Individual<size> bestIndividual = m_individuals.front();

	outputFile << "#start\n";
	outputFile << "#generationNo: 0\n";
	PrintIndividuals(outputFile, bestIndividual);
	for (size_t index = 0; index < this->m_numberOfEpochs && this->m_individuals.size() > 1; ++index)
	{
		Selection();
		Crossover();
		Mutation();
		allIndividualGenerations.push_back(this->m_individuals);

		outputFile << "#generationNo: " << index + 1 << "\n";
		PrintIndividuals(outputFile, bestIndividual);
	}

	std::string fullChromozome = "";
	for (auto& ch : bestIndividual.GetChromosomes())
	{
		fullChromozome += ch.GetGenes().to_string<char, std::string::traits_type, std::string::allocator_type>();
	}

	outputFile << "Maximum/Minimum chromosome: " << fullChromozome << "\n";
	outputFile << "Maximum/Minimum value: " << this->m_fitnessFunction(bestIndividual) << "\n";
	outputFile << "#end";
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
		probabilityOfSelection.emplace_back(exp(this->m_fitnessFunction(individual)) / sumOfFitness);
	});
	return probabilityOfSelection;
}

template<size_t size>
inline double GeneticAlgorithm<size>::CalculateSumOfFitnessFunction()
{
	double sumOfFitness = 0;
	std::for_each(this->m_individuals.begin(), this->m_individuals.end(), [&](Individual<size> & individual)
	{
		sumOfFitness += exp(this->m_fitnessFunction(individual));
	});
	return sumOfFitness;
}

template<size_t size>
inline void GeneticAlgorithm<size>::PrintIndividuals(std::ostream & out, Individual<size>& bestIndividual)
{
	std::string fullChromozome = "";
	double fitnessChromozome;
	for (auto& individ : m_individuals)
	{
		for (auto& ch : individ.GetChromosomes())
		{
			fullChromozome += ch.GetGenes().to_string<char, std::string::traits_type, std::string::allocator_type>();
		}
		fitnessChromozome = this->m_fitnessFunction(individ);

		if (fitnessChromozome > this->m_fitnessFunction(bestIndividual))
		{
			bestIndividual = individ;
		}

		out << "Chromosome " << fullChromozome << "\n";
		out << "Value " << fitnessChromozome << "\n";
		fullChromozome = "";
	}
}

template<size_t size>
inline bool GeneticAlgorithm<size>::IsInInterval(double number, double lowerBound, double upperBound) const
{
	return number > lowerBound && number <= upperBound;
}

template<size_t size>
inline void GeneticAlgorithm<size>::ChangeGenes(Individual<size> & first, Individual<size> & second, size_t randomSize)
{
	for (int indexChromosomes = 0; indexChromosomes < first.GetChromosomes().size(); ++indexChromosomes)
	{
		auto & firstGenes = first.GetChromosomes()[indexChromosomes].GetGenes();
		auto & secondGenes = second.GetChromosomes()[indexChromosomes].GetGenes();

		auto temp = firstGenes;

		for (size_t indexGenes = randomSize; indexGenes < size; ++indexGenes)
		{
			firstGenes[indexGenes] = secondGenes[indexGenes];
			secondGenes[indexGenes] = temp[indexGenes];
		}
	}
}

template<size_t size>
inline GeneticAlgorithm<size>::GeneticAlgorithm(std::function<double(Individual<size>)> fitnessFunction,
	size_t initialPopulationSize,
	size_t numberOfEpochs,
	double crossoverProb,
	double mutationProb,
	size_t chNumber,
	bool minimization) :
	m_populationSize(initialPopulationSize),
	m_numberOfEpochs(numberOfEpochs),
	m_crossoverProbability(crossoverProb),
	m_mutationProbability(mutationProb),
	m_chromosomeNumberPerIndividual(chNumber),
	m_minimization(minimization)
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
inline double GeneticAlgorithm<size>::GetResult(double number) const
{
	return this->m_fitnessFunction(number);
}
