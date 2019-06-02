#include "Enviorment/GraphicalObjects/Cuboid.h"

GraphicalObjects::Cuboid::Cuboid(const chrono::Vector & blockSize, const chrono::ChVector<int> & shapeSize)
{
	this->m_mesh = std::make_shared<chrono::fea::ChMesh>();
	this->m_physicsItems.emplace_back(this->m_mesh);
	this->m_blockSize = blockSize;
	this->m_size = shapeSize;
}

void GraphicalObjects::Cuboid::SetMaterial(const std::shared_ptr<chrono::fea::ChContinuumElastic>& material)
{
	this->m_material = material;
}

void GraphicalObjects::Cuboid::SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualization)
{
	this->m_mesh->AddAsset(visualization);
}


void GraphicalObjects::Cuboid::Build()
{
	try
	{
		auto nodes = CreateNodes();

		for (size_t levelIndex = 0; levelIndex < nodes.size() - 1; ++levelIndex)
		{
			for (size_t lineIndex = 0; lineIndex < nodes.front().size() - 1; ++lineIndex)
			{
				for (size_t columnIndex = 0; columnIndex < nodes.front().front().size() - 1; ++columnIndex)
				{
					BuildBlock
					({
						nodes[levelIndex][lineIndex][columnIndex],
						nodes[levelIndex][(lineIndex + 1)][columnIndex],
						nodes[levelIndex][lineIndex][(columnIndex + 1)],
						nodes[levelIndex][(lineIndex + 1)][(columnIndex + 1)],

						nodes[(levelIndex + 1)][lineIndex][columnIndex],
						nodes[(levelIndex + 1)][(lineIndex + 1)][columnIndex],
						nodes[(levelIndex + 1)][lineIndex][(columnIndex + 1)],
						nodes[(levelIndex + 1)][(lineIndex + 1)][(columnIndex + 1)],
						});
				}
			}
		}
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
}

std::shared_ptr<chrono::fea::ChMesh> GraphicalObjects::Cuboid::GetMesh() const
{
	return this->m_mesh;
}

std::vector<std::vector<std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>>> GraphicalObjects::Cuboid::CreateNodes()
{
	try
	{
		int length = this->m_size.x(), height = this->m_size.y(), width = this->m_size.z();
		double xSize = this->m_blockSize.x(), ySize = this->m_blockSize.y(), zSize = this->m_blockSize.z();
		std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> baseNodes;

		std::vector<std::vector<std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>>> nodes
		(height, std::vector<std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>>
			(width, std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>(length)));

		for (size_t levelIndex = 0; levelIndex < height; ++levelIndex)
		{
			for (size_t lineIndex = 0; lineIndex < width; ++lineIndex)
			{
				for (size_t columnIndex = 0; columnIndex < length; ++columnIndex)
				{
					auto node = std::make_shared<chrono::fea::ChNodeFEAxyz>(chrono::Vector(lineIndex * xSize, levelIndex * ySize, columnIndex * zSize));
					nodes[levelIndex][lineIndex][columnIndex] = node;
					this->m_mesh->AddNode(node);
					if (lineIndex == 0)
					{
						baseNodes.emplace_back(node);
					}
				}
			}
		}
		std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(this->m_mesh->GetNodes().back())->SetForce(chrono::Vector(0.0, -10.0, 0.0));
		std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(this->m_mesh->GetNodes().back())->SetMass(100.0);
		SetFixedBase(baseNodes);
		return nodes;
	}
	catch (const std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
}

void GraphicalObjects::Cuboid::BuildBlock(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & nodes)
{
	auto BuildTetra = [this](const auto & firstNode, const auto & secondNode, const auto & thirdNode, const auto & fourthNode)
	{
		auto tetra = std::make_shared<chrono::fea::ChElementTetra_4>();
		tetra->SetNodes(firstNode, secondNode, thirdNode, fourthNode);
		tetra->SetMaterial(this->m_material);
		this->m_mesh->AddElement(tetra);
	};

	BuildTetra(nodes[4], nodes[1], nodes[2], nodes[0]);
	BuildTetra(nodes[1], nodes[2], nodes[4], nodes[7]);
	BuildTetra(nodes[6], nodes[7], nodes[4], nodes[2]);
	BuildTetra(nodes[1], nodes[4], nodes[5], nodes[7]);
	BuildTetra(nodes[1], nodes[2], nodes[3], nodes[7]);
}

void GraphicalObjects::Cuboid::SetFixedBase(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & baseNodes)
{
	auto base = std::make_shared<chrono::ChBody>();
	base->SetBodyFixed(true);
	this->m_physicsItems.emplace_back(base);
	for (const auto & node : baseNodes)
	{
		auto constraint = std::make_shared<chrono::fea::ChLinkPointFrame>();
		constraint->Initialize(node, base);
		this->m_physicsItems.emplace_back(constraint);
	}
}
