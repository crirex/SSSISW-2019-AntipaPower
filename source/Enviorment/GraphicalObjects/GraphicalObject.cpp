#include "Enviorment/GraphicalObjects//GraphicalObject.h"

std::vector<std::shared_ptr<chrono::ChPhysicsItem>> GraphicalObjects::GraphicalObject::GetPhysicsItems() const
{
	return this->m_physicsItems;
}
