#pragma once

#include <vector>

#include "fea/ChMesh.h"
#include "fea/ChElementBeamEuler.h"
#include "fea/ChVisualizationFEAmesh.h"
#include "chrono/physics/ChBody.h"
#include "chrono/assets/ChBoxShape.h"
#include "chrono/physics/ChSystemNSC.h"
#include "chrono/fea/ChElementTetra_4.h"
#include "chrono/fea/ChLinkPointFrame.h"

#include "MeshBlock.h"

class Cuboid
{

public:
	Cuboid(chrono::ChSystemNSC & system);
	Cuboid(chrono::ChSystemNSC & system,
		const chrono::ChVector<> & blockSize,
		const chrono::ChVector<> & size = chrono::Vector(1, 1, 1));
	~Cuboid() = default;

public:
	///Getters
	const std::shared_ptr<chrono::fea::ChMesh> & GetMesh() const;

	///Setters
	void SetMaterial(const std::shared_ptr<chrono::fea::ChContinuumElastic> & material);
	void SetVisualizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> & visualization);

public:
	void Build(const chrono::Vector & orientation = chrono::VECT_Y, const chrono::Vector & origin = chrono::Vector(0, 0, 0));

private:
	void SetFixedBase(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & baseNodes);

private:
	chrono::ChSystemNSC & m_refSystem;

	std::shared_ptr<chrono::ChBody> m_base;
	std::shared_ptr<chrono::fea::ChMesh> m_mesh;
	std::shared_ptr<chrono::fea::ChContinuumElastic> m_material;

	chrono::Vector m_size;
	chrono::Vector m_density;
};


