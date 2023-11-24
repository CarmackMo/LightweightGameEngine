// Includes
//=========

#include <Engine/Graphics/Graphics.h>
#include <Engine/Physics/cBVHTree.h>
#include <Engine/Physics/Collision.h>

#include <queue>



// sBVHNode Implementation
//==================

eae6320::Physics::sBVHNode::sBVHNode() :
	parent(nullptr), collider(nullptr), childrenCrossed(false)
{
	children[0] = nullptr;
	children[1] = nullptr;
}


bool eae6320::Physics::sBVHNode::IsLeaf() const
{
	return children[0] == nullptr;
}


void eae6320::Physics::sBVHNode::SetBranch(sBVHNode* i_node0, sBVHNode* i_node1)
{
	i_node0->parent = this;
	i_node1->parent = this;

	children[0] = i_node0;
	children[1] = i_node1;
}


void eae6320::Physics::sBVHNode::SetLeaf(cCollider* i_collider)
{
	this->collider = i_collider;

	children[0] = nullptr;
	children[1] = nullptr;
}


void eae6320::Physics::sBVHNode::UpdateFatAABB(float i_margin)
{
	if (IsLeaf())
	{
		// make fat AABB, the min/max extent directly represents the world cooridnate of 
		// the associated collider. Thus the fat AABB should have ZERO world position
		const Math::sVector marginVec(i_margin, i_margin, i_margin);
		fatAABB.UpdateExtents(collider->GetMinExtent_world() - marginVec, collider->GetMaxExtent_world() + marginVec);
	}
	else
	{
		// make union of child nodes' AABB
		fatAABB = children[0]->fatAABB.Union(children[1]->fatAABB);
	}
}


eae6320::Physics::sBVHNode* eae6320::Physics::sBVHNode::GetSibling() const
{
	return (this == parent->children[0]) ?
		parent->children[1] :
		parent->children[0];
}


// cBVHTree Implementation
//==================

eae6320::Physics::sBVHNode* eae6320::Physics::cBVHTree::Search(cCollider* i_collider)
{
	if (m_root == nullptr)
		return nullptr;

	std::queue<sBVHNode*> container;
	container.push(m_root);

	while (container.empty() == false)
	{
		sBVHNode* current = container.front();
		container.pop();

		if (current->collider == i_collider)
		{
			return current;
		}
		else if (current->IsLeaf() == false)
		{
			if (Collision::IsOverlaps(&(current->children[0]->fatAABB), i_collider))
				container.push(current->children[0]);
			if (Collision::IsOverlaps(&(current->children[1]->fatAABB), i_collider))
				container.push(current->children[1]);
		}
	}

	return nullptr;
}


void eae6320::Physics::cBVHTree::Add(cCollider* i_collider)
{
	if (m_root != nullptr)
	{
		// if this is not the first node of the tree, insert node to tree
		sBVHNode* node = new sBVHNode();
		node->SetLeaf(i_collider);
		node->UpdateFatAABB(m_margin);
		InsertNode(node, &m_root);

		// Create two new debug cLine object, one for new node, the other for branch node
		{
			Graphics::cLine* newNodeLine = nullptr;
			m_renderData.push_back(std::pair<Graphics::cLine*, Math::cMatrix_transformation>(newNodeLine, Math::cMatrix_transformation()));
			RenderInitializeHelper(m_renderData.back().first);

			Graphics::cLine* branchNodeLine = nullptr;
			m_renderData.push_back(std::pair<Graphics::cLine*, Math::cMatrix_transformation>(newNodeLine, Math::cMatrix_transformation()));
			RenderInitializeHelper(m_renderData.back().first);
		}
	}
	else
	{
		// if this is the first node, make it root
		m_root = new sBVHNode();
		m_root->SetLeaf(i_collider);
		m_root->UpdateFatAABB(m_margin);

		// Create one new debug cLine objec for the new node
		{
			Graphics::cLine* newNodeLine = nullptr;
			m_renderData.push_back(std::pair<Graphics::cLine*, Math::cMatrix_transformation>(newNodeLine, Math::cMatrix_transformation()));
			RenderInitializeHelper(m_renderData.back().first);
		}
	}
}


void eae6320::Physics::cBVHTree::Remove(cCollider* i_collider)
{
	sBVHNode* node = Search(i_collider);
	node->collider = nullptr;
	RemoveNode(node);
}


void eae6320::Physics::cBVHTree::Update()
{
	if (m_root != nullptr)
	{
		if (m_root->IsLeaf())
		{
			m_root->UpdateFatAABB(m_margin);
		}
		else
		{
			// grab all invalid nodes
			m_invalidNodes.clear();
			UpdateInvalidNodeHelper(m_root, m_invalidNodes);

			// re-insert all invalid nodes
			for (sBVHNode* node : m_invalidNodes)
			{
				// grab parent link (pointer to another pointer that points to parent)
				sBVHNode* parent = node->parent;
				sBVHNode* sibling = node->GetSibling();
				sBVHNode** parentLink = (parent->parent != nullptr) ?
					(parent == parent->parent->children[0] ? &parent->parent->children[0] : &parent->parent->children[1]) :
					&m_root;

				// replace parent with sibling
				sibling->parent = (parent->parent != nullptr) ?
					parent->parent : 
					nullptr;		// if sibling's parent is root, root has null parent

				// let grand parent's child points to sibling (which was originally pointed to the parent of the sibling)
				*parentLink = sibling;
				delete parent;

				// re-insert node
				node->UpdateFatAABB(m_margin);
				InsertNode(node, &m_root);
			}

			m_invalidNodes.clear();
		}
	}

	// Update rendering data
	{
		RenderUpdateHelper();
	}
}


std::list<std::pair<eae6320::Physics::cCollider*, eae6320::Physics::cCollider*>>& eae6320::Physics::cBVHTree::ComputePairs()
{
	m_pairs.clear();

	if (m_root == nullptr || m_root->IsLeaf())
		return m_pairs;

	// clear Node::childrenCrossed flags
	ClearChildrenCrossFlagHelper(m_root);

	// base recursive call
	ComputePairsHelper(m_root->children[0], m_root->children[1]);

	return m_pairs;
}


std::vector<eae6320::Physics::cCollider*> eae6320::Physics::cBVHTree::Query(cCollider* i_collider) const
{
	std::vector<cCollider*> result = std::vector<cCollider*>(0);
	std::queue<sBVHNode*> container;

	if (m_root == nullptr)
		return result;

	container.push(m_root);
	while (container.empty() == false)
	{
		sBVHNode* current = container.front();
		container.pop();

		if (current->IsLeaf())
		{
			if (Collision::IsOverlaps(i_collider, current->collider) && i_collider != current->collider)
				result.push_back(current->collider);
		}
		else
		{
			if (Collision::IsOverlaps(i_collider, dynamic_cast<cCollider*>(&current->children[0]->fatAABB)))
				container.push(current->children[0]);
			if (Collision::IsOverlaps(i_collider, dynamic_cast<cCollider*>(&current->children[1]->fatAABB)))
				container.push(current->children[1]);
		}
	}

	return result;
}


void eae6320::Physics::cBVHTree::InitialzieRenderData()
{
	m_renderData.clear();

	if (m_root == nullptr)
		return;

	std::queue<sBVHNode*> container;
	container.push(m_root);

	while (container.empty() == false)
	{
		sBVHNode* current = container.front();
		container.pop();

		Graphics::cLine* fatAABBLine = nullptr;
		RenderInitializeHelper(fatAABBLine);
		m_renderData.push_back(std::pair<Graphics::cLine*, Math::cMatrix_transformation>(fatAABBLine, Math::cMatrix_transformation()));

		if (current->IsLeaf() == false)
		{
			container.push(current->children[0]);
			container.push(current->children[1]);
		}
	}
}


std::vector<std::pair<eae6320::Graphics::cLine*, eae6320::Math::cMatrix_transformation>>& eae6320::Physics::cBVHTree::GetRenderData()
{
	return m_renderData;
}


void eae6320::Physics::cBVHTree::InsertNode(sBVHNode* i_node, sBVHNode** i_parent)
{
	sBVHNode* p = *i_parent;

	// If parent is leaf node, simply split
	if (p->IsLeaf())
	{
		sBVHNode* newParent = new sBVHNode();
		newParent->parent = p->parent;
		newParent->SetBranch(i_node, p);
		*i_parent = newParent;
	}
	// If parent is branch node, compute volume differences between pre-insert and post-insert
	else
	{
		const cAABBCollider& aabb0 = p->children[0]->fatAABB;
		const cAABBCollider& aabb1 = p->children[1]->fatAABB;

		float volumeDiff0 = aabb0.Union(i_node->fatAABB).GetVolume() - aabb0.GetVolume();
		float volumeDiff1 = aabb1.Union(i_node->fatAABB).GetVolume() - aabb1.GetVolume();

		// insert to the child that gives less volume increase
		if (volumeDiff0 < volumeDiff1)
			InsertNode(i_node, &p->children[0]);
		else
			InsertNode(i_node, &p->children[1]);
	}

	// update parent AABB (propagates back up the recursion stack)
	(*i_parent)->UpdateFatAABB(m_margin);
}


void eae6320::Physics::cBVHTree::RemoveNode(sBVHNode* i_node)
{
	// replace parent with sibling, remove parent node
	sBVHNode* parent = i_node->parent;

	// if current node is not root
	if (parent != nullptr)
	{
		sBVHNode* sibling = i_node->GetSibling();

		// if there is a grandparent, update sibling with the grandparent
		if (parent->parent != nullptr)
		{
			sibling->parent = parent->parent;
			(parent == parent->parent->children[0] ?
				parent->parent->children[0] :
				parent->parent->children[1]) = sibling;
		}
		// if there is no grandparent, make sibling root
		else
		{
			m_root = sibling;
			sibling->parent = nullptr;
		}

		delete i_node;
		delete parent;

		m_renderData.pop_back();
		m_renderData.pop_back();
	}
	// if current node is root
	else
	{
		m_root = nullptr;
		delete parent;

		m_renderData.pop_back();
	}
}


void eae6320::Physics::cBVHTree::UpdateInvalidNodeHelper(sBVHNode* i_node, std::vector<sBVHNode*>& i_invalidNodes)
{
	std::queue<sBVHNode*> container;
	container.push(i_node);

	while (container.empty() == false)
	{
		sBVHNode* current = container.front();
		container.pop();

		// check if fat AABB doesn't contain the collider's AABB anymore
		if (current->IsLeaf())
		{
			if (current->fatAABB.IsContains(*current->collider) == false)
				i_invalidNodes.push_back(current);
		}
		else
		{
			container.push(current->children[0]);
			container.push(current->children[1]);
		}
	}
}


void eae6320::Physics::cBVHTree::ComputePairsHelper(sBVHNode* i_node0, sBVHNode* i_node1)
{
	/*
	* 2 Leaf Nodes ¨C We¡¯ve reached the end of the tree, simply check the AABBs of the corresponding 
	*	colliders and possibly add them to the pair list
	* 
	* 1 Leaf Node plus 1 Branch Node ¨C Cross check the child nodes of the branch node (make a recursive 
	*	call with the child nodes), and make recursive calls with the leaf node against each child nodes 
	*	of the branch node.
	* 
	* 2 Branch Nodes ¨C Make a recursive call on every combination of 2 nodes out of the 4 child nodes.
	*/

	if (i_node0->IsLeaf())
	{
		// 2 leaves, check proxies instead of fat AABBs
		if (i_node1->IsLeaf())
		{
			if (Collision::IsOverlaps(i_node0->collider, i_node1->collider))
			{
				m_pairs.push_back(std::pair<cCollider*, cCollider*>(i_node0->collider, i_node1->collider));
			}
		}
		// 1 branch / 1 leaf, 2 cross checks
		else
		{
			CrossChildren(i_node1);
			ComputePairsHelper(i_node0, i_node1->children[0]);
			ComputePairsHelper(i_node0, i_node1->children[1]);
		}
	}
	else
	{
		// 1 branch / 1 leaf, 2 cross checks
		if (i_node1->IsLeaf())
		{
			CrossChildren(i_node0);
			ComputePairsHelper(i_node0->children[0], i_node1);
			ComputePairsHelper(i_node0->children[1], i_node1);
		}
		// 2 branches, 4 cross checks
		else
		{
			CrossChildren(i_node0);
			CrossChildren(i_node1);
			ComputePairsHelper(i_node0->children[0], i_node1->children[0]);
			ComputePairsHelper(i_node0->children[0], i_node1->children[1]);
			ComputePairsHelper(i_node0->children[1], i_node1->children[0]);
			ComputePairsHelper(i_node0->children[1], i_node1->children[1]);
		}
	}
}


void eae6320::Physics::cBVHTree::ClearChildrenCrossFlagHelper(sBVHNode* i_node)
{
	i_node->childrenCrossed = false;
	if (i_node->IsLeaf() == false)
	{
		ClearChildrenCrossFlagHelper(i_node->children[0]);
		ClearChildrenCrossFlagHelper(i_node->children[1]);
	}
}


void eae6320::Physics::cBVHTree::CrossChildren(sBVHNode* i_node)
{
	if (i_node->childrenCrossed == false)
	{
		ComputePairsHelper(i_node->children[0], i_node->children[1]);
		i_node->childrenCrossed = true;
	}
}


void eae6320::Physics::cBVHTree::RenderInitializeHelper(Graphics::cLine*& io_AABBLine)
{
	// Vertex data
	constexpr uint32_t vertexCount = 24;
	Graphics::VertexFormats::sVertex_line vertexData[vertexCount];
	{
		Math::sVector color = Math::sVector(0.0f, 1.0f, 1.0f);

		// 0-1
		vertexData[0] = Graphics::VertexFormats::sVertex_line(-0.5f, -0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[1] = Graphics::VertexFormats::sVertex_line(-0.5f, 0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		// 1-2
		vertexData[2] = Graphics::VertexFormats::sVertex_line(-0.5f, 0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[3] = Graphics::VertexFormats::sVertex_line(-0.5f, 0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		// 2-3
		vertexData[4] = Graphics::VertexFormats::sVertex_line(-0.5f, 0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[5] = Graphics::VertexFormats::sVertex_line(-0.5f, -0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		// 3-0
		vertexData[6] = Graphics::VertexFormats::sVertex_line(-0.5f, -0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[7] = Graphics::VertexFormats::sVertex_line(-0.5f, -0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		// 4-5
		vertexData[8] = Graphics::VertexFormats::sVertex_line(0.5f, -0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[9] = Graphics::VertexFormats::sVertex_line(0.5f, 0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		// 5-6
		vertexData[10] = Graphics::VertexFormats::sVertex_line(0.5f, 0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[11] = Graphics::VertexFormats::sVertex_line(0.5f, 0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		// 6-7
		vertexData[12] = Graphics::VertexFormats::sVertex_line(0.5f, 0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[13] = Graphics::VertexFormats::sVertex_line(0.5f, -0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		// 7-4
		vertexData[14] = Graphics::VertexFormats::sVertex_line(0.5f, -0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[15] = Graphics::VertexFormats::sVertex_line(0.5f, -0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		// 1-5
		vertexData[16] = Graphics::VertexFormats::sVertex_line(-0.5f, 0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[17] = Graphics::VertexFormats::sVertex_line(0.5f, 0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		// 2-6
		vertexData[18] = Graphics::VertexFormats::sVertex_line(-0.5f, 0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[19] = Graphics::VertexFormats::sVertex_line(0.5f, 0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		// 0-4
		vertexData[20] = Graphics::VertexFormats::sVertex_line(-0.5f, -0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[21] = Graphics::VertexFormats::sVertex_line(0.5f, -0.5f, -0.5f, color.x, color.y, color.z, 1.0f);
		// 3-7
		vertexData[22] = Graphics::VertexFormats::sVertex_line(-0.5f, -0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
		vertexData[23] = Graphics::VertexFormats::sVertex_line(0.5f, -0.5f, 0.5f, color.x, color.y, color.z, 1.0f);
	}

	// Index data
	constexpr uint32_t indexCount = vertexCount;
	uint16_t indexData[indexCount];
	for (uint32_t i = 0; i < indexCount; i++)
	{
		indexData[i] = i;
	}

	// Send cLine data to rendering thread for initialization
	if (Graphics::AcquireRenderObjectInitMutex() == WAIT_OBJECT_0)
	{
		Graphics::cLine** linePtr = &(io_AABBLine);
		Graphics::AddLineInitializeTask(linePtr, vertexData, vertexCount, indexData, indexCount);
		Graphics::ReleaseRenderObjectInitMutex();
	}
}


void eae6320::Physics::cBVHTree::RenderUpdateHelper()
{
	if (m_root == nullptr)
		return;

	std::queue<sBVHNode*> container;
	container.push(m_root);
	int index = 0;

	while (container.empty() == false)
	{
		sBVHNode* current = container.front();
		container.pop();

		// Update aabb line transformation matrix
		Math::sVector scale = current->fatAABB.GetMaxExtent_local() - current->fatAABB.GetMinExtent_local();
		Math::sVector worldPos = current->fatAABB.GetCentroid_world();
		m_renderData[index].second = Math::cMatrix_transformation(scale, worldPos);

		index++;

		if (current->IsLeaf() == false)
		{
			container.push(current->children[0]);
			container.push(current->children[1]);
		}
	}
}