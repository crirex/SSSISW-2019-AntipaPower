#pragma once

#include "chrono/physics/ChBodyEasy.h"

#include "Enviorment/GraphicalObjects/GraphicalObject.h"

namespace GraphicalObjects
{
	class Wall :
		public GraphicalObject
	{
	public:
		Wall(double xSize, double ySize, double zSize, double density, bool collide, bool visualAsset);
		~Wall() = default;


	public:
		// Inherited via GraphicalObject
		void Build() override;
		std::shared_ptr<chrono::fea::ChMesh> GetMesh() const override;

	public:
		void SetPosition(const chrono::Vector & position);
		void SetRotation(double angle, const chrono::Vector & axis);

	private:
		std::shared_ptr<chrono::ChBodyEasyBox> m_wall;
	};
}

