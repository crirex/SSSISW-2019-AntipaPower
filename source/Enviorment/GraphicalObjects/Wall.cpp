#include "Enviorment/GraphicalObjects/Wall.h"

GraphicalObjects::Wall::Wall(double xSize, double ySize, double zSize, double density, bool collide, bool visualAsset)
{
	this->m_wall = std::make_shared<chrono::ChBodyEasyBox>(xSize, ySize, zSize, density, collide, visualAsset);
	this->m_wall->SetBodyFixed(true);
	this->m_physicsItems.emplace_back(this->m_wall);
}

void GraphicalObjects::Wall::SetPosition(const chrono::Vector & position)
{
	this->m_wall->SetPos(position);
}

void GraphicalObjects::Wall::SetRotation(double angle, const chrono::Vector & axis)
{
	this->m_wall->SetRot(Q_from_AngAxis(angle, axis));
}

void GraphicalObjects::Wall::Build()
{
	//de mutat aici constructia
}

std::shared_ptr<chrono::fea::ChMesh> GraphicalObjects::Wall::GetMesh() const
{
	return nullptr;
}