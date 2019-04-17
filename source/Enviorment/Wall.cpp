#include "../../AMMO/SSSISW-2019-AntipaPower/include/Enviorment/Wall.h"

Wall::Wall(double xSize, double ySize, double zSize, double density, bool collide, bool visualAsset)
{
	this->m_wall = std::make_shared<ChBodyEasyBox>(xSize, ySize, zSize, density, collide, visualAsset);
	this->m_wall->SetBodyFixed(true);
}

const std::shared_ptr<chrono::ChBodyEasyBox>& Wall::GetWall() const
{
	return this->m_wall;
}

void Wall::SetPosition(const chrono::Vector & position)
{
	this->m_wall->SetPos(position);
}

void Wall::SetRotation(double angle, const chrono::Vector & axis)
{
	this->m_wall->SetRot(Q_from_AngAxis(angle, axis));
}