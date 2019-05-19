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
	void generatedRandomChromosome();
public:
	Chromosome();
	~Chromosome() = default;

	const unsigned long getDecimalValue() const;
	std::bitset<size>& getGenes() ;	
	const double getValueInInterval(const int lowerBound, const int upperBound) const;
private:
	std::bitset<size> m_genes;
};

template<size_t size>
inline Chromosome<size>::Chromosome()
{
	generatedRandomChromosome();
}

template<size_t size>
inline const unsigned long Chromosome<size>::getDecimalValue() const
{
	return this->m_genes.to_ulong();
}

template<size_t size>
inline std::bitset<size>& Chromosome<size>::getGenes()
{
	return this->m_genes;
}

template<size_t size>
inline void Chromosome<size>::generatedRandomChromosome()
{
	const int kLowerBound = 0, kUpperBound = 1;
	for (size_t index = 0; index < this->m_genes.size(); ++index)
	{
		this->m_genes[index] = RandomGenerator::getIntegerInRange(kLowerBound, kUpperBound);
	}
}

template<size_t size>
inline const double Chromosome<size>::getValueInInterval(const int lowerBound, const int upperBound) const
{
	return lowerBound + this->m_genes.to_ulong() * ((upperBound - lowerBound) / (pow(2, size)));
}
