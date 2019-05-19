#pragma once

#include <map>
#include <vector>
#include <algorithm>

#include "Enviorment/Services/IBuiltObjectService.h"
#include "Enviorment/GraphicalObjects/IGraphicalObject.h"


class GraphicalBuilder
{
public:
	GraphicalBuilder(const std::vector<std::shared_ptr<Services::IBuiltObjectService>>& services);
	~GraphicalBuilder() = default;

public:
	template<typename T>
	std::shared_ptr<T> GetService();

	void Build(std::shared_ptr<chrono::ChSystem> system, std::shared_ptr<GraphicalObjects::IGraphicalObject> graphicalObject);

private:
	void CallServices(std::shared_ptr<GraphicalObjects::GraphicalContext> context) const;

private:
	std::map<std::string, std::shared_ptr<Services::IBuiltObjectService>> m_services;
};

template<typename T>
inline std::shared_ptr<T> GraphicalBuilder::GetService()
{
	static_assert(std::is_base_of<Services::IBuiltObjectService, T>::value, "Specified template parameter must be of type IBuiltObjectService.");
	return std::dynamic_pointer_cast<T>(this->m_services[typeid(T).name()]);
}
