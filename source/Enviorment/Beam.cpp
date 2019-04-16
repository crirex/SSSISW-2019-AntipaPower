#include "../../AMMO/SSSISW-2019-AntipaPower/include/Enviorment/Beam.h"

Beam::Beam()
{
	this->m_mesh = std::make_shared<chrono::fea::ChMesh>();
	this->m_beamElement = std::make_shared<chrono::fea::ChElementBeamEuler>();
	this->m_visualizationMesh = std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(this->m_mesh.get()));
	this->m_mesh->AddElement(this->m_beamElement);
	InitVisualizationMesh();
}

Beam::Beam(const chrono::ChVector<> & firstNode,
	const chrono::ChVector<> & secondNode,
	const std::pair<double, double> & sectionWidthYZ) : Beam()
{
	this->m_beamElement->SetNodes(std::make_shared<chrono::fea::ChNodeFEAxyzrot>(chrono::ChFrame<>(firstNode)),
		std::make_shared<chrono::fea::ChNodeFEAxyzrot>(chrono::ChFrame<>(secondNode)));
	SetSection(sectionWidthYZ.first, sectionWidthYZ.second);
}

Beam::Beam(const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & firstNode,
	const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & secondNode,
	const std::shared_ptr<chrono::fea::ChBeamSectionAdvanced> & section) : Beam()
{

	this->m_beamElement->SetNodes(firstNode, secondNode);
	this->m_beamElement->SetSection(section);
}

std::shared_ptr<chrono::fea::ChMesh> Beam::GetMesh() const
{
	return this->m_mesh;
}

const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & Beam::GetVisualizationMesh() const
{
	return this->m_visualizationMesh;
}

const std::shared_ptr<chrono::fea::ChElementBeamEuler> & Beam::GetBeamElement() const
{
	return this->m_beamElement;
}

std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> Beam::GetFirstNode() const
{
	return this->m_beamElement->GetNodeA();
}

std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> Beam::GetSecondNode() const
{
	return this->m_beamElement->GetNodeB();
}

void Beam::SetSection(double widthY, double widthZ)
{
	auto section = std::make_shared<chrono::fea::ChBeamSectionAdvanced>(chrono::fea::ChBeamSectionAdvanced());
	section->SetAsRectangularSection(widthY, widthZ);
	this->m_beamElement->SetSection(section);
}

void Beam::InitVisualizationMesh()
{
	this->m_visualizationMesh->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_ELEM_BEAM_MZ);
	this->m_visualizationMesh->SetColorscaleMinMax(-0.5, 0.5);
	this->m_visualizationMesh->SetSmoothFaces(true);
	this->m_visualizationMesh->SetWireframe(false);
	this->m_mesh->AddAsset(this->m_visualizationMesh);
}
