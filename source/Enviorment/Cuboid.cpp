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
	MeshBlock meshBlock(this->m_mesh, this->m_material, chrono::Vector(0, 0, 0), chrono::Vector(1, 1, 1));
	for (int index = 0; index < meshBlock.GetFaces().size(); ++index)
	{
		std::cout << "Face number " << index << ": \n\t";
		for (const auto & node : meshBlock.GetFaces()[index].GetNodes())
		{
			std::cout << "(" << node->GetPos().x() << ", " << node->GetPos().y() << ", " << node->GetPos().z() << ") ";
		}
		std::cout << "\n";
	}
	for (uint16_t index = 0; index < this->m_density.z(); ++index)
	{
		//eroare
		//meshBlock.Expand(orientation);
	}
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