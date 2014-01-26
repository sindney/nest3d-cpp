#include "OcNode.h"

namespace nest
{
	OcNode::OcNode(OcNode *parent, int id, int depth)
	{
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

	OcNode::~OcNode()
	{
		OcNode *child;
		while(childs.size() != 0)
		{
			child = childs.back();
			childs.pop_back();
			if(child != NULL) delete child;
		}
		MeshNode *object;
		while(objects.size() != 0)
		{
			object = objects.back();
			objects.pop_back();
			object->node = NULL;
			object->tree = NULL;
			if(object->parent == NULL) delete object;
		}
	}
}