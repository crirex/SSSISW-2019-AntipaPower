#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <chrono>

#include "Enviorment/GraphicalObjects/GraphicalContext.h"

namespace Services
{
	class IBuiltObjectService
	{
	public:
		virtual std::string GetHashCode() const = 0;
		virtual void OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context) = 0;
		virtual ~IBuiltObjectService() = default;
	};
}