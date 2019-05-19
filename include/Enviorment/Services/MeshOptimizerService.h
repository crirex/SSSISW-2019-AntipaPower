#pragma once

#include "chrono/fea/ChMesh.h"
#include "chrono/fea/ChElementTetra_4.h"

#include "IBuiltObjectService.h"


namespace Services
{
	class MeshOptimizerService :
		public IBuiltObjectService
	{
	public:
		// Inherited via IBuiltObjectService
		virtual std::string GetHashCode() const override;
		virtual void OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context) override;
	public:
		std::shared_ptr<chrono::fea::ChMesh> OptimizeMesh(std::shared_ptr<chrono::fea::ChMesh> mesh) const;
	};
}


