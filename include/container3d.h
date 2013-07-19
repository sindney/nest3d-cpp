#ifndef CONTAINER3D_H
#define CONTAINER3D_H

#include <iterator>
#include <vector>

#include "object3d.h"

namespace nest
{
	using namespace std;

	class container3d : public object3d
	{
	public:

		vector<object3d*> objects;

		bool castShadows;

		bool visible;

		container3d() : castShadows(false), visible(true) {}

		~container3d()
		{
			objects.clear();
		}

		void addChild(object3d *node)
		{
			objects.push_back(static_cast<object3d*>(node));
			node->parent = this;
			node->recompose();
		}

		object3d *removeChild(object3d *node)
		{
			vector<object3d*>::iterator i;
			for(i = objects.begin(); i != objects.end(); i++)
			{
				if(*i == node) return removeChildAt(i);
			}
			return NULL;
		}

		object3d *removeChildAt(vector<object3d*>::iterator index)
		{
			object3d *node = *index;
			node->parent = NULL;
			node->recompose();
			objects.erase(index);
			return node;
		}

		void recompose()
		{
			object3d::recompose();
			vector<object3d*>::iterator i;
			for(i = objects.begin(); i != objects.end(); i++)
			{
				(*i)->recompose();
			}
		}
	};
}

#endif