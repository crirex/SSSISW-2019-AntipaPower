#include "Cable.h"

Cable::Cable(chrono::ChSystemNSC& system) :
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

void Cable::ConstructCableFromBeam(Beam& beam)
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes
	{
		std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(*(beam.GetMesh()->GetNodes().end() - 1)),
		std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(*(beam.GetMesh()->GetNodes().end() - 4))
	};

	ConstructBase(nodes);
	nodes.emplace_back(std::make_shared<chrono::fea::ChNodeFEAxyz>
		(chrono::ChVector<>(nodes[0]->GetPos().x(), -2, (nodes[0]->GetPos().z() + nodes[1]->GetPos().z()) / 2)));

	ConstructCables(nodes);
	ConstructWeight();
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

void Cable::ConstructWeight()
{
	auto box = std::make_shared<chrono::ChBodyEasyBox>(1, 1, 1, 10000);
	box->SetPos(m_builder.GetLastBeamNodes().back()->GetPos() + chrono::ChVector<>(0.1, 0, 0));
	this->m_refSystem.Add(box);

	auto constraintPos = std::make_shared<chrono::fea::ChLinkPointFrame>();
	constraintPos->Initialize(this->m_builder.GetLastBeamNodes().back(), box);
	this->m_refSystem.Add(constraintPos);
}

void Cable::ConstructCables(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes)
{
	///First cable
	this->m_builder.BuildBeam(this->m_mesh, this->m_sectionCable, 1, nodes[0]->GetPos(), nodes[2]->GetPos() /*+ chrono::VECT_Y * -2*/);
	auto constraintOverFirstNode = std::make_shared<chrono::fea::ChLinkPointFrame>();
	constraintOverFirstNode->Initialize(this->m_builder.GetLastBeamNodes().front(), this->m_base);
	this->m_refSystem.Add(constraintOverFirstNode);

	auto constraintOverJuncture1 = std::make_shared<chrono::fea::ChLinkPointFrame>();
	constraintOverJuncture1->Initialize(this->m_builder.GetLastBeamNodes().back(), this->m_base);
	this->m_refSystem.Add(constraintOverJuncture1);
	///

	///Second Cable
	this->m_builder.BuildBeam(this->m_mesh, this->m_sectionCable, 1, nodes[1]->GetPos(), nodes[2]->GetPos());
	auto constraintOverSecondNode = std::make_shared<chrono::fea::ChLinkPointFrame>();
	constraintOverSecondNode->Initialize(this->m_builder.GetLastBeamNodes().front(), this->m_base);
	this->m_refSystem.Add(constraintOverSecondNode);

	auto constraintOverJuncture2 = std::make_shared<chrono::fea::ChLinkPointFrame>();
	constraintOverJuncture2->Initialize(this->m_builder.GetLastBeamNodes().back(), this->m_base);
	this->m_refSystem.Add(constraintOverJuncture2);
	///

	///Third cable
	this->m_builder.BuildBeam(this->m_mesh, this->m_sectionCable, 1, nodes[2]->GetPos(), nodes[2]->GetPos() + chrono::VECT_Y * -1);
	auto constraintOverThirdNode = std::make_shared<chrono::fea::ChLinkPointFrame>();
	constraintOverThirdNode->Initialize(this->m_builder.GetLastBeamNodes().front(), this->m_base);
	this->m_refSystem.Add(constraintOverThirdNode);
	///
}
