#ifndef N3D_OCNODE_H
#define N3D_OCNODE_H

#include <vector>

#include "aabb.h"
#include "vector4.h"

namespace nest
{
	using namespace std;

	class mesh;

	class ocnode
	{
	public:

		aabb bound;

		vector<mesh*> objects;

		vector<ocnode*> childs;

		ocnode *parent;

		unsigned int id, depth;

		ocnode(ocnode *parent, unsigned int id, unsigned int depth);

		~ocnode();
	};
}

#endif