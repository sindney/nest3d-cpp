#ifndef N3D_OCTREE_H
#define N3D_OCTREE_H

#include <vector>

#include "Mesh.h"

namespace nest
{
	class OcNode;

	class OcTree
	{
	protected:

		bool findNode(Vector4 *objMax, Vector4 *objMin, Vector4 *nodeMax, Vector4 *nodeMin, int *id, Vector4 *max, Vector4 *min);

	public:

		OcNode *root;

		int depth;

		OcTree(float size, int depth);

		~OcTree();

		void addChild(Mesh *object);

		void removeChild(Mesh *object);

		void transformChild(Mesh *object);
	};
}

#endif