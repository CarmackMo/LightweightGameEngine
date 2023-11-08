#pragma once

// Includes
//=========

#include <Engine/Math/sVector.h>
#include <Engine/Physics/cAABBCollider.h>
#include <Engine/Physics/cColliderBase.h>

#include <list>
#include <vector>




typedef std::list<std::pair<eae6320::Physics::cCollider*, eae6320::Physics::cCollider*>> ColliderPairList;



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

		sBVHNode() :
			parent(nullptr), data(nullptr), childrenCrossed(false)
		{
			children[0] = nullptr;
			children[1] = nullptr;
		}

		bool IsLeaf() const
		{
			return children[0] == nullptr;
		}

		/* Make this ndoe a branch node */ 
		void SetBranch(sBVHNode* i_node0, sBVHNode* i_node1)
		{
			i_node0->parent = this;
			i_node1->parent = this;

			children[0] = i_node0;
			children[1] = i_node1;
		}

		/* Make this node a leaf */ 
		void SetLeaf(cAABBCollider* data)
		{
			// create two-way link
			this->data = data;
			//data->userData = this;

			children[0] = nullptr;
			children[1] = nullptr;
		}

		void UpdateAABB(float margin)
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



			//if (IsLeaf())
			//{
			//	// make fat AABB
			//	const Vec3 marginVec(margin, margin, margin);
			//	aabb.minPoint = data->minPoint - marginVec;
			//	aabb.maxPoint = data->maxPoint + marginVec;
			//}
			//else
			//	// make union of child AABBs of child nodes
			//	aabb =
			//	children[0]->aabb.Union(children[1]->aabb);
		}


	};



	class cBVHTree
	{
		// Interface
		//=========================

	public:

		cBVHTree() :
			m_root(nullptr), m_margin(0.2f)
		{ }

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

		void UpdateNodeHelper(sBVHNode* i_node, std::vector<sBVHNode*>& i_invalidNodes);
		void InsertNode(sBVHNode* i_node, sBVHNode** i_parent);
		void RemoveNode(sBVHNode* i_node);

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
