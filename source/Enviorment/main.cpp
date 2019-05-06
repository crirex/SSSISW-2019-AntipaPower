#include "chrono/physics/ChSystemNSC.h"
#include "chrono/timestepper/ChTimestepper.h"
#include "chrono/solver/ChSolverPMINRES.h"
#include "chrono/solver/ChSolverMINRES.h"
#include "chrono_irrlicht/ChIrrApp.h"
#include "chrono/fea/ChLinkPointFrame.h"
#include "chrono/fea/ChBuilderBeam.h"
#include "chrono/fea/ChMeshExporter.h"

#include "Cable.h"
#include "Cuboid.h"
#include "Wall.h"

using namespace chrono;
using namespace chrono::fea;
using namespace chrono::irrlicht;
using namespace irr;

void writeMesh(std::shared_ptr<ChMesh> my_mesh, std::string SaveAs) 
{
	std::ofstream MESH;  // output file stream
	MESH.open(SaveAs, std::ios::out);
	MESH.precision(7);
	MESH << std::scientific;
	std::vector<std::vector<int>> CableElemNodes;
	std::vector<std::vector<int>> ShellElemNodes;
	std::vector<std::vector<int>> BrickElemNodes;

	std::vector<std::shared_ptr<ChNodeFEAbase>> myvector;
	myvector.resize(my_mesh->GetNnodes());

	for (unsigned int i = 0; i < my_mesh->GetNnodes(); i++)
	{
		myvector[i] = std::dynamic_pointer_cast<ChNodeFEAbase>(my_mesh->GetNode(i));
	}

	int numCables = 0;
	int numShells = 0;
	int numBricks = 0;
	for (unsigned int iele = 0; iele < my_mesh->GetNelements(); iele++)
	{
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele)))
			numBricks++;
	}
	MESH << "\nCELLS " << my_mesh->GetNelements() << " "
		<< (unsigned int)(numCables * 3 + numShells * 5 + numBricks * 9) << "\n";

	for (unsigned int iele = 0; iele < my_mesh->GetNelements(); iele++)
	{
		std::vector<int> mynodes;
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele))) 
		{
			mynodes.resize(4);
			MESH << "4 ";
			int nodeOrder[] = { 0, 1, 2, 3 };
			mynodes[0] = element->GetNodeN(nodeOrder[0])->GetIndex();
			mynodes[1] = element->GetNodeN(nodeOrder[1])->GetIndex();
			mynodes[2] = element->GetNodeN(nodeOrder[2])->GetIndex();
			mynodes[3] = element->GetNodeN(nodeOrder[3])->GetIndex();
			ShellElemNodes.push_back(mynodes);
			for (int myNodeN = 0; myNodeN < mynodes.size(); myNodeN++) 
			{
				auto nodeA = (element->GetNodeN(nodeOrder[myNodeN]));
				std::vector<std::shared_ptr<ChNodeFEAbase>>::iterator it;
				it = find(myvector.begin(), myvector.end(), nodeA);
				if (it == myvector.end())
				{
					// name not in vector
				}
				else 
				{
					auto index = std::distance(myvector.begin(), it);
					MESH << (unsigned int)index << " ";
				}
			}
			MESH << "\n";
		}
		
	}

	MESH << "\nCELL_TYPES " << my_mesh->GetNelements() << "\n";

	for (unsigned int iele = 0; iele < my_mesh->GetNelements(); iele++)
	{
		 if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele)))
			MESH << "9\n";
	}

	MESH.close();
}

void writeFrame(std::shared_ptr<ChMesh> my_mesh, char SaveAsBuffer[256], std::string MeshFileBuffer) {
	std::ofstream output;
	std::string SaveAsBuffer_string(SaveAsBuffer);
	SaveAsBuffer_string.erase(SaveAsBuffer_string.length() - 4, 4);
	std::cout << SaveAsBuffer_string << std::endl;
	snprintf(SaveAsBuffer, sizeof(char) * 256, ("%s"), (SaveAsBuffer_string + ".vtk").c_str());
	output.open(SaveAsBuffer, std::ios::app);

	output << "# vtk DataFile Version 2.0" << std::endl;
	output << "Unstructured Grid Example" << std::endl;
	output << "ASCII" << std::endl;
	output << "DATASET UNSTRUCTURED_GRID" << std::endl;

	output << "POINTS " << my_mesh->GetNnodes() << " float\n";
	for (unsigned int i = 0; i < my_mesh->GetNnodes(); i++) 
	{
		auto node = std::dynamic_pointer_cast<ChNodeFEAxyz>(my_mesh->GetNode(i));
		output << node->GetPos().x() << " " << node->GetPos().y() << " " << node->GetPos().z() << "\n";
	}

	std::ifstream CopyFrom(MeshFileBuffer);
	output << CopyFrom.rdbuf();

	int numCell = 0;
	for (unsigned int iele = 0; iele < my_mesh->GetNelements(); iele++)
	{
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele)))
			numCell++;
	}

	output << "\nCELL_DATA " << numCell << "\n";
	output << "SCALARS Deflection float 1\n";
	output << "LOOKUP_TABLE default\n";

	double scalar = 0;
	for (unsigned int iele = 0; iele < my_mesh->GetNelements(); iele++) {
		if (auto element = std::dynamic_pointer_cast<ChElementCableANCF>(my_mesh->GetElement(iele)))
			scalar = element->GetCurrLength() - element->GetRestLength();
		else if (auto element = std::dynamic_pointer_cast<ChElementShellANCF>(my_mesh->GetElement(iele)))
			element->EvaluateDeflection(scalar);
		/*else if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele)))
			element->*/
		output << scalar + 1e-20 << "\n";
	}

	output << "VECTORS Strain float\n";
	//ChVector<> StrainV;
	for (unsigned int iele = 0; iele < my_mesh->GetNelements(); iele++) {
		/*if (auto element = std::dynamic_pointer_cast<ChElementCableANCF>(my_mesh->GetElement(iele)))
			element->EvaluateSectionStrain(0.0, StrainV);
		else if (auto element = std::dynamic_pointer_cast<ChElementShellANCF>(my_mesh->GetElement(iele)))
			StrainV = element->EvaluateSectionStrains();*/
		/*else*/ if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele)))
		{
			auto StrainV = element->GetStrain();
			output << StrainV.XX() << " " << StrainV.YY() << " " << StrainV.ZZ() << "\n";
		}

		//StrainV += ChVector<>(1e-20);
		//output << StrainV.x() << " " << StrainV.y() << " " << StrainV.z() << "\n";
	}

	output << "\nPOINT_DATA " << my_mesh->GetNnodes() << "\n";

	output << "VECTORS Velocity float\n";
	for (unsigned int i = 0; i < my_mesh->GetNnodes(); i++) {
		ChVector<> vel = std::dynamic_pointer_cast<ChNodeFEAxyz>(my_mesh->GetNode(i))->GetPos_dt();
		vel += ChVector<>(1e-20);
		output << (double)vel.x() << " " << (double)vel.y() << " " << (double)vel.z() << "\n";
	}

	output << "VECTORS Acceleration float\n";

	for (unsigned int i = 0; i < my_mesh->GetNnodes(); i++) {
		ChVector<> acc = std::dynamic_pointer_cast<ChNodeFEAxyz>(my_mesh->GetNode(i))->GetPos_dtdt();
		acc += ChVector<>(1e-20);
		output << (double)acc.x() << " " << (double)acc.y() << " " << (double)acc.z() << "\n";
	}

	output.close();
}

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
	//my_system.SetEndTime(); Cu asta putem limita timpul de execute(folositor la chestia cu 200 de frame-uri?)
	/*my_system.SetEndTime(0.1);
	my_system.SetSt
	std::cout << my_system.GetEndTime();*/


	auto msolver = std::static_pointer_cast<ChSolverMINRES>(my_system.GetSolver());
	msolver->SetVerbose(false);
	msolver->SetDiagonalPreconditioning(true);

	writeMesh(beam.GetMesh(), "beamWriteMesh");
	char buffer[256] = "beamWriteFrame.vtk";
	writeFrame(beam.GetMesh(), buffer, "beamWriteMesh");

	while (application.GetDevice()->run())
	{
		beam.StartLogStrained();
		application.BeginScene();
		application.DrawAll();
		application.DoStep();
		application.EndScene();
	}
	return 0;
}
