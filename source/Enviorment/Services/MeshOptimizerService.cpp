#include "Enviorment/Services/MeshOptimizerService.h"

std::string Services::MeshOptimizerService::GetHashCode() const
{
	return typeid(*this).name();
}

void Services::MeshOptimizerService::OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context)
{
	auto mesh = context->GetGraphicalObject()->GetMesh();
	std::thread([&, mesh]() {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		auto optimizedMeh = OptimizeMesh(mesh);
		mesh->ClearElements();
		for each (auto element in optimizedMeh->GetElements())
		{
			mesh->AddElement(element);
		}
		}).detach();
}

std::shared_ptr<chrono::fea::ChMesh> Services::MeshOptimizerService::OptimizeMesh(std::shared_ptr<chrono::fea::ChMesh> mesh) const
{
	auto clone = std::make_shared<chrono::fea::ChMesh>(*mesh->Clone());
	clone->ClearElements();
	auto elements = mesh->GetElements();
	auto minmaxTetra = std::minmax_element(elements.begin(), elements.end(), [](auto first,
		auto second) {
			auto firstStrain = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(first)->GetStrain();
			auto secondStrain = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(second)->GetStrain();
			return firstStrain.GetEquivalentVonMises() > secondStrain.GetEquivalentVonMises();
		});

	for each (auto element in elements)
	{
		if (element.get() != (*minmaxTetra.first).get() &&
			element.get() != (*minmaxTetra.second).get())
		{
			clone->AddElement(element);
		}
	}
	return clone;
}
