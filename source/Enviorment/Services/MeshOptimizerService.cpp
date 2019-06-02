#include "Enviorment/Services/MeshOptimizerService.h"

std::string Services::MeshOptimizerService::GetHashCode() const
{
	return typeid(*this).name();
}

void Services::MeshOptimizerService::OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context)
{
	auto mesh = context->GetGraphicalObject()->GetMesh();
	std::thread([&, mesh]() {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		auto optimizedMeh = OptimizeMesh(mesh);
		mesh->ClearElements();
		for each (auto element in optimizedMeh->GetElements())
		{
			mesh->AddElement(element);
		}
	}).detach();
}

double Services::MeshOptimizerService::StandardDeviation(Individual<> individual)
{
	double mean = std::accumulate(this->m_strainData.begin(), this->m_strainData.end(), .0) /this->m_strainData.size();
	double sum = 0.0;
	auto[maximInterval, minimInterval] = std::minmax_element(this->m_strainData.begin(), this->m_strainData.end(), [](double firstStrain, double secondStrain) {
		return firstStrain > secondStrain;
	});
	const auto & cromososns = individual.GetChromosomes();

	for (size_t index = 0; index < this->m_strainData.size(); ++index)
	{
		double x = cromososns[index].GetValueInInterval(*minimInterval, *maximInterval);
		sum += std::pow(x - mean, 2);
	}

	return std::sqrt(sum/this->m_strainData.size());
}

std::shared_ptr<chrono::fea::ChMesh> Services::MeshOptimizerService::OptimizeMesh(std::shared_ptr<chrono::fea::ChMesh> mesh)
{
	auto clone = std::make_shared<chrono::fea::ChMesh>(*mesh->Clone());
	clone->ClearElements();
	auto elements = mesh->GetElements();
	std::for_each(elements.begin(), elements.end(), [this](auto element) {
		if (auto tetra = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(element))
		{
			this->m_strainData.emplace_back(tetra->GetStrain().GetEquivalentVonMises());
		}
	});

	GeneticAlgorithm<> algorithm([&](Individual<> individual) -> double {
		return StandardDeviation(individual);
	}, 300, 300, 0.1, 0.01, this->m_strainData.size(), true);

	algorithm.Fit("output.test");

	for each (auto element in elements)
	{
		if (auto tetra = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(element))
		{
			if (tetra->GetStrain().GetEquivalentVonMises() > algorithm.GetResult(algorithm.GetBestIndividual()))
			{
				clone->AddElement(element);
			}
		}
	}
	return clone;
}