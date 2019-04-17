#include "chrono/physics/ChSystemNSC.h"
#include "chrono/timestepper/ChTimestepper.h"
#include "chrono/solver/ChSolverPMINRES.h"
#include "chrono/solver/ChSolverMINRES.h"
#include "chrono_irrlicht/ChIrrApp.h"

#include "../../AMMO/SSSISW-2019-AntipaPower/include/Enviorment/Beam.h"
#include "../../AMMO/SSSISW-2019-AntipaPower/include/Enviorment/Wall.h"

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

	auto node1 = std::make_shared<ChNodeFEAxyzrot>(ChFrame<>(ChVector<>(0, 0, 0)));
	auto node2 = std::make_shared<ChNodeFEAxyzrot>(ChFrame<>(ChVector<>(0.1, 0, 0)));

	node1->SetFixed(true);
	node2->SetMass(15);
	node2->SetForce(chrono::VectorF(0.0f, -1.0f, 0.0f));

	Beam barna(node1, node2);
	my_system.Add(barna.CreateBeam());

	auto wall = std::make_shared<Wall>(0.3, 0.01, 0.3, 1000, false, true);
	wall->SetPosition(ChVector<>(0, 0, 0));
	wall->SetRotation(CH_C_PI / 2, VECT_Z);
	my_system.Add(wall->GetWall());

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