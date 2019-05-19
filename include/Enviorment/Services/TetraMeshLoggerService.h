#pragma once

#include "chrono/fea/ChElementTetra_4.h"

#include "IBuiltObjectService.h"

namespace Services
{
	class TetraMeshLoggerService :
		public IBuiltObjectService
	{
	public:
		// Inherited via IBuiltObjectService
		virtual std::string GetHashCode() const override;
		virtual void OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context) override;
	private:
		std::mutex m_mutex;
	};
}

