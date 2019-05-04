#pragma once

#include <array>
#include <vector>

#include <chrono/fea/ChNodeFEAxyz.h>

class BlockFace
{
public:
	static const uint8_t size = 4;

public:
	BlockFace(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & nodes);
	~BlockFace() = default;

public:
	const std::array<std::shared_ptr<chrono::fea::ChNodeFEAxyz>, size> & GetNodes() const;

private:
	std::array<std::shared_ptr<chrono::fea::ChNodeFEAxyz>, size> m_nodes;
};
