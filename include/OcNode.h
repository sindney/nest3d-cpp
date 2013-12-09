#ifndef N3D_OCNODE_H
#define N3D_OCNODE_H

#include <vector>

#include "AABB.h"
#include "Mesh.h"

namespace nest
{
	class OcTree;

	class OcNode
	{
	public:

		AABB bound;

		std::vector<Mesh*> objects;

		std::vector<OcNode*> childs;

		OcNode *parent;

		OcTree *belonging;

		unsigned int id, depth;

		OcNode(OcTree *belonging, OcNode *parent, int id, int depth);

		~OcNode();
	};
}

#endif