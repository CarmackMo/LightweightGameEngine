#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>

#include <list>
#include <vector>


#define DEFAULT_BVH_MARGIN 0.0f


// BVH Tree Node
//=============

namespace eae6320
{
namespace Physics
{

	struct sBVHNode
	{

		// Data
		//=========================

		sBVHNode* parent;
		sBVHNode* children[2];

		// fatAABB whose min/max extent contains the world extents of all children of this node
		// min/max extent uses world coordinate directly. World position should be ZERO
		cAABBCollider fatAABB;

		// link to the actual gameobject's collider
		cCollider* collider;

		// TODO: need to optmize this
		bool childrenCrossed;


		// Interface
		//=========================

		sBVHNode();

		bool IsLeaf() const;

		/* Make this ndoe a branch node */
		void SetBranch(sBVHNode* i_node0, sBVHNode* i_node1);

		/* Make this node a leaf */
		void SetLeaf(cCollider* i_collider);

		void UpdateAABB(float i_margin);

		sBVHNode* GetSibling() const;
	};

}// Namespace Physics
}// Namespace eae6320


// BVH Tree Class Declaration
//=============

namespace eae6320
{
namespace Physics
{

	class cBVHTree
	{
		// Interface
		//=========================

	public:
		cBVHTree() : m_root(nullptr), m_margin(DEFAULT_BVH_MARGIN) {}
		cBVHTree(float i_margin) : m_root(nullptr), m_margin(i_margin) { }

		virtual sBVHNode* Search(cCollider* i_collider);
		virtual void Add(cCollider* i_collider);
		virtual void Remove(cCollider* i_collider);
		virtual void Update();

		virtual std::list<std::pair<cCollider*, cCollider*>>& ComputePairs();
		virtual std::vector<cCollider*> Query(cCollider* i_collider) const;
		// TODO
		//virtual cCollider* Pick(const Math::sVector& i_point) const;
		//virtual RayCastResult RayCast(const Ray3& ray) const;



		// Implementation
		//=========================

	private:

		void InsertNode(sBVHNode* i_node, sBVHNode** i_parent);
		void RemoveNode(sBVHNode* i_node);

		void UpdateNodeHelper(sBVHNode* i_node, std::vector<sBVHNode*>& i_invalidNodes);
		void ComputePairsHelper(sBVHNode* i_node0, sBVHNode* i_node1);
		void ClearChildrenCrossFlagHelper(sBVHNode* i_node);
		void CrossChildren(sBVHNode* i_node);


		// Data
		//=========================

	private:

		float m_margin;
		sBVHNode* m_root;
		std::list<std::pair<cCollider*, cCollider*>> m_pairs;
		std::vector<sBVHNode*> m_invalidNodes;

	};

}// Namespace Physics
}// Namespace eae6320