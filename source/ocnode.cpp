#include "mesh.h"
#include "ocnode.h"
#include "octree.h"

namespace nest
{
	ocnode::ocnode(octree *belonging, ocnode *parent, unsigned int id, unsigned int depth)
	{
		this->belonging = belonging;
		this->parent = parent;
		this->id = id;
		this->depth = depth;
		int i = 0;
		while(i < 8)
		{
			childs.push_back(NULL);
			i++;
		}
	}

	ocnode::~ocnode()
	{
		ocnode *child;
		while(childs.size() != 0)
		{
			child = childs.back();
			childs.pop_back();
			if(child != NULL) delete child;
		}
		objects.clear();
	}
}