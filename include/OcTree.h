#ifndef N3D_OCTREE_H
#define N3D_OCTREE_H

#include <vector>

#include "Mesh.h"

namespace nest
{
	class OcNode;

	class OcTree
	{
	public:

		OcNode *root;

		/**
		 *	@param size The size of your root tree.
		 *	@param depth The max depth this tree can grow, should be bigger than 0.
		 */
		OcTree(float size, int depth);

		/**
		 *	Note that OcTree's delocation won't delete Meshes attached to OcNodes.
		 */
		~OcTree();

		/**
		 *	Return the max depth value.
		 */
		int getDepth();

		/**
		 *	Make sure mesh belongs to no tree.
		 */
		void addChild(Mesh *object);

		/**
		 *	Make sure mesh belongs to this tree.
		 */
		void removeChild(Mesh *object);

		/**
		 *	Call this when your mesh's transform changes. Make sure mesh belongs to this tree.
		 */
		void transformChild(Mesh *object);

	private:

		int depth;

		/**
		 *	Find the right location for mesh to settle in specific node.
		 *	
		 *	@param objMax Mesh's bound.max in world space.
		 *	@param objMin Mesh's bound.min in world space.
		 *	@param nodeMax Node's bound.max in world space.
		 *	@param nodeMin Node's bound.min in world space.
		 *	@param id Identifies which child node our mesh should settle.
		 *	@param max Child node's bound.max.
		 *	@param min child node's bound.min.
		 *	
		 *	@return True: mesh belongs to current node. False: mesh belongs to current node's child node. Aka node.childs[id].
		 */
		bool findNode(Vector4 *objMax, Vector4 *objMin, Vector4 *nodeMax, Vector4 *nodeMin, int *id, Vector4 *max, Vector4 *min);
	};
}

#endif