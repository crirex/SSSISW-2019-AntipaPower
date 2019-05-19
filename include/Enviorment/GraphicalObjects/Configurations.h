#pragma once

#include <memory>

#include "chrono_irrlicht/ChIrrApp.h"
#include "fea/ChVisualizationFEAmesh.h"


namespace GraphicalObjects
{
	class Configurations
	{
	public:
		static std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> CreateBeamVisualizationMeshConfig(std::shared_ptr<chrono::ChPhysicsItem> mesh);
		static std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> CreateCableVisualizationMeshConfigFirst(std::shared_ptr<chrono::ChPhysicsItem> mesh);
		static std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> CreateCableVisualizationMeshConfigSecond(std::shared_ptr<chrono::ChPhysicsItem> mesh);
		static std::shared_ptr<chrono::fea::ChContinuumPlasticVonMises> CreateBeamMaterialConfig();
		static std::shared_ptr<chrono::irrlicht::ChIrrApp> CreateAppConfig(std::shared_ptr<chrono::ChSystem> system);
	};
}