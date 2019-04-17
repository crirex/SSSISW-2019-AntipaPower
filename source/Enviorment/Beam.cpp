#include "../../AMMO/SSSISW-2019-AntipaPower/include/Enviorment/Beam.h"

Beam::Beam()
{
	m_node1 = std::make_shared<chrono::fea::ChNodeFEAxyzrot>(chrono::ChFrame<>(chrono::ChVector<>(0, 0, 0)));
	m_node2 = std::make_shared<chrono::fea::ChNodeFEAxyzrot>(chrono::ChFrame<>(chrono::ChVector<>(0.1, 0, 0)));
	m_section_widthY = 0.01;
	m_section_widthZ = 0.01;
}

Beam::Beam(const chrono::ChVector<> & firstNode,
	const chrono::ChVector<> & secondNode,
	const std::pair<double, double> & sectionWidthYZ)
{
	m_node1 = std::make_shared<chrono::fea::ChNodeFEAxyzrot>(chrono::ChFrame<>(firstNode));
	m_node2 = std::make_shared<chrono::fea::ChNodeFEAxyzrot>(chrono::ChFrame<>(secondNode));
	m_section_widthY = sectionWidthYZ.first;
	m_section_widthZ = sectionWidthYZ.second;
}

Beam::Beam(const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & firstNode,
	const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & secondNode,
	const double widthY, const double widthZ)
{
	m_node1 = firstNode;
	m_node2 = secondNode;
	m_section_widthY = widthY;
	m_section_widthZ = widthZ;
}

std::shared_ptr<chrono::fea::ChNodeFEAxyzrot>& Beam::GetFirstNode()
{
	return m_node1;
}

std::shared_ptr<chrono::fea::ChNodeFEAxyzrot>& Beam::GetSecondNode()
{
	return m_node2;
}

void Beam::SetSection(double widthY, double widthZ)
{
	m_section_widthY = widthY;
	m_section_widthZ = widthZ;
}

std::shared_ptr<chrono::fea::ChMesh> Beam::CreateBeam()
{
	auto my_mesh = std::make_shared<chrono::fea::ChMesh>();
	auto mvisualizebeam = std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(my_mesh.get()));
	mvisualizebeam->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_ELEM_BEAM_MZ);
	mvisualizebeam->SetColorscaleMinMax(-0.4, 0.4);
	mvisualizebeam->SetSmoothFaces(true);
	mvisualizebeam->SetWireframe(false);
	my_mesh->AddAsset(mvisualizebeam);

	my_mesh->AddNode(m_node1);
	my_mesh->AddNode(m_node2);


	auto beamElement = std::make_shared<chrono::fea::ChElementBeamEuler>();
	my_mesh->AddElement(beamElement);


	beamElement->SetNodes(m_node1, m_node2);

	auto section = std::make_shared<chrono::fea::ChBeamSectionAdvanced>(chrono::fea::ChBeamSectionAdvanced());
	section->SetAsRectangularSection(m_section_widthY, m_section_widthZ);
	beamElement->SetSection(section);
	return my_mesh;
}
