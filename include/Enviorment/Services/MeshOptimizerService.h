#pragma once

#include <functional>

#include "chrono/fea/ChMesh.h"
#include "chrono/fea/ChElementTetra_4.h"

#include "IBuiltObjectService.h"
#include "Enviorment/GeneticAlgorithm/GeneticAlgorithm.h"


namespace Services
{
	class MeshOptimizerService :
		public IBuiltObjectService
	{
	public:
		// Inherited via IBuiltObjectService
		virtual std::string GetHashCode() const override;
		virtual void OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context) override;

	private:
		double StandardDeviation(Individual<> individual);

	public:
		std::shared_ptr<chrono::fea::ChMesh> OptimizeMesh(std::shared_ptr<chrono::fea::ChMesh> mesh);

	private:
		std::vector<double> m_strainData;
	};
}


