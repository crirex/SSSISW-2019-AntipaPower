#include "Cable.h"

Cable::Cable(chrono::ChSystemNSC& system):
	m_refSystem(system)
{
	this->m_mesh = std::make_shared<chrono::fea::ChMesh>();
	this->m_base = std::make_shared<chrono::ChBody>();
	
	this->m_refSystem.Add(this->m_mesh);
	this->m_refSystem.Add(this->m_base);
	InitializeSectionCable();
}

const std::shared_ptr<chrono::fea::ChMesh>& Cable::GetMesh() const
{
	return this->m_mesh;
}

void Cable::SetVisualtizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualtiozationMesh)
{
	this->m_mesh->AddAsset(visualtiozationMesh);
}

void Cable::BuildCable(Beam& beam)
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes
	{
		std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(*(beam.GetMesh()->GetNodes().end()-1)),
		std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(*(beam.GetMesh()->GetNodes().end()-4))
	};

	ConstructBase(nodes);

	for (const auto& node : nodes)
	{
		auto constraintOverBean = std::make_shared<chrono::fea::ChLinkPointFrame>();
		this->m_builder.BuildBeam(this->m_mesh, this->m_sectionCable, 1, node->GetPos(), node->GetPos() + chrono::VECT_Y * -2);
		constraintOverBean->Initialize(this->m_builder.GetLastBeamNodes().front(), this->m_base);
		this->m_refSystem.Add(constraintOverBean);

		auto mbox = std::make_shared<chrono::ChBodyEasyBox>(1, 1, 1 , 1000);
		mbox->SetPos(m_builder.GetLastBeamNodes().back()->GetPos() + chrono::ChVector<>(0.1, 0, 0));
		this->m_refSystem.Add(mbox);

		auto constraint_pos = std::make_shared<chrono::fea::ChLinkPointFrame>();
		constraint_pos->Initialize(this->m_builder.GetLastBeamNodes().back(), mbox);
		this->m_refSystem.Add(constraint_pos);
	}
}

void Cable::InitializeSectionCable()
{
	this->m_sectionCable = std::make_shared<chrono::fea::ChBeamSectionCable>();
	this->m_sectionCable->SetDiameter(0.15);
	this->m_sectionCable->SetYoungModulus(0.01e9);
	this->m_sectionCable->SetBeamRaleyghDamping(0.000);
}

void Cable::ConstructBase(std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes)
{
	for (const auto& node : nodes)
	{
		auto constraint = std::make_shared<chrono::fea::ChLinkPointFrame>();
		constraint->Initialize(node, this->m_base);
		this->m_refSystem.Add(constraint);
	}
}
