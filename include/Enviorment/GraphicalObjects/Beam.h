#pragma once

#include <iostream>
#include <fstream>
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
	class Beam : 
		public GraphicalObject
	{
	public:
		Beam(const chrono::Vector & blockSize, int blocks, const chrono::Vector & orientation = chrono::VECT_X, const chrono::Vector & origin = chrono::Vector(0, 0, 0));
		~Beam() = default;
	public:
		// Inherited via GraphicalObject
		virtual void Build() override;
		virtual std::shared_ptr<chrono::fea::ChMesh> GetMesh() const override;

	public:
		void SetMaterial(const std::shared_ptr<chrono::fea::ChContinuumElastic> & material);
		void SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & visualization);

	private:
		void BuildBlock(const chrono::Vector & origin, const chrono::Vector & size, const chrono::Vector & orientation);
		void SetFixedBase(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & baseNodes);

		std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> BuildBase(const chrono::Vector & origin, const chrono::Vector & orientation, const chrono::Vector & size);
		std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> ConstructBlockNodes(const chrono::Vector & origin, const chrono::Vector & orientation, const chrono::Vector & size);
	private:
		std::shared_ptr<chrono::fea::ChMesh> m_mesh;
		std::shared_ptr<chrono::fea::ChContinuumElastic> m_material;

		chrono::Vector m_orientation;
		chrono::Vector m_blockSize;
		chrono::Vector m_origin;
		int m_blocks;
	private:
		const uint8_t baseSize = 4;
	};
}