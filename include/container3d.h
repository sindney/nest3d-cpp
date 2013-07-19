#ifndef CONTAINER3D_H
#define CONTAINER3D_H

#include <iterator>
#include <typeinfo>
#include <vector>

#include "mesh.h"
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

		/**
		 *	This delocate function will delete all child objects.
		 *	You can call container.objects.clear() before delete to avoid that process from happening.
		 */
		~container3d()
		{
			vector<container3d*> containers;
			container3d *current = this;
			object3d *object0 = NULL;
			mesh *mesh0 = NULL;
			while(true)
			{
				while(current->objects.size() != 0)
				{
					object0 = current->objects.back();
					current->objects.pop_back();
					if(typeid(*object0) == typeid(container3d))
					{
						containers.push_back(static_cast<container3d*>(object0));
					} 
					else if(typeid(*object0) == typeid(mesh))
					{
						mesh0 = static_cast<mesh*>(object0);
						delete mesh0;
					}
				}
				if(current != this) delete current;
				if(containers.size() != 0)
				{
					current = containers.back();
					containers.pop_back();
					continue;
				}
				break;
			}
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