#pragma once

#include "fea/ChElementBeamEuler.h"
#include "fea/ChMesh.h"
#include "fea/ChVisualizationFEAmesh.h"

class Beam
{
public:
	Beam();
	Beam(const chrono::ChVector<> & firstNode,
		const chrono::ChVector<> & secondNode,
		const std::pair<double, double> & sectionWidthYZ);

	Beam(const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & firstNode,
		const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & secondNode,
		const std::shared_ptr<chrono::fea::ChBeamSectionAdvanced> & section);
	~Beam() = default;

public:
	std::shared_ptr<chrono::fea::ChMesh> GetMesh() const;
	const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & GetVisualizationMesh() const;
	const std::shared_ptr<chrono::fea::ChElementBeamEuler> & GetBeamElement() const;
	std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> GetFirstNode() const;
	std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> GetSecondNode() const;

private:
	void SetSection(double widthY, double widthZ);
	void InitVisualizationMesh();

private:
	std::shared_ptr<chrono::fea::ChMesh> m_mesh;
	std::shared_ptr<chrono::fea::ChElementBeamEuler> m_beamElement;
	std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> m_visualizationMesh;
};


