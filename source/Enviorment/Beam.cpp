#include "Beam.h"

Beam::Beam(chrono::ChSystemNSC & system) : m_refSystem(system)
{
	this->m_mesh = std::make_shared<chrono::fea::ChMesh>();
	this->m_refSystem.Add(this->m_mesh);
}

const std::shared_ptr<chrono::fea::ChMesh> & Beam::GetMesh() const
{
	return this->m_mesh;
}

void Beam::SetMaterial(const std::shared_ptr<chrono::fea::ChContinuumElastic> & material)
{
	this->m_material = material;
}

void Beam::SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & visualization)
{
	this->m_mesh->AddAsset(visualization);
}

void Beam::Build(const chrono::Vector & blockSize, int blocks, const chrono::Vector & orientation, const chrono::Vector & origin)
{
	this->m_blockSize = blockSize;
	this->m_orientation = orientation;
	this->m_blocks = blocks;
	for (uint16_t index = 0; index < blocks; ++index)
	{
		BuildBlock(origin, blockSize, orientation);
	}
}

void Beam::StartLogStrained() const
{
	std::ofstream out;
	out.open("node_info.txt", std::ios::out | std::ios::trunc);
	for (unsigned int iele = 0; iele < this->m_mesh->GetNelements(); iele++)
	{
		if (auto element = std::dynamic_pointer_cast<chrono::fea::ChElementTetra_4>(this->m_mesh->GetElement(iele)))
		{
			auto StrainV = element->GetStrain();
			out << "Strain On Thetra No: " << iele << ", X: " << StrainV.XX() << " , Y: " << StrainV.YY() << ", Z: " << StrainV.ZZ() << std::endl;
			for (size_t nodeIndex = 0; nodeIndex < element->GetNnodes(); ++nodeIndex)
			{
				auto node = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(element->GetNodeN(nodeIndex));
				out << "Node: " << node->GetIndex() << ", Pos X : " << node->GetPos().x() << ", Y : " << node->GetPos().y() << ", Z : " << node->GetPos().z() << std::endl;
			}
		}
	}
	out.close();
}

void Beam::BuildBlock(const chrono::Vector & origin, const chrono::Vector & size, const chrono::Vector & orientation)
{
	auto nodes = ConstructBlockNodes(origin, orientation, size);

	auto BuildTetra = [this](const auto & firstNode, const auto & secondNode, const auto & thirdNode, const auto & fourthNode)
	{
		auto tetra = std::make_shared<chrono::fea::ChElementTetra_4>();
		tetra->SetNodes(firstNode, secondNode, thirdNode, fourthNode);
		tetra->SetMaterial(this->m_material);
		this->m_mesh->AddElement(tetra);
	};

	BuildTetra(nodes[0], nodes[1], nodes[6], nodes[7]);
	BuildTetra(nodes[0], nodes[1], nodes[2], nodes[6]);
	BuildTetra(nodes[0], nodes[1], nodes[3], nodes[7]);
	BuildTetra(nodes[6], nodes[7], nodes[0], nodes[4]);
	BuildTetra(nodes[6], nodes[7], nodes[1], nodes[5]);
}

std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> Beam::BuildBase(const chrono::Vector & origin, const chrono::Vector & orientation, const chrono::Vector & size)
{
	auto base = chrono::Vector(1) - orientation;
	auto nodes = std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>({
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin),
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + size * base),
		});
	if (base.x())
	{
		nodes.push_back(std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + size * chrono::VECT_X));
	}
	if (base.y())
	{
		nodes.push_back(std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + size * chrono::VECT_Y));
	}
	if (base.z())
	{
		nodes.push_back(std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + size * chrono::VECT_Z));
	}
	assert(nodes.size() == baseSize);
	return nodes;
}

std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> Beam::ConstructBlockNodes(const chrono::Vector & origin, const chrono::Vector & orientation, const chrono::Vector & size)
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes;
	if (!this->m_mesh->GetNodes().size())
	{
		auto baseNodes = BuildBase(origin, orientation, size);
		if (true)
		{
			SetFixedBase(baseNodes);
		}
		std::for_each(baseNodes.begin(), baseNodes.end(), [&](auto & node) {
			nodes.push_back(node);
			this->m_mesh->AddNode(node);
		});
	}
	else
	{
		std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>> lastNodes(this->m_mesh->GetNodes().end() - baseSize, this->m_mesh->GetNodes().end());
		std::for_each(lastNodes.begin(), lastNodes.end(), [&](const auto & node) {
			nodes.push_back(std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(node));
		});
	}
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAbase>> lastNodes(this->m_mesh->GetNodes().end() - baseSize, this->m_mesh->GetNodes().end());
	std::for_each(lastNodes.begin(), lastNodes.end(), [&](const auto & node) {
		auto newNode = std::make_shared<chrono::fea::ChNodeFEAxyz>(std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(node)->GetPos() + orientation * size);
		nodes.push_back(newNode);
		this->m_mesh->AddNode(newNode);
	});

	return nodes;
}

void Beam::SetFixedBase(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & baseNodes)
{
	this->m_base = std::make_shared<chrono::ChBody>();
	this->m_base->SetBodyFixed(true);
	this->m_refSystem.Add(this->m_base);
	for (const auto & node : baseNodes)
	{
		auto constraint = std::make_shared<chrono::fea::ChLinkPointFrame>();
		constraint->Initialize(node, this->m_base);
		this->m_refSystem.Add(constraint);
	}
}