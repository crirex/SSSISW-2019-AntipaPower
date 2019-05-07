#pragma once

#include "chrono/fea/ChBuilderBeam.h"
#include "chrono/fea/ChMesh.h"
#include "chrono/fea/ChVisualizationFEAmesh.h"
#include "chrono/fea/ChBuilderBeam.h"
#include "chrono/physics/ChBodyEasy.h"
#include <cmath>

#include "Beam.h"

class Cable
{
public:
	Cable(chrono::ChSystemNSC& system);
	~Cable() = default;

	const std::shared_ptr<chrono::fea::ChMesh>& GetMesh() const;

	void SetVisualtizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualtiozationMesh);
	void BuildCable(Beam& beam);

private:
	void InitializeSectionCable();
	void ConstructBase(std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes);

private:
	std::shared_ptr<chrono::fea::ChMesh> m_mesh;
	std::shared_ptr<chrono::fea::ChBeamSectionCable> m_sectionCable;
	std::shared_ptr<chrono::ChBody> m_base;

	chrono::ChSystemNSC& m_refSystem;

	chrono::fea::ChBuilderBeamANCF m_builder;
};
