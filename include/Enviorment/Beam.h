#pragma once

#include "chrono/physics/ChSystemNSC.h"
#include "chrono/physics/ChLinkMate.h"
#include "chrono/physics/ChBodyEasy.h"
#include "chrono/timestepper/ChTimestepper.h"
#include "chrono/solver/ChSolverPMINRES.h"
#include "chrono/solver/ChSolverMINRES.h"
#include "fea/ChElementBeamEuler.h"
#include "fea/ChBuilderBeam.h"
#include "fea/ChMesh.h"
#include "fea/ChVisualizationFEAmesh.h"
#include "fea/ChElementTetra_4.h"
#include "fea/ChLinkPointFrame.h"
#include "fea/ChLinkDirFrame.h"
#include "chrono_irrlicht/ChIrrApp.h"

class Beam
{
public:
	Beam();
	Beam(const chrono::ChVector<> & firstNode,
		const chrono::ChVector<> & secondNode,
		const std::pair<double, double> & sectionWidthYZ);

	Beam(const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & firstNode,
		const std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> & secondNode,
		const double widthY = 0.01, const double widthZ = 0.01);
	~Beam() = default;

public:
	std::shared_ptr<chrono::fea::ChNodeFEAxyzrot>& GetFirstNode();
	std::shared_ptr<chrono::fea::ChNodeFEAxyzrot>& GetSecondNode();
	std::shared_ptr<chrono::fea::ChMesh> CreateBeam();

private:
	void SetSection(double widthY, double widthZ);

public:
	std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> m_node1;
	std::shared_ptr<chrono::fea::ChNodeFEAxyzrot> m_node2;
private:
	double m_section_widthY;
	double m_section_widthZ;
};

