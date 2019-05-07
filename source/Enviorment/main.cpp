#include "chrono/physics/ChSystemNSC.h"
#include "chrono/timestepper/ChTimestepper.h"
#include "chrono/solver/ChSolverPMINRES.h"
#include "chrono/solver/ChSolverMINRES.h"
#include "chrono_irrlicht/ChIrrApp.h"
#include "chrono/fea/ChLinkPointFrame.h"
#include "chrono/fea/ChBuilderBeam.h"
#include "chrono/fea/ChMeshExporter.h"

#include "Cable.h"
#include "Beam.h"
#include "Wall.h"
#include "Exporter.h"

using namespace chrono;
using namespace chrono::fea;
using namespace chrono::irrlicht;
using namespace irr;

// Time interval between two render frames
double render_step_size = 1.0 / 50; // FPS = 50  == 0.02

int main(int argc, char* argv[])
{
	SetChronoDataPath(CHRONO_DATA_DIR);

	ChSystemNSC my_system;

	ChIrrApp application(&my_system, L"Environment", core::dimension2d<u32>(1200, 720), false);
	application.AddTypicalLogo();
	application.AddTypicalSky();
	application.AddTypicalLights();
	application.AddTypicalCamera(core::vector3df(3, 3, 3));

	auto beam = Beam(my_system);

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

	beam.Build(chrono::Vector(0.5, 0.5, 0.5), 6, chrono::Vector(1, 0, 0), chrono::Vector(0, 0, 0));

	auto cablu = Cable(my_system);

	auto mvisualizebeamA = std::make_shared<ChVisualizationFEAmesh>(*(cablu.GetMesh()));
	mvisualizebeamA->SetFEMdataType(ChVisualizationFEAmesh::E_PLOT_ELEM_BEAM_MZ);
	mvisualizebeamA->SetColorscaleMinMax(-0.4, 0.4);
	mvisualizebeamA->SetSmoothFaces(true);
	mvisualizebeamA->SetWireframe(false);
	cablu.SetVisualtizationMesh(mvisualizebeamA);

	auto mvisualizebeamC = std::make_shared<ChVisualizationFEAmesh>(*(cablu.GetMesh()));
	mvisualizebeamC->SetFEMglyphType(ChVisualizationFEAmesh::E_GLYPH_NODE_DOT_POS); // E_GLYPH_NODE_CSYS
	mvisualizebeamC->SetFEMdataType(ChVisualizationFEAmesh::E_PLOT_NONE);
	mvisualizebeamC->SetSymbolsThickness(0.006);
	mvisualizebeamC->SetSymbolsScale(0.01);
	mvisualizebeamC->SetZbufferHide(false);
	cablu.SetVisualtizationMesh(mvisualizebeamC);


	cablu.BuildCable(beam);

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

	ChRealtimeStepTimer realtime_timer;
	size_t stepCount = 0;

	while (application.GetDevice()->run())
	{
		double step = realtime_timer.SuggestSimulationStep(render_step_size);
		application.SetTimestep(step);
		//beam.StartLogStrained();
		application.BeginScene();
		application.DrawAll();
		application.DoStep();
		application.EndScene();
		stepCount > 200 ? application.GetDevice()->closeDevice() : ++stepCount;
	}

	Exporter::WriteMesh(beam.GetMesh(), "beamWriteMesh");
	char buffer[256] = "beamWriteFrame.vtk";
	Exporter::WriteFrame(beam.GetMesh(), buffer, "beamWriteMesh");

	return 0;
}
