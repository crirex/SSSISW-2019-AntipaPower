#pragma once
#include <bitset>
#include <random>
#include <math.h>

#include "RandomGenerator.h"

static const int kDefaultNumberOfGenes = 8;

template <size_t size = kDefaultNumberOfGenes>
class Chromosome
{
private:
	void GenerateRandomChromosome();
public:
	Chromosome();
	~Chromosome() = default;

	unsigned long GetDecimalValue() const;
	std::bitset<size>& GetGenes();	
	double GetValueInInterval(int lowerBound, int upperBound) const;
private:
	std::bitset<size> m_genes;
};

template<size_t size>
inline Chromosome<size>::Chromosome()
{
	GenerateRandomChromosome();
}

template<size_t size>
inline unsigned long Chromosome<size>::GetDecimalValue() const
{
	return this->m_genes.to_ulong();
}

template<size_t size>
inline std::bitset<size>& Chromosome<size>::GetGenes()
{
	return this->m_genes;
}

template<size_t size>
inline void Chromosome<size>::GenerateRandomChromosome()
{
	const int kLowerBound = 0, kUpperBound = 1;
	for (size_t index = 0; index < this->m_genes.size(); ++index)
	{
		this->m_genes[index] = RandomGenerator::GetIntegerInRange(kLowerBound, kUpperBound);
	}
}

template<size_t size>
inline double Chromosome<size>::GetValueInInterval(int lowerBound, int upperBound) const
{
	return lowerBound + this->m_genes.to_ulong() * ((upperBound - lowerBound) / (pow(2, size)));
}