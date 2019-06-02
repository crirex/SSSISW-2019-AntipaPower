#pragma once

#include <functional>

#include "chrono/fea/ChMesh.h"
#include "chrono/fea/ChElementTetra_4.h"

#include "IBuiltObjectService.h"
#include "RenderingService.h"
#include "Enviorment/GraphicalBuilder.h"
#include "Enviorment/GraphicalObjects/Cuboid.h"
#include "Enviorment/GraphicalObjects/Configurations.h"
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
		std::shared_ptr<chrono::fea::ChMesh> OptimizeMesh(std::shared_ptr<GraphicalObjects::GraphicalContext> context);

		void CalculateStarin(const std::vector<std::shared_ptr<chrono::fea::ChElementBase>> & elements);

	private:
		std::vector<double> m_strainData;
	};
}


