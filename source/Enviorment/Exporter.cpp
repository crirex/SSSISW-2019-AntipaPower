#include "Exporter.h"


void Exporter::WriteMesh(std::shared_ptr<ChMesh> my_mesh, std::string SaveAs)
{
	std::ofstream MESH;
	MESH.open(SaveAs, std::ios::out | std::ofstream::trunc);
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
			numShells++;
	}

	MESH << "\nCELLS " << my_mesh->GetNelements() << " " << (unsigned int)(numCables * 3 + numShells * 5 + numBricks * 9) << "\n";

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

void Exporter::WriteFrame(std::shared_ptr<ChMesh> my_mesh, char SaveAsBuffer[256], std::string MeshFileBuffer)
{
	std::ofstream output;
	std::string SaveAsBuffer_string(SaveAsBuffer);
	SaveAsBuffer_string.erase(SaveAsBuffer_string.length() - 4, 4);
	std::cout << SaveAsBuffer_string << std::endl;
	snprintf(SaveAsBuffer, sizeof(char) * 256, ("%s"), (SaveAsBuffer_string + ".vtk").c_str());
	output.open(SaveAsBuffer, std::ios::out | std::ofstream::trunc);

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
		else if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele)))
			EvaluateDeflection(element, scalar);
		output << scalar + 1e-20 << "\n";
	}

	output << "VECTORS Strain float\n";

	ChVector<> Strain(1e-20);
	for (unsigned int iele = 0; iele < my_mesh->GetNelements(); iele++) {
		if (auto element = std::dynamic_pointer_cast<ChElementTetra_4>(my_mesh->GetElement(iele)))
		{
			auto StrainV = element->GetStrain();
			output << StrainV.XX() + Strain.x() << " " << StrainV.YY() + Strain.y() << " " << StrainV.ZZ() + Strain.z() << "\n";
		}
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
