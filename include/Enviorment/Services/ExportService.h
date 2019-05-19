#pragma once

#include "Enviorment/Services/IBuiltObjectService.h"

namespace Services
{
	class ExportService :
		public IBuiltObjectService
	{
	public:
		// Inherited via IBuiltObjectService
		std::string GetHashCode() const override;
		void OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context) override;
	};
}


