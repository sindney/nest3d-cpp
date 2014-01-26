#ifndef N3D_OCNODE_H
#define N3D_OCNODE_H

#include <vector>

#include "AABB.h"
#include "MeshNode.h"

namespace nest
{
	class OcNode
	{
	public:

		AABB bound;

		std::vector<MeshNode*> objects;

		std::vector<OcNode*> childs;

		OcNode *parent;

		int id, depth;

		OcNode(OcNode *parent, int id, int depth);

		/**
		 *	If mesh is not attached to any container, then it'll be deleted during OcNode's delocation.
		 */
		~OcNode();
	};
}

#endif