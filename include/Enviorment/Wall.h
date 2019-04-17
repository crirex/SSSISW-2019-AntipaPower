#pragma once

#include "chrono/physics/ChBodyEasy.h"

class Wall
{
public:
	Wall(double xSize, double ySize, double zSize, double density, bool collide, bool visualAsset);
	~Wall() = default;

public: 
	const std::shared_ptr<chrono::ChBodyEasyBox> & GetWall() const;

	void SetPosition(const chrono::Vector & position);
	void SetRotation(double angle, const chrono::Vector & axis);

private:
	std::shared_ptr<chrono::ChBodyEasyBox> m_wall;
};

