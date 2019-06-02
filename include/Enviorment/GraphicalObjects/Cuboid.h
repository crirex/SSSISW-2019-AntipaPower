#pragma once

#include <iostream>
#include <vector>

#include "GraphicalObject.h"

#include "fea/ChMesh.h"
#include "fea/ChElementBeamEuler.h"
#include "fea/ChVisualizationFEAmesh.h"
#include "chrono/physics/ChBody.h"
#include "chrono/assets/ChBoxShape.h"
#include "chrono/physics/ChSystemNSC.h"
#include "chrono/fea/ChElementTetra_4.h"
#include "chrono/fea/ChLinkPointFrame.h"

namespace GraphicalObjects
{
	class Cuboid :
		public GraphicalObject
	{
	public:
		Cuboid(std::shared_ptr<chrono::fea::ChMesh> mesh);
		Cuboid(const chrono::Vector & blockSize, const chrono::ChVector<int> & shapeSize);
		~Cuboid() = default;


	public:
		void SetMaterial(const std::shared_ptr<chrono::fea::ChContinuumElastic> & material);
		void SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & visualization);
		std::shared_ptr<chrono::fea::ChContinuumElastic> GetMaterial() const;
		std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> GetVisualisationMesh() const;


	public:
		// Inherited via GraphicalObject
		void Build() override;
		std::shared_ptr<chrono::fea::ChMesh> GetMesh() const override;
		void SetMesh(std::shared_ptr<chrono::fea::ChMesh> mesh);

	private:
		std::vector<std::vector<std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>>> CreateNodes();
		void BuildBlock(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & nodes);
		void SetFixedBase(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & baseNodes);

	private:
		std::shared_ptr<chrono::fea::ChMesh> m_mesh;
		std::shared_ptr<chrono::fea::ChContinuumElastic> m_material;
		std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> m_visualisationMesh;

		chrono::Vector m_blockSize;
		chrono::ChVector<int> m_size;
	};
}

