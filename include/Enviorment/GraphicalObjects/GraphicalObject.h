#pragma once

#include "IGraphicalObject.h"

namespace GraphicalObjects
{
	class GraphicalObject :
		public IGraphicalObject
	{
	public:
		// Inherited via IGraphicalObject
		std::vector<std::shared_ptr<chrono::ChPhysicsItem>> GetPhysicsItems() const override;

	protected:
		std::vector<std::shared_ptr<chrono::ChPhysicsItem>> m_physicsItems;
	};
}


