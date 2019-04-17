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
#include "../../AMMO/SSSISW-2019-AntipaPower/include/Enviorment/Beam.h"

using namespace chrono;
using namespace chrono::fea;
using namespace chrono::irrlicht;
using namespace irr;

class Wall
{
public:
	Wall(double xSize, double ySize, double zSize, double density, bool collide, bool visualAsset);
	~Wall() = default;

public: 
	const std::shared_ptr<chrono::ChBodyEasyBox> & GetWall() const;

	void SetPosition(const chrono::Vector & position);
	void SetRotation(double angle, const chrono::Vector & axis);

private:
	std::shared_ptr<chrono::ChBodyEasyBox> m_wall;
};

