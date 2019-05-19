#include "Enviorment\Services\TetraMeshLoggerService.h"

std::string Services::TetraMeshLoggerService::GetHashCode() const
{
	return typeid(*this).name();
}

void Services::TetraMeshLoggerService::OnBuiltObject(std::shared_ptr<GraphicalObjects::GraphicalContext> context)
{
	auto mesh = context->GetGraphicalObject()->GetMesh();
	std::thread([&, mesh]() {
		while (true)
		{
			std::lock_guard<std::mutex> lock(this->m_mutex);
			std::ofstream out;
			out.open("node_info.txt", std::ios::out | std::ios::trunc);
			for (unsigned int indexElement = 0; indexElement < mesh->GetNelements(); indexElement++)
			{
				if (auto element = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(mesh->GetElement(indexElement)))
				{
					auto StrainV = element->GetStrain();
					out << "Strain On Thetra No: " << indexElement << ", X: " << StrainV.XX() << " , Y: " << StrainV.YY() << ", Z: " << StrainV.ZZ() << std::endl;
					for (size_t nodeIndex = 0; nodeIndex < element->GetNnodes(); ++nodeIndex)
					{
						auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(element->GetNodeN(static_cast<int>(nodeIndex)));
						out << "Node: " << node->GetIndex() << ", Pos X : " << node->GetPos().x() << ", Y : " << node->GetPos().y() << ", Z : " << node->GetPos().z() << std::endl;
					}
				}
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		}).detach();
}
