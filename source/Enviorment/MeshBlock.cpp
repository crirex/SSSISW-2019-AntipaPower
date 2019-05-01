#include "MeshBlock.h"

MeshBlock::MeshBlock(const std::shared_ptr<chrono::fea::ChMesh> & mesh,
	const std::shared_ptr<chrono::fea::ChContinuumElastic> & material,
	const chrono::Vector & origin,
	const chrono::Vector & size)
{
	this->m_size = size;
	this->m_mesh = mesh;
	this->m_material = material;
	BuildBlock(origin);
}

MeshBlock::MeshBlock(const std::shared_ptr<chrono::fea::ChMesh>& mesh,
	const std::shared_ptr<chrono::fea::ChContinuumElastic>& material,
	const std::vector<BlockFace>& faces,
	const chrono::Vector & orientation,
	const chrono::Vector & size)
{
	this->m_size = size;
	this->m_mesh = mesh;
	this->m_material = material;
	this->m_faces = faces;
	BuildBlock(orientation,true);
}

std::shared_ptr<MeshBlock> MeshBlock::Expand(const chrono::Vector & orientation)
{
	auto newBlock = std::make_shared<MeshBlock>(this->m_mesh,
		this->m_material,
		this->m_faces,
		orientation,
		this->m_size);
	return newBlock;
}

const BlockFace & MeshBlock::GetBottom() const
{
	return this->m_faces[0];
}

const BlockFace & MeshBlock::GetTop() const
{
	return this->m_faces[1];
}

const BlockFace & MeshBlock::GetLeft() const
{
	return this->m_faces[5];
}

const BlockFace & MeshBlock::GetRight() const
{
	return this->m_faces[3];
}

const BlockFace & MeshBlock::GetFront() const
{
	return this->m_faces[4];
}

const BlockFace & MeshBlock::GetBack() const
{
	return this->m_faces[2];
}

const std::vector<BlockFace>& MeshBlock::GetFaces() const
{
	return this->m_faces;
}

const std::shared_ptr<chrono::fea::ChMesh>& MeshBlock::GetMesh() const
{
	return this->m_mesh;
}

void MeshBlock::BuildBlock(const chrono::Vector & origin, bool expand)
{
	this->m_nodes = expand ? ConstructExpandedNodes(origin) : ConstructNodes(origin);
	BuildFaces(this->m_nodes);

	auto BuildTetra = [this](const auto & firstNode, const auto & secondNode, const auto & thirdNode, const auto & fourthNode)
	{
		auto tetra = std::make_shared<chrono::fea::ChElementTetra_4>();
		tetra->SetNodes(firstNode, secondNode, thirdNode, fourthNode);
		tetra->SetMaterial(this->m_material);
		this->m_mesh->AddElement(tetra);
	};

	BuildTetra(this->m_nodes[0], this->m_nodes[1], this->m_nodes[6], this->m_nodes[7]);
	BuildTetra(this->m_nodes[0], this->m_nodes[1], this->m_nodes[2], this->m_nodes[6]);
	BuildTetra(this->m_nodes[0], this->m_nodes[1], this->m_nodes[3], this->m_nodes[7]);
	BuildTetra(this->m_nodes[6], this->m_nodes[7], this->m_nodes[0], this->m_nodes[4]);
	BuildTetra(this->m_nodes[6], this->m_nodes[7], this->m_nodes[1], this->m_nodes[5]);
}

void MeshBlock::BuildFaces(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> & nodes)
{
	this->m_faces.clear();
	auto bottom = BlockFace({ nodes.begin(), nodes.begin() + 4 });
	auto top = BlockFace({ nodes.begin() + 4, nodes.end() });
	this->m_faces.emplace_back(bottom);
	this->m_faces.emplace_back(top);

	this->m_faces.emplace_back(BlockFace({ bottom.GetNodes()[0], bottom.GetNodes()[2], top.GetNodes()[0], top.GetNodes()[2] }));//back
	this->m_faces.emplace_back(BlockFace({ bottom.GetNodes()[2], bottom.GetNodes()[1], top.GetNodes()[2], top.GetNodes()[1] }));//right
	this->m_faces.emplace_back(BlockFace({ bottom.GetNodes()[1], bottom.GetNodes()[3], top.GetNodes()[1], top.GetNodes()[3] }));//front
	this->m_faces.emplace_back(BlockFace({ bottom.GetNodes()[3], bottom.GetNodes()[0], top.GetNodes()[3], top.GetNodes()[0] }));//left
}


std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> MeshBlock::ConstructNodes(const chrono::Vector & origin)
{
	auto nodes = std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>({
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin),
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + this->m_size * (chrono::VECT_X + chrono::VECT_Z)),
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + this->m_size * chrono::VECT_X),
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + this->m_size * chrono::VECT_Z),

		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + this->m_size * chrono::VECT_Y),
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + this->m_size * (chrono::VECT_X + chrono::VECT_Y + chrono::VECT_Z)),
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + this->m_size * (chrono::VECT_X + chrono::VECT_Y)),
		std::make_shared<chrono::fea::ChNodeFEAxyz>(origin + this->m_size * (chrono::VECT_Y + chrono::VECT_Z)),
		});
	std::for_each(nodes.begin(), nodes.end(), [this](const auto & node) { this->m_mesh->AddNode(node); });
	return nodes;
}

std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> MeshBlock::ConstructExpandedNodes(const chrono::Vector & orientation)
{
	std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes;

	auto lastNodes = GetCurrentFace(orientation).GetNodes();
	nodes.insert(nodes.end(), lastNodes.begin(), lastNodes.end());

	std::for_each(lastNodes.begin(), lastNodes.end(), [&](const auto & node) {
		auto castana = std::dynamic_pointer_cast<chrono::fea::ChNodeFEAxyz>(node)->GetPos();
		auto newNode = std::make_shared<chrono::fea::ChNodeFEAxyz>(castana + orientation * this->m_size);
		nodes.push_back(newNode);
		this->m_mesh->AddNode(newNode);
	});

	nodes = RemapNodes(orientation, nodes);
	return nodes;
}

const BlockFace & MeshBlock::GetCurrentFace(const chrono::Vector & orientation) const
{
	if (orientation.x())
	{
		return orientation.x() > 0 ? GetRight() : GetLeft();
	}
	else if (orientation.y())
	{
		return orientation.y() > 0 ? GetTop() : GetBottom();
	}
	else if (orientation.z())
	{
		return orientation.z() > 0 ? GetFront() : GetBack();
	}
	else
	{
		throw std::invalid_argument("Invalid orientation vector");
	}
}

std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> MeshBlock::RemapNodes(const chrono::Vector & orientation ,  const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>& nodes) const
{
	if(orientation.x() > 0 )
		return std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>{nodes[0], nodes[5], nodes[4], nodes[1], nodes[2], nodes[7], nodes[6], nodes[3] };
	if(orientation.x() < 0)
		return std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>{nodes[5], nodes[0], nodes[1], nodes[4], nodes[7], nodes[2], nodes[3], nodes[6] };

	if(orientation.y() < 0)
		return std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>{nodes[4], nodes[5], nodes[6], nodes[7], nodes[0], nodes[1], nodes[2], nodes[3] };
	if(orientation.y() > 0)
		return std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>{nodes[0], nodes[1], nodes[2], nodes[3], nodes[4], nodes[5], nodes[6], nodes[7] };

	if(orientation.z() > 0 )
		return std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>{nodes[1], nodes[4], nodes[0], nodes[5], nodes[3], nodes[6], nodes[2], nodes[7] };
	if(orientation.z() < 0)
		return std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>>{nodes[4], nodes[1], nodes[5], nodes[0], nodes[6], nodes[3], nodes[7], nodes[2] };
}
