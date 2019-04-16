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

int main(int argc, char* argv[])
{
	SetChronoDataPath(CHRONO_DATA_DIR);

	ChSystemNSC my_system;

	ChIrrApp application(&my_system, L"Environment", core::dimension2d<u32>(800, 600), false);
	application.AddTypicalLogo();
	application.AddTypicalSky();
	application.AddTypicalCamera(core::vector3df(0.3, 0.3, 0.3));
	application.AddLightWithShadow(core::vector3df(1, 25, -5), core::vector3df(0, 0, 0), 35, 0.2, 35, 100, 512, video::SColorf(1, 1, 1));

	auto my_mesh = std::make_shared<ChMesh>();
	my_system.Add(my_mesh);

	auto beam = std::make_shared<Beam>(ChVector<>(0, 0.1, 0.1), ChVector<>(0.1, 0.1, 0.1), std::make_pair<double,double>( 0.01, 0.01 ));
	my_system.Add(beam->GetMesh());
	beam->GetFirstNode()->SetFixed(true);
	beam->GetSecondNode()->SetMass(15);
	beam->GetSecondNode()->SetForce(chrono::VectorF(0.0f, -1.0f, 0.0f));




	auto mvisualizebeam = std::make_shared<ChVisualizationFEAmesh>(*(my_mesh.get()));
	mvisualizebeam->SetFEMdataType(ChVisualizationFEAmesh::E_PLOT_ELEM_BEAM_MZ);
	mvisualizebeam->SetColorscaleMinMax(-0.4, 0.4);
	mvisualizebeam->SetSmoothFaces(true);
	mvisualizebeam->SetWireframe(false);
	my_mesh->AddAsset(mvisualizebeam);

	auto mnode1 = std::make_shared<ChNodeFEAxyzrot>(ChFrame<>(ChVector<>(0, 0, 0)));
	auto mnode2 = std::make_shared<ChNodeFEAxyzrot>(ChFrame<>(ChVector<>(0.1, 0, 0)));

	mnode1->SetFixed(true);

	my_mesh->AddNode(mnode1);
	my_mesh->AddNode(mnode2);

	mnode2->SetMass(15);
	mnode2->SetForce(chrono::VectorF(0.0f, -1.0f, 0.0f));


	auto beamElement = std::make_shared<ChElementBeamEuler>();
	my_mesh->AddElement(beamElement);


	beamElement->SetNodes(mnode1, mnode2);

	auto section = std::make_shared<fea::ChBeamSectionAdvanced>(fea::ChBeamSectionAdvanced());
	section->SetAsRectangularSection(0.01, 0.01);
	beamElement->SetSection(section);


	application.AssetBindAll();
	application.AssetUpdateAll();
	my_system.SetupInitial();

	my_system.SetSolverType(ChSolver::Type::MINRES);
	my_system.SetSolverWarmStarting(true);
	my_system.SetMaxItersSolverSpeed(460);
	my_system.SetMaxItersSolverStab(460);
	my_system.SetTolForce(1e-13);

	auto msolver = std::static_pointer_cast<ChSolverMINRES>(my_system.GetSolver());
	msolver->SetVerbose(false);
	msolver->SetDiagonalPreconditioning(true);

	while (application.GetDevice()->run())
	{
		application.BeginScene();
		application.DrawAll();
		application.DoStep();
		application.EndScene();
	}
	return 0;
}