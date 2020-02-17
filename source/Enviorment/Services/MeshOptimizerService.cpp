#include "Enviorment/Services/MeshOptimizerService.h"

//Services::MeshOptimizerService::number_of_thetra;

std::string Services::MeshOptimizerService::GetHashCode() const
{
	return typeid(*this).name();
}

void Services::MeshOptimizerService::OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context)
{
	auto mesh = context->GetGraphicalObject()->GetMesh();
	auto clone = std::make_shared<chrono::fea::ChMesh>(*mesh->Clone());
	std::thread([&, mesh, context]() {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		auto optimizedMeh = OptimizeMesh(context);
		mesh->ClearElements();
		for each (auto element in optimizedMeh->GetElements())
		{
			mesh->AddElement(element);
		}
	}).detach();


	GraphicalBuilder graphicalBuilder({
		std::make_shared<Services::RenderingService>(),
		});


	std::shared_ptr<GraphicalObjects::Cuboid> cuboid = std::make_shared<GraphicalObjects::Cuboid>(chrono::Vector(0.5), chrono::ChVector<int>(3, 3, 10));
	cuboid->SetMesh(clone);
	cuboid->SetMaterial(GraphicalObjects::Configurations::CreateBeamMaterialConfig());
	cuboid->SetVisualizationMesh(GraphicalObjects::Configurations::CreateBeamVisualizationMeshConfig(cuboid->GetMesh()));
	graphicalBuilder.Build(context->GetSystem(), cuboid);

}

double Services::MeshOptimizerService::StandardDeviation(Individual<> individual)
{
	double mean = std::accumulate(this->m_strainData.begin(), this->m_strainData.end(), .0) / this->m_strainData.size();
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

	return std::sqrt(sum / this->m_strainData.size());
}

double Services::MeshOptimizerService::fittnessFunction(Individual<number_of_thetra> individual)
{
	auto mesh = this->m_context->GetGraphicalObject()->GetMesh();
	auto cloneMesh = std::make_shared<chrono::fea::ChMesh>(*mesh->Clone());
	auto elements = mesh->GetElements();
	cloneMesh->ClearElements();

	std::string fullChromozome = "";
	int index = 0;

	for (auto& ch : individual.GetChromosomes())
	{
		fullChromozome += ch.GetGenes().to_string<char, std::string::traits_type, std::string::allocator_type>();
	}
	
	for each (auto element in elements)
	{
		if (fullChromozome[index] == '1')
		{
			if (auto tetra = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(element))
			{
				cloneMesh->AddElement(element);
			}
		}
		++index;
	}

	auto cloneElements = cloneMesh->GetElements();
	CalculateStrain(cloneElements);

	GeneticAlgorithm<> algorithm([&](Individual<> individual) -> double {
		return StandardDeviation(individual);
	}, 100, 10, 0.1, 0.01, this->m_strainData.size(), true);

	algorithm.Fit("output1.test");
	return this->StandardDeviation(algorithm.GetBestIndividual());
}

std::shared_ptr<chrono::fea::ChMesh> Services::MeshOptimizerService::OptimizeMesh(std::shared_ptr<GraphicalObjects::GraphicalContext> context)
{
	this->m_context = context;
	auto mesh = context->GetGraphicalObject()->GetMesh();
	auto elements = mesh->GetElements();
	auto cloneMesh = std::make_shared<chrono::fea::ChMesh>(*mesh->Clone());
	cloneMesh->ClearElements();

	GeneticAlgorithm<number_of_thetra> algorithm([&](Individual<number_of_thetra> individual) -> double 
	{
		return fittnessFunction(individual); 
	}, 100, 10, 0.1, 0.01, 1, true);

	algorithm.Fit("output.test");

	auto individual = algorithm.GetBestIndividual();
	std::string fullChromozome = "";
	for (auto& ch : individual.GetChromosomes())
	{
		fullChromozome += ch.GetGenes().to_string<char, std::string::traits_type, std::string::allocator_type>();
	}

	int index = 0;
	for each (auto element in elements)
	{
		if (fullChromozome[index] == '1')
		{
			if (auto tetra = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(element))
			{
				cloneMesh->AddElement(element);
			}
		}
		++index;
	}
	return cloneMesh;
}

void Services::MeshOptimizerService::CalculateStrain(const std::vector<std::shared_ptr<chrono::fea::ChElementBase>> & elements)
{
	std::for_each(elements.begin(), elements.end(), [this](auto element) {
		if (auto tetra = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(element))
		{
			this->m_strainData.emplace_back(tetra->GetStrain().GetEquivalentVonMises());
		}
	});
}
