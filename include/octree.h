#ifndef N3D_OCTREE_H
#define N3D_OCTREE_H

#include <vector>

#include "mesh.h"
#include "vector4.h"

namespace nest
{
	class ocnode;

	class octree
	{
	protected:

		bool findNode(vector4 *objMax, vector4 *objMin, vector4 *nodeMax, vector4 *nodeMin, unsigned int *id, vector4 *max, vector4 *min);

	public:

		ocnode *root;

		unsigned int depth;

		octree(float size, unsigned int depth);

		~octree();

		void addChild(mesh *object);

		void removeChild(mesh *object);

		void transformChild(mesh *object);
	};
}

#endif