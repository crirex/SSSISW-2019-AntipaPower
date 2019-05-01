#include "Cuboid.h"

Cuboid::Cuboid(chrono::ChSystemNSC & system) : m_refSystem(system)
{
	this->m_mesh = std::make_shared<chrono::fea::ChMesh>();
	this->m_refSystem.Add(this->m_mesh);
}

Cuboid::Cuboid(chrono::ChSystemNSC & system, const chrono::ChVector<> & blockSize, const chrono::ChVector<> & size) :
	Cuboid(system)
{
	this->m_size = blockSize;
	this->m_density = size;
}

const std::shared_ptr<chrono::fea::ChMesh> & Cuboid::GetMesh() const
{
	return this->m_mesh;
}

void Cuboid::SetMaterial(const std::shared_ptr<chrono::fea::ChContinuumElastic> & material)
{
	this->m_material = material;
}

void Cuboid::SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & visualization)
{
	this->m_mesh->AddAsset(visualization);
}

void Cuboid::Build(const chrono::Vector & orientation, const chrono::Vector & origin)
{
	auto blockSize = this->m_size / this->m_density;
	auto meshBlock = std::make_shared<MeshBlock>(this->m_mesh, this->m_material, chrono::Vector(0, 0, 0), chrono::Vector(1, 1, 1));

	this->SetFixedBase({ meshBlock->GetBottom().GetNodes().begin(), meshBlock->GetBottom().GetNodes().end() });

	this->m_meshBlocks.emplace_back(meshBlock);

	ConstructShape(chrono::VECT_X);
	ConstructShape(chrono::VECT_Y);
	ConstructShape(chrono::VECT_Z);
}


void Cuboid::SetFixedBase(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & baseNodes)
{
	this->m_base = std::make_shared<chrono::ChBody>();
	this->m_base->SetBodyFixed(true);
	this->m_refSystem.Add(this->m_base);
	for (const auto & node : baseNodes)
	{
		auto constraint = std::make_shared<chrono::fea::ChLinkPointFrame>();
		constraint->Initialize(node, this->m_base);
		this->m_refSystem.Add(constraint);

		auto mboxfloor = std::make_shared<chrono::ChBoxShape>();
		mboxfloor->GetBoxGeometry().Size = chrono::ChVector<>(0.005);
		constraint->AddAsset(mboxfloor);
	}
}

void Cuboid::ConstructShape(const chrono::Vector & orientation)
{
	auto vec = orientation * this->m_density;
	uint16_t size = 0;
	for (uint16_t index = 0; index < vec.Length(); ++index)
	{
		if (vec[index] != 0)
		{
			size = vec[index];
			break;
		}
	}
	for (const auto & block : this->m_meshBlocks)
	{
		auto copyBlock = block;
		for (uint16_t index = 0; index < size; ++index)
		{
			copyBlock = copyBlock->Expand(orientation);
			this->m_meshBlocks.emplace_back(copyBlock);
		}
	}
}
