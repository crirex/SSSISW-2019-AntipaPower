#include "Enviorment/GraphicalObjects/GraphicalContext.h"


GraphicalObjects::GraphicalContext::GraphicalContext(std::shared_ptr<chrono::ChSystem> system, std::shared_ptr<GraphicalObjects::IGraphicalObject> graphicalObject) :
	m_system(system), m_graphicalObject(graphicalObject)
{
}

std::shared_ptr<chrono::ChSystem> GraphicalObjects::GraphicalContext::GetSystem() const
{
	return this->m_system;
}

std::shared_ptr<GraphicalObjects::IGraphicalObject> GraphicalObjects::GraphicalContext::GetGraphicalObject() const
{
	return this->m_graphicalObject;
}
