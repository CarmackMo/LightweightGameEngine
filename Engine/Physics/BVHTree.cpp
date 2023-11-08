// Includes
//=========

#include <Engine/Physics/BVHTree.h>
#include <Engine/Asserts/Asserts.h>
#include <Engine/Physics/Collision.h>

// TODO: Temporary code
#include <queue>



// sBVHNode Implementation
//==================

eae6320::Physics::sBVHNode::sBVHNode() :
	parent(nullptr), data(nullptr), childrenCrossed(false)
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


void eae6320::Physics::sBVHNode::SetLeaf(cCollider* i_data)
{
	// create two-way link
	this->data = i_data;
	//data->userData = this;

	children[0] = nullptr;
	children[1] = nullptr;
}


void eae6320::Physics::sBVHNode::UpdateAABB(float i_margin)
{
	if (IsLeaf())
	{
		// make fat AABB
		const Math::sVector marginVec(i_margin, i_margin, i_margin);
		aabb.m_min = data->GetMinExtent_world() - marginVec;
		aabb.m_max = data->GetMaxExtent_world() + marginVec;
	}
	else
	{
		// make union of child nodes' AABB
		aabb = children[0]->aabb.Union(children[1]->aabb);
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

		if (current->data == i_collider)
		{
			return current;
		}
		else if (current->IsLeaf() == false)
		{
			if (current->children[0]->aabb.IsContains(*i_collider))
				container.push(current->children[0]);
			else if (current->children[1]->aabb.IsContains(*i_collider))
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
		node->UpdateAABB(m_margin);
		InsertNode(node, &m_root);
	}
	else
	{
		// if this is the first node, make it root
		m_root = new sBVHNode();
		m_root->SetLeaf(i_collider);
		m_root->UpdateAABB(m_margin);
	}
}


void eae6320::Physics::cBVHTree::Remove(cCollider* i_collider)
{
	sBVHNode* node = Search(i_collider);
	node->data = nullptr;
	RemoveNode(node);
}


void eae6320::Physics::cBVHTree::Update()
{
	if (m_root != nullptr)
	{
		if (m_root->IsLeaf())
		{
			m_root->UpdateAABB(m_margin);
		}
		else
		{
			// grab all invalid nodes
			m_invalidNodes.clear();
			UpdateNodeHelper(m_root, m_invalidNodes);

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
				node->UpdateAABB(m_margin);
				InsertNode(node, &m_root);
			}

			m_invalidNodes.clear();
		}
	}
}


ColliderPairList& eae6320::Physics::cBVHTree::ComputePairs()
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
		const cAABBCollider& aabb0 = p->children[0]->aabb;
		const cAABBCollider& aabb1 = p->children[1]->aabb;

		float volumeDiff0 = aabb0.Union(i_node->aabb).GetVolume() - aabb0.GetVolume();
		float volumeDiff1 = aabb1.Union(i_node->aabb).GetVolume() - aabb1.GetVolume();

		// insert to the child that gives less volume increase
		if (volumeDiff0 < volumeDiff1)
			InsertNode(i_node, &p->children[0]);
		else
			InsertNode(i_node, &p->children[1]);
	}

	// update parent AABB (propagates back up the recursion stack)
	(*i_parent)->UpdateAABB(m_margin);
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
	}
	// if current node is root
	else
	{
		m_root = nullptr;
		delete parent;
	}
}


void eae6320::Physics::cBVHTree::UpdateNodeHelper(sBVHNode* i_node, std::vector<sBVHNode*>& i_invalidNodes)
{
	// Recursive apporach
	{
		if (i_node->IsLeaf())
		{
			// check if fat AABB doesn't contain the collider's AABB anymore
			if (i_node->aabb.IsContains(*i_node->data) == false)
				i_invalidNodes.push_back(i_node);
		}
		else
		{
			UpdateNodeHelper(i_node->children[0], i_invalidNodes);
			UpdateNodeHelper(i_node->children[1], i_invalidNodes);
		}
	}

	//// Iterative approach, using Breadth First Search
	//{
	//	std::queue<sBVHNode*> container;
	//	container.push(i_node);

	//	while (container.empty() == false)
	//	{
	//		sBVHNode* current = container.front();
	//		container.pop();

	//		// check if fat AABB doesn't contain the collider's AABB anymore
	//		if (current->IsLeaf())
	//		{
	//			if (current->aabb.IsContains(*current->data) == false)
	//				i_invalidNodes.push_back(current);
	//		}
	//		else
	//		{
	//			container.push(current->children[0]);
	//			container.push(current->children[1]);
	//		}
	//	}
	//}
}


void eae6320::Physics::cBVHTree::ComputePairsHelper(sBVHNode* i_n0, sBVHNode* i_n1)
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

	if (i_n0->IsLeaf())
	{
		// 2 leaves, check proxies instead of fat AABBs
		if (i_n1->IsLeaf())
		{
			if (Physics::IsOverlaps(i_n0->data, i_n1->data))
			{
				m_pairs.push_back(std::pair<cCollider*, cCollider*>(i_n0->data, i_n1->data));
			}
		}
		// 1 branch / 1 leaf, 2 cross checks
		else
		{
			CrossChildren(i_n1);
			ComputePairsHelper(i_n0, i_n1->children[0]);
			ComputePairsHelper(i_n0, i_n1->children[1]);
		}
	}
	else
	{
		// 1 branch / 1 leaf, 2 cross checks
		if (i_n1->IsLeaf())
		{
			CrossChildren(i_n0);
			ComputePairsHelper(i_n0->children[0], i_n1);
			ComputePairsHelper(i_n0->children[1], i_n1);
		}
		// 2 branches, 4 cross checks
		else
		{
			CrossChildren(i_n0);
			CrossChildren(i_n1);
			ComputePairsHelper(i_n0->children[0], i_n1->children[0]);
			ComputePairsHelper(i_n0->children[0], i_n1->children[1]);
			ComputePairsHelper(i_n0->children[1], i_n1->children[0]);
			ComputePairsHelper(i_n0->children[1], i_n1->children[1]);
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
