#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>

#include <list>
#include <vector>




typedef std::list<std::pair<eae6320::Physics::cCollider*, eae6320::Physics::cCollider*>> ColliderPairList;


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

		// these will be explained later
		bool childrenCrossed;
		cAABBCollider aabb;
		cAABBCollider* data;


		// Interface
		//=========================

		sBVHNode();

		bool IsLeaf() const;

		/* Make this ndoe a branch node */
		void SetBranch(sBVHNode* i_node0, sBVHNode* i_node1);

		/* Make this node a leaf */
		void SetLeaf(cAABBCollider* i_data);

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

		cBVHTree() :
			m_root(nullptr), m_margin(0.2f)
		{ }

		virtual sBVHNode* Search(cAABBCollider* i_AABB);
		virtual void Add(cAABBCollider* i_AABB);
		virtual void Remove(cAABBCollider* i_AABB);
		virtual void Update();

		virtual ColliderPairList& ComputePairs();
		virtual cCollider* Pick(const Math::sVector& i_point) const;
		virtual void Query(const cAABBCollider& i_AABB, std::vector<cCollider*>& out) const;
		// TODO
		//virtual RayCastResult RayCast(const Ray3& ray) const;



		// Implementation
		//=========================

	private:

		void InsertNode(sBVHNode* i_node, sBVHNode** i_parent);
		void RemoveNode(sBVHNode* i_node);

		void UpdateNodeHelper(sBVHNode* i_node, std::vector<sBVHNode*>& i_invalidNodes);
		void ComputePairsHelper(sBVHNode* i_n0, sBVHNode* i_n1);
		void ClearChildrenCrossFlagHelper(sBVHNode* i_node);
		void CrossChildren(sBVHNode* i_node);


		// Data
		//=========================

	private:

		float m_margin;
		sBVHNode* m_root;
		ColliderPairList m_pairs;
		std::vector<sBVHNode*> m_invalidNodes;

	};

}// Namespace Physics
}// Namespace eae6320
