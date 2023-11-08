// Includes
//=========

#include <Engine/Physics/BVHTree.h>
#include <Engine/Asserts/Asserts.h>

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


void eae6320::Physics::sBVHNode::SetLeaf(cAABBCollider* data)
{
	// create two-way link
	this->data = data;
	//data->userData = this;

	children[0] = nullptr;
	children[1] = nullptr;
}


void eae6320::Physics::sBVHNode::UpdateAABB(float margin)
{
	if (IsLeaf())
	{
		// make fat AABB
		const Math::sVector marginVec(margin, margin, margin);
		aabb.m_min = data->m_min - marginVec;
		aabb.m_max = data->m_max + marginVec;
	}
	else
	{
		// make union of child nodes' AABB
		aabb = children[0]->aabb.Union(children[1]->aabb);
	}
}


eae6320::Physics::sBVHNode* eae6320::Physics::sBVHNode::GetSibling() const
{
	return this == parent->children[0] ?
		parent->children[1] :
		parent->children[0];
}


// cBVHTree Implementation
//==================

void eae6320::Physics::cBVHTree::Update()
{
	if (m_root)
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

	//// Iteration approach, using Breadth First Search
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
