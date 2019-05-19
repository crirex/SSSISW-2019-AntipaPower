#include "Enviorment/Services/ExportService.h"

std::string Services::ExportService::GetHashCode() const
{
	return typeid(*this).name();
}

void Services::ExportService::OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context)
{

}
