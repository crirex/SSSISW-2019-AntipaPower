#pragma once

#include "chrono/physics/ChSystem.h"
#include "Enviorment/GraphicalObjects/IGraphicalObject.h"

namespace GraphicalObjects
{
	class GraphicalContext
	{
	public:
		GraphicalContext(std::shared_ptr<chrono::ChSystem> system, std::shared_ptr<GraphicalObjects::IGraphicalObject> graphicalObject);
		~GraphicalContext() = default;

	public:
		std::shared_ptr<chrono::ChSystem> GetSystem() const;
		std::shared_ptr<GraphicalObjects::IGraphicalObject> GetGraphicalObject() const;

	private:
		std::shared_ptr<chrono::ChSystem> m_system;
		std::shared_ptr<GraphicalObjects::IGraphicalObject> m_graphicalObject;
	};
}