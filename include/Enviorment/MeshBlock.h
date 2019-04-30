#pragma once

#include <vector>
#include <stdexcept>
#include <functional>

#include "fea/ChMesh.h"
#include "fea/ChVisualizationFEAmesh.h"
#include "chrono/fea/ChElementTetra_4.h"

#include "BlockFace.h"


class MeshBlock
{
public:
	MeshBlock(const std::shared_ptr<chrono::fea::ChMesh> & mesh,
		const std::shared_ptr<chrono::fea::ChContinuumElastic> & material,
		const chrono::Vector & origin,
		const chrono::Vector & size);
	~MeshBlock() = default;
public:
	MeshBlock(const std::shared_ptr<chrono::fea::ChMesh> & mesh,
		const std::shared_ptr<chrono::fea::ChContinuumElastic> & material,
		const std::vector<BlockFace> & faces,
		const chrono::Vector & orientation,
		const chrono::Vector & size);
public:
	std::shared_ptr<MeshBlock> Expand(const chrono::Vector & orientation);

	const BlockFace & GetBottom() const;
	const BlockFace & GetTop() const;
	const BlockFace & GetLeft() const;
	const BlockFace & GetRight() const;
	const BlockFace & GetFront() const;
	const BlockFace & GetBack() const;

	const std::vector<BlockFace> & GetFaces() const;
	const std::shared_ptr<chrono::fea::ChMesh> & GetMesh() const;

private:
	void BuildBlock(const chrono::Vector & origin, bool expand = false);
	void BuildFaces(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & nodes);

	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> ConstructNodes(const chrono::Vector & origin);
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> ConstructExpandedNodes(const chrono::Vector & orientation);


	const BlockFace & GetCurrentFace(const chrono::Vector & orientation) const;

private:
	std::shared_ptr<chrono::fea::ChMesh> m_mesh;
	std::shared_ptr<chrono::fea::ChContinuumElastic> m_material;
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> m_nodes;
	std::vector<BlockFace> m_faces;

	chrono::Vector m_size;

};