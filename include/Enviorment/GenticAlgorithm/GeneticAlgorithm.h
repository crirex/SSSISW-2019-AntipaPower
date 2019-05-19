#pragma once

#include <functional>

#include "Individual.h"

template <size_t size>
class GeneticAlgorithm
{
private:
	void initalizePopulation();
	
public:
	GeneticAlgorithm(std::function<double(double)> fitnessFunction,const size_t initialPopulationSize = 10, const size_t numberOfEpochs = 300, const double crossoverProb = 0.3, const double mutationProb = 0.3, const size_t chNumber = 1);
	~GeneticAlgorithm() = default;

	std::vector<Individual<size>>& getIndividuals();
	const double getResult(const double number) const;


	std::vector<Individual<size>> Selection();
private:
	std::vector<Individual<size>> m_individuals;
	double m_crossoverProbability;
	double m_mutationProbability;
	size_t m_numberOfEpochs;
	size_t m_initialPopulationSize;
	size_t m_chromosomeNumberPerIndividual;
	std::function<double(double)> m_fitnessFunction;
};

template<size_t size>
inline void GeneticAlgorithm<size>::initalizePopulation()
{
	for (size_t index = 0; index < m_initialPopulationSize; index++)
	{
		m_individuals.emplace_back(Individual<size>(m_chromosomeNumberPerIndividual));
	}
}

template<size_t size>
inline std::vector<Individual<size>> GeneticAlgorithm<size>::Selection()
{
	double sumOfFitness = 0;
	std::for_each(this->m_individuals.begin(), this->m_individuals.end(), [&](Individual<size>& individual)
	{
		for (Chromosome<size>& chromosome : individual.getChromosomes())
		{
			sumOfFitness += this->m_fitnessFunction(chromosome.getValueInInterval(-10, 10)); // de adaugat din param
		}
	});

	std::vector<double> probabilityOfSelection{};

	std::for_each(this->m_individuals.begin(), this->m_individuals.end(), [&](Individual<size>& individual)
	{
		//de calculat probabiliattea de selectie pentru fiecare individ - > devazut modul in care se selecteaza indivizii pentru fucntii de 2 variabile
	});

	return this->m_individuals;
}

template<size_t size>
inline GeneticAlgorithm<size>::GeneticAlgorithm(std::function<double(double)> fitnessFunction,const size_t initialPopulationSize, const size_t numberOfEpochs, const double crossoverProb, const double mutationProb, const size_t chNumber) :
	m_initialPopulationSize(initialPopulationSize), 
	m_numberOfEpochs(numberOfEpochs),
	m_crossoverProbability(crossoverProb),
	m_mutationProbability(mutationProb),
	m_chromosomeNumberPerIndividual(chNumber)
{
	m_fitnessFunction = fitnessFunction;
	initalizePopulation();
}

template<size_t size>
inline std::vector<Individual<size>>& GeneticAlgorithm<size>::getIndividuals()
{
	return this->m_individuals;
}

template<size_t size>
inline const double GeneticAlgorithm<size>::getResult(const double number)const
{
	return this->m_fitnessFunction(number);
}
