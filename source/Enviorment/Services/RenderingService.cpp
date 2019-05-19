#include "Enviorment/Services/RenderingService.h"

std::string Services::RenderingService::GetHashCode() const
{
	return typeid(*this).name();
}

void Services::RenderingService::OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context)
{
	auto items = context->GetGraphicalObject()->GetPhysicsItems();
	std::for_each(items.begin(), items.end(), [&](auto item) {
		context->GetSystem()->Add(item);
		});
}
