#pragma once

#include <vector>

#include "Chromosome.h"

template <size_t size = kDefaultNumberOfGenes>
class Individual
{
public:
	Individual(const int numberOfChromosomes );
	~Individual() = default;

	void printChromosomes();
	std::vector<Chromosome<size>>& getChromosomes();
private:
	std::vector<Chromosome<size>> m_chromosomes;
};

template<size_t size>
inline Individual<size>::Individual(const int numberOfChromosomes)
{
	for (size_t index = 0; index < numberOfChromosomes; ++index)
	{
		m_chromosomes.emplace_back(Chromosome<size>());
	}
}

template<size_t size>
inline void Individual<size>::printChromosomes()
{
	for (auto& ch : this->m_chromosomes)
	{
		std::cout << ch.getGenes() << "\n";
	}
}

template<size_t size>
inline std::vector<Chromosome<size>>& Individual<size>::getChromosomes()
{
	return this->m_chromosomes;
}
