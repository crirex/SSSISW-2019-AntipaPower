#include "chrono/physics/ChSystemNSC.h"
#include "chrono_irrlicht/ChIrrApp.h"

#include "Enviorment/Exporter.h"

#include "Enviorment/GraphicalObjects/Configurations.h"

#include "Enviorment/RenderingSystem.h"
#include "Enviorment/GraphicalBuilder.h"

#include "Enviorment/GraphicalObjects/Wall.h"
#include "Enviorment/GraphicalObjects/Beam.h"
#include "Enviorment/GraphicalObjects/Cable.h"
#include "Enviorment/Services/RenderingService.h"
#include "Enviorment/Services/TetraMeshLoggerService.h"
#include "Enviorment/Services/MeshOptimizerService.h"

#include "Enviorment/GeneticAlgorithm/GeneticAlgorithm.h"


double function(double x)
{
	return x * x + 6 * x + 1;
}


int main(int argc, char* argv[]) 
{
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);

	auto system = std::make_shared<chrono::ChSystemNSC>();
	auto application = GraphicalObjects::Configurations::CreateAppConfig(system);
	RenderingSystem renderingSystem(application, system);

	auto wall = std::make_shared<GraphicalObjects::Wall>(5, 0.1, 5, 1000, false, true);
	wall->SetPosition(chrono::Vector(0, 0, 0));
	wall->SetRotation(chrono::CH_C_PI / 2, chrono::VECT_Z);
	auto beam = std::make_shared<GraphicalObjects::Beam>(0.5, 6);
	beam->SetMaterial(GraphicalObjects::Configurations::CreateBeamMaterialConfig());
	beam->SetVisualizationMesh(GraphicalObjects::Configurations::CreateBeamVisualizationMeshConfig(beam->GetMesh()));
	auto cable = std::make_shared<GraphicalObjects::Cable>(*beam);
	cable->SetVisualtizationMesh(GraphicalObjects::Configurations::CreateCableVisualizationMeshConfigFirst(cable->GetMesh()));
	cable->SetVisualtizationMesh(GraphicalObjects::Configurations::CreateCableVisualizationMeshConfigSecond(cable->GetMesh()));

	GraphicalBuilder beamBuilder({
		std::make_shared<Services::RenderingService>(),
		std::make_shared<Services::TetraMeshLoggerService>(),
		std::make_shared<Services::MeshOptimizerService>(),
		});
	beamBuilder.Build(system, beam);
	
	GraphicalBuilder graphicalBuilder({
		std::make_shared<Services::RenderingService>(),
		});
	graphicalBuilder.Build(system, wall);
	graphicalBuilder.Build(system, cable);

	Exporter::WriteMesh(beam->GetMesh(), "beamWriteMesh");
	std::string buffer = "beamWriteFrameAfter.vtk";
	Exporter::WriteFrame(beam->GetMesh(), buffer, "beamWriteMesh");

	//renderingSystem.Start();
	/*
	x = -1.79688
	f(x)= -6.55249
	
	*/

	GeneticAlgorithm<> geneticAlgorithm(function);
	geneticAlgorithm.Fit();


	return 0;
}
