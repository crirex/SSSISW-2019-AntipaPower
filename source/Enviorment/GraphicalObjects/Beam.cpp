#include "Enviorment/GraphicalObjects/Beam.h"

GraphicalObjects::Beam::Beam(const chrono::Vector & blockSize, int blocks, const chrono::Vector & orientation, const chrono::Vector & origin)
{
	this->m_mesh = std::make_shared<chrono::fea::ChMesh>();
	this->m_physicsItems.emplace_back(this->m_mesh);

	this->m_blockSize = blockSize;
	this->m_blocks = blocks;
	this->m_orientation = orientation;
	this->m_origin = origin;
}

void GraphicalObjects::Beam::Build()
{
	for (uint16_t index = 0; index < this->m_blocks; ++index)
	{
		BuildBlock(this->m_origin, this->m_blockSize, this->m_orientation);
	}
}

std::shared_ptr<chrono::fea::ChMesh> GraphicalObjects::Beam::GetMesh() const
{
	return this->m_mesh;
}


void GraphicalObjects::Beam::SetMaterial(const std::shared_ptr<chrono::fea::ChContinuumElastic> & material)
{
	this->m_material = material;
}

void GraphicalObjects::Beam::SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & visualization)
{
	this->m_mesh->AddAsset(visualization);
}

void GraphicalObjects::Beam::BuildBlock(const chrono::Vector & origin, const chrono::Vector & size, const chrono::Vector & orientation)
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

std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> GraphicalObjects::Beam::BuildBase(const chrono::Vector & origin, const chrono::Vector & orientation, const chrono::Vector & size)
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

std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> GraphicalObjects::Beam::ConstructBlockNodes(const chrono::Vector & origin, const chrono::Vector & orientation, const chrono::Vector & size)
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


void GraphicalObjects::Beam::SetFixedBase(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & baseNodes)
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