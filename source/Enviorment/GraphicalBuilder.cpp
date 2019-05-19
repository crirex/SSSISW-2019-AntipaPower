#include "Enviorment/GraphicalBuilder.h"

GraphicalBuilder::GraphicalBuilder(const std::vector<std::shared_ptr<Services::IBuiltObjectService>>& services)
{
	std::for_each(services.begin(), services.end(), [this](auto service) {
		this->m_services[service->GetHashCode()] = service;
		});
}

void GraphicalBuilder::Build(std::shared_ptr<chrono::ChSystem> system, std::shared_ptr<GraphicalObjects::IGraphicalObject> graphicalObject)
{
	graphicalObject->Build();
	CallServices(std::make_shared<GraphicalObjects::GraphicalContext>(system, graphicalObject));
}

void GraphicalBuilder::CallServices(std::shared_ptr<GraphicalObjects::GraphicalContext> context) const
{
	std::for_each(this->m_services.begin(), this->m_services.end(), [&](const auto & service) {
		service.second->OnBuiltObject(context);
		});
}