#pragma once

#include <iostream>
#include <array>

#include "fea/ChMesh.h"
#include "fea/ChElementBeamEuler.h"
#include "fea/ChVisualizationFEAmesh.h"
#include "chrono/physics/ChBody.h"
#include "chrono/assets/ChBoxShape.h"
#include "chrono/physics/ChSystemNSC.h"
#include "chrono/fea/ChElementTetra_4.h"
#include "chrono/fea/ChLinkPointFrame.h"
#include "chrono/fea/ChElementShellANCF.h"


using namespace chrono;
using namespace chrono::fea;

class Exporter
{
public:
	static void WriteMesh(std::shared_ptr<ChMesh> my_mesh, std::string saveAs);
	static void WriteFrame(std::shared_ptr<ChMesh> my_mesh, std::string saveAsBuffer, std::string meshFileBuffer);
};