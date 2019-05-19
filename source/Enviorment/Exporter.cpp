#include "Enviorment/Exporter.h"


void Exporter::WriteMesh(std::shared_ptr<ChMesh> my_mesh, std::string SaveAs)
{
	int numTetras = 0;
	std::vector<std::shared_ptr<ChNodeFEAbase>> nodes;
	std::ofstream meshFile;

	meshFile.open(SaveAs, std::ios::out | std::ofstream::trunc);
	meshFile.precision(7);
	meshFile << std::scientific;
	nodes.resize(my_mesh->GetNnodes());

	for (uint16_t nodeIndex = 0; nodeIndex < my_mesh->GetNnodes(); ++nodeIndex)
	{
		nodes[nodeIndex] = std::dynamic_pointer_cast<ChNodeFEAbase>(my_mesh->GetNode(nodeIndex));
	}

	for (uint16_t elementIndex = 0; elementIndex < my_mesh->GetNelements(); ++elementIndex)
	{
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(elementIndex)))
			numTetras++;
	}

	meshFile << "\nCELLS " << my_mesh->GetNelements() << " " << static_cast<uint16_t>(numTetras * 5) << "\n";

	for (uint16_t elementIndex = 0; elementIndex < my_mesh->GetNelements(); ++elementIndex)
	{
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(elementIndex)))
		{
			meshFile << "4 ";
			for (uint16_t myNodeN = 0; myNodeN < element->GetNnodes(); myNodeN++)
			{
				auto node = element->GetNodeN(myNodeN);
				std::vector<std::shared_ptr<ChNodeFEAbase>>::iterator it;
				it = find(nodes.begin(), nodes.end(), node);
				if (it != nodes.end())
				{
					auto index = std::distance(nodes.begin(), it);
					meshFile << static_cast<uint16_t>(index) << " ";
				}
			}
			meshFile << "\n";
		}
	}

	meshFile << "\nCELL_TYPES " << my_mesh->GetNelements() << "\n";

	for (uint16_t elementIndex = 0; elementIndex < my_mesh->GetNelements(); ++elementIndex)
	{
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(elementIndex)))
			meshFile << "9\n";
	}

	meshFile.close();
}

void Exporter::WriteFrame(std::shared_ptr<ChMesh> my_mesh, std::string saveAsBuffer, std::string meshFileBuffer)
{
	std::ofstream output;
	output.open(saveAsBuffer, std::ios::out | std::ofstream::trunc);

	output << "# vtk DataFile Version 2.0" << std::endl;
	output << "Unstructured Grid Example" << std::endl;
	output << "ASCII" << std::endl;
	output << "DATASET UNSTRUCTURED_GRID" << std::endl;
	output << "POINTS " << my_mesh->GetNnodes() << " float\n";

	for (uint16_t index = 0; index < my_mesh->GetNnodes(); ++index)
	{
		auto node = std::dynamic_pointer_cast<ChNodeFEAxyz>(my_mesh->GetNode(index));
		output << node->GetPos().x() << " " << node->GetPos().y() << " " << node->GetPos().z() << "\n";
	}

	std::ifstream copyFrom(meshFileBuffer);
	output << copyFrom.rdbuf();

	int numCell = 0;
	for (uint16_t elementIndex = 0; elementIndex < my_mesh->GetNelements(); elementIndex++)
	{
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(elementIndex)))
			numCell++;
	}

	output << "\nCELL_DATA " << numCell << "\n";
	output << "SCALARS Deflection float 1\n";
	output << "LOOKUP_TABLE default\n";

	double scalar = 0;
	for (uint16_t elementIndex = 0; elementIndex < my_mesh->GetNelements(); elementIndex++) {
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(elementIndex)))
			EvaluateDeflection(element, scalar);
		output << scalar + 1e-20 << "\n";
	}

	output << "VECTORS Strain float\n";

	ChVector<> Strain(1e-20);
	for (uint16_t elementIndex = 0; elementIndex < my_mesh->GetNelements(); elementIndex++) {
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(elementIndex)))
		{
			auto StrainV = element->GetStrain();
			output << StrainV.XX() + Strain.x() << " " << StrainV.YY() + Strain.y() << " " << StrainV.ZZ() + Strain.z() << "\n";
		}
	}

	output << "\nPOINT_DATA " << my_mesh->GetNnodes() << "\n";

	output << "VECTORS Velocity float\n";

	for (uint16_t index = 0; index < my_mesh->GetNnodes(); index++) {
		ChVector<> vel = std::dynamic_pointer_cast<ChNodeFEAxyz>(my_mesh->GetNode(index))->GetPos_dt();
		vel += ChVector<>(1e-20);
		output << static_cast<double>(vel.x()) << " " << static_cast<double>(vel.y()) << " " << static_cast<double>(vel.z()) << "\n";
	}

	output << "VECTORS Acceleration float\n";

	for (uint16_t index = 0; index < my_mesh->GetNnodes(); index++) {
		ChVector<> acc = std::dynamic_pointer_cast<ChNodeFEAxyz>(my_mesh->GetNode(index))->GetPos_dtdt();
		acc += ChVector<>(1e-20);
		output << static_cast<double>(acc.x()) << " " << static_cast<double>(acc.y()) << " " << static_cast<double>(acc.z()) << "\n";
	}

	output.close();
}

void Exporter::EvaluateDeflection(std::shared_ptr<ChElementTetra_4> & element, double & def)
{
	//Extracting the information regarding the nodes
	ChMatrixDynamic<> matrix;
	element->GetStateBlock(matrix);
	std::vector<ChVector<>> vectors;
	vectors.emplace_back(ChVector<>(matrix.ClipVector(0, 0)));
	vectors.emplace_back(ChVector<>(matrix.ClipVector(3, 0)));
	vectors.emplace_back(ChVector<>(matrix.ClipVector(6, 0)));
	vectors.emplace_back(ChVector<>(matrix.ClipVector(9, 0)));

	ChVector<> pos;
	ChVector<> defVec;

	for (const auto& vect : vectors)
	{
		pos.x() += vect.x();
		pos.y() += vect.y();
		pos.z() += vect.z();
	}

	defVec = pos / 4;
	def = defVec.Length();
}
