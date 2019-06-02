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
#include "Enviorment/Services/RenderingService.h"
#include "Enviorment/Services/TetraMeshLoggerService.h"
#include "Enviorment/Services/MeshOptimizerService.h"

#include "Enviorment/GeneticAlgorithm/GeneticAlgorithm.h"

#include <math.h>


double functionUno(Individual<> individual)
{
	const int minimInterval = -10;
	const int maximInterval = 10;

	const auto& cromososns = individual.GetChromosomes();
	const auto& x = cromososns[0].GetValueInInterval(minimInterval, maximInterval);

	return x * x + 6 * x + 1;
}

double functionDos(Individual<> individual)
{
	const int xMinimInterval = -5;
	const int xMaximInterval = 3;
	const int yMinimInterval = 2;
	const int yMaximInterval = 10;

	const auto& cromososns = individual.GetChromosomes();
	const auto& x = cromososns[0].GetValueInInterval(xMinimInterval, xMaximInterval);
	const auto& y = cromososns[1].GetValueInInterval(yMinimInterval, yMaximInterval);

	return std::sin(std::_Pi * 10 * x + 10 / (1 + y * y)) + std::log(x * x + y * y);

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
	auto cuboid = std::make_shared<GraphicalObjects::Cuboid>(chrono::Vector(0.5), chrono::ChVector<int>(3, 3, 10));
	cuboid->SetMaterial(GraphicalObjects::Configurations::CreateBeamMaterialConfig());
	cuboid->SetVisualizationMesh(GraphicalObjects::Configurations::CreateBeamVisualizationMeshConfig(cuboid->GetMesh()));



	GraphicalBuilder beamBuilder({
		std::make_shared<Services::RenderingService>(),
		std::make_shared<Services::TetraMeshLoggerService>(),
		std::make_shared<Services::MeshOptimizerService>(),
		});
	//beamBuilder.Build(system, beam);

	GraphicalBuilder graphicalBuilder({
		std::make_shared<Services::RenderingService>(),
		std::make_shared<Services::MeshOptimizerService>(),

		});
	//graphicalBuilder.Build(system, wall);
	//graphicalBuilder.Build(system, cable);
	graphicalBuilder.Build(system, cuboid);

	renderingSystem.Start();

	Exporter::WriteMesh(cuboid->GetMesh(), "beamWriteMesh");
	std::string buffer = "beamWriteFrameAfter.vtk";
	Exporter::WriteFrame(cuboid->GetMesh(), buffer, "beamWriteMesh");

	GeneticAlgorithm<> geneticAlgorithmUno(functionUno, 10, 300, 0.1, 0.01, 1, true);
	geneticAlgorithmUno.Fit("first_output_functionUno.txt");
	geneticAlgorithmUno.Fit("second_output_functionUno.txt");
	geneticAlgorithmUno.Fit("third_output_functionUno.txt");

	GeneticAlgorithm<> geneticAlgorithmDos(functionDos, 10, 300, 0.1, 0.01, 2, false);
	geneticAlgorithmDos.Fit("first_output_functionDos.txt");
	geneticAlgorithmDos.Fit("second_output_functionDos.txt");
	geneticAlgorithmDos.Fit("third_output_functionDos.txt");

	return 0;
}
