#pragma once

#include <vector>

#include "Chromosome.h"

template <size_t size = kDefaultNumberOfGenes>
class Individual
{
public:
	Individual(size_t numberOfChromosomes);
	~Individual() = default;

	void PrintChromosomes();
	std::vector<Chromosome<size>>& GetChromosomes();
private:
	std::vector<Chromosome<size>> m_chromosomes;
};

template<size_t size>
inline Individual<size>::Individual(size_t numberOfChromosomes)
{
	for (size_t index = 0; index < numberOfChromosomes; ++index)
	{
		m_chromosomes.emplace_back(Chromosome<size>());
	}
}

template<size_t size>
inline void Individual<size>::PrintChromosomes()
{
	for (auto& ch : this->m_chromosomes)
	{
		std::cout << ch.GetGenes() << "\n";
	}
}

template<size_t size>
inline std::vector<Chromosome<size>>& Individual<size>::GetChromosomes()
{
	return this->m_chromosomes;
}
