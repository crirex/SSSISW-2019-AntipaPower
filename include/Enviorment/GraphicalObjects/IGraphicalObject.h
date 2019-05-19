#pragma once

#include <vector>

#include "chrono/fea/ChMesh.h"
#include "chrono/physics/ChPhysicsItem.h"

namespace GraphicalObjects
{
	class IGraphicalObject
	{
	public:
		virtual void Build() = 0;
		virtual std::shared_ptr<chrono::fea::ChMesh> GetMesh() const = 0;
		virtual std::vector<std::shared_ptr<chrono::ChPhysicsItem>> GetPhysicsItems() const = 0;
		
		virtual ~IGraphicalObject() = default;
	};
}
