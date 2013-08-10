#ifndef N3D_OCNODE_H
#define N3D_OCNODE_H

#include <vector>

#include "aabb.h"
#include "vector4.h"

namespace nest
{
	using namespace std;

	class mesh;

	class octree;

	class ocnode
	{
	public:

		aabb bound;

		vector<mesh*> objects;

		vector<ocnode*> childs;

		ocnode *parent;

		octree *belonging;

		unsigned int id, depth;

		ocnode(octree *belonging, ocnode *parent, int id, int depth);

		~ocnode();
	};
}

#endif