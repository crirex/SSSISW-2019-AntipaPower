#pragma once

#include "IBuiltObjectService.h"

namespace Services
{
	class RenderingService :
		public IBuiltObjectService
	{
	public:
		// Inherited via IBuiltObjectService
		std::string GetHashCode() const override;
		void OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context) override;
	};
}