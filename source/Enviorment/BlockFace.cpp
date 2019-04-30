#include "BlockFace.h"

BlockFace::BlockFace(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & nodes)
{
	for (uint8_t index = 0; index < BlockFace::size; ++index)
	{
		this->m_nodes[index] = nodes[index];
	}
}

const std::array<std::shared_ptr<chrono::fea::ChNodeFEAxyz>, BlockFace::size>& BlockFace::GetNodes() const
{
	return this->m_nodes;
}
