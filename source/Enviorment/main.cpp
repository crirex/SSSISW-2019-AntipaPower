#include "chrono/physics/ChSystemNSC.h"
#include "chrono_irrlicht/ChIrrApp.h"

#include "Enviorment/Exporter.h"

#include "Enviorment/GraphicalObjects/Configurations.h"

#include "Enviorment/RenderingSystem.h"
#include "Enviorment/GraphicalBuilder.h"

#include "Enviorment/GraphicalObjects/Wall.h"
#include "Enviorment/GraphicalObjects/Beam.h"
#include "Enviorment/GraphicalObjects/Cuboid.h"
#include "Enviorment/GraphicalObjects/Cable.h"

#include "Enviorment/AlgorithmExampleFunctions.h"
#include "Enviorment/Services/RenderingService.h"
#include "Enviorment/Services/TetraMeshLoggerService.h"
#include "Enviorment/Services/MeshOptimizerService.h"

#include "Enviorment/GeneticAlgorithm/GeneticAlgorithm.h"

int main(int argc, char* argv[])
{
	// ========= Some setup that should always be here for the rest of the code =========
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);
	auto system = std::make_shared<chrono::ChSystemNSC>();
	auto application = GraphicalObjects::Configurations::CreateAppConfig(system);
	RenderingSystem renderingSystem(application, system);

	// ========= This is for the beam on the wall and a cube at the end that is connected by the beam with 2 strings =========
	/*auto wall = std::make_shared<GraphicalObjects::Wall>(5, 0.1, 5, 1000, false, true);
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
		});
	beamBuilder.Build(system, beam);
	beamBuilder.Build(system, wall);
	beamBuilder.Build(system, cable);*/

	// ========= This is for the beam optimization using the genetic algorithm =========
	auto cuboid = std::make_shared<GraphicalObjects::Cuboid>(chrono::Vector(0.5), chrono::ChVector<int>(3, 3, 10));
	cuboid->SetMaterial(GraphicalObjects::Configurations::CreateBeamMaterialConfig());
	cuboid->SetVisualizationMesh(GraphicalObjects::Configurations::CreateBeamVisualizationMeshConfig(cuboid->GetMesh()));

	GraphicalBuilder graphicalBuilder({
		std::make_shared<Services::RenderingService>(),
		std::make_shared<Services::MeshOptimizerService>(),
		});

	graphicalBuilder.Build(system, cuboid);

	// ========= This is the infinite loop for the rendering. Only after this is closed does the other code below execute. This needs to always be here. =========
	renderingSystem.Start();

	// ========= This is the export to put the resulted beam in a different app to view it externally =========
	Exporter::WriteMesh(cuboid->GetMesh(), "beamWriteMesh");
	std::string buffer = "beamWriteFrameAfter.vtk";
	Exporter::WriteFrame(cuboid->GetMesh(), buffer, "beamWriteMesh");

	// ========= The function examples we had to do on our genetic algoritm to verify if it did well =========
	/*
	GeneticAlgorithm<> geneticAlgorithmUno(AlgorithmExampleFunctions::function1, 10, 300, 0.1, 0.01, 1, true);
	geneticAlgorithmUno.Fit("first_output_functionUno.txt");
	geneticAlgorithmUno.Fit("second_output_functionUno.txt");
	geneticAlgorithmUno.Fit("third_output_functionUno.txt");

	GeneticAlgorithm<> geneticAlgorithmDos(AlgorithmExampleFunctions::function2, 10, 300, 0.1, 0.01, 2, false);
	geneticAlgorithmDos.Fit("first_output_functionDos.txt");
	geneticAlgorithmDos.Fit("second_output_functionDos.txt");
	geneticAlgorithmDos.Fit("third_output_functionDos.txt");
	*/

	return 0;
}
