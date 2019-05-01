#include "chrono/physics/ChSystemNSC.h"
#include "chrono/timestepper/ChTimestepper.h"
#include "chrono/solver/ChSolverPMINRES.h"
#include "chrono/solver/ChSolverMINRES.h"
#include "chrono_irrlicht/ChIrrApp.h"
#include "chrono/fea/ChLinkPointFrame.h"

#include "Cuboid.h"
#include "Wall.h"

using namespace chrono;
using namespace chrono::fea;
using namespace chrono::irrlicht;
using namespace irr;

int main(int argc, char* argv[])
{

	SetChronoDataPath(CHRONO_DATA_DIR);

	ChSystemNSC my_system;

	ChIrrApp application(&my_system, L"Environment", core::dimension2d<u32>(1200, 720), false);
	application.AddTypicalLogo();
	application.AddTypicalSky();
	application.AddTypicalLights();
	application.AddTypicalCamera(core::vector3df(3, 3, 3));

	auto beam = Cuboid(my_system, chrono::Vector(0.5, 0.5, 0.5), chrono::Vector(5, 5, 5));


	auto mesh = std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(beam.GetMesh().get()));

	mesh->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_ELEM_STRAIN_VONMISES);
	mesh->SetColorscaleMinMax(-0.5, 0.5);
	mesh->SetSmoothFaces(true);
	mesh->SetWireframe(false);
	beam.SetVisualizationMesh(mesh);

	auto material = std::make_shared<chrono::fea::ChContinuumPlasticVonMises>();
	material->Set_E(0.005e9);  // rubber 0.01e9, steel 200e9 
	material->Set_v(0.3);
	beam.SetMaterial(material);

	beam.Build(chrono::Vector(1, 0, 0));

	auto lastNode = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(beam.GetMesh()->GetNodes().back());
	lastNode->SetForce(chrono::Vector(10000, 0, 0));


	/*auto wall = std::make_shared<Wall>(0.3, 0.01, 0.3, 1000, false, true);
	wall->SetPosition(ChVector<>(0, 0, 0));
	wall->SetRotation(CH_C_PI / 2, VECT_Z);
	my_system.Add(wall->GetWall());*/


	application.AssetBindAll();
	application.AssetUpdateAll();
	my_system.SetupInitial();

	my_system.SetSolverType(ChSolver::Type::MINRES);
	my_system.SetSolverWarmStarting(true);
	my_system.SetMaxItersSolverSpeed(460);
	my_system.SetMaxItersSolverStab(460);
	my_system.SetTolForce(0.1e-13);

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
