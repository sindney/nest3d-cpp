#include <iterator>
#include <stdexcept>
#include <typeinfo>
#include <vector>

#include "container3d.h"

namespace nest
{
	container3d::~container3d()
	{
		vector<container3d*> containers;
		container3d *current = this;
		object3d *object0 = NULL;
		while(true)
		{
			while(current->objects.size() != 0)
			{
				object0 = current->objects.back();
				current->objects.pop_back();
				if(typeid(*object0) == typeid(container3d))
					containers.push_back(static_cast<container3d*>(object0));
				else 
					delete object0;
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

	void container3d::addChild(object3d *object)
	{
		objects.push_back(object);
		object->parent = this;
		object->recompose();
	}

	void container3d::removeChild(object3d *object)
	{
		bool flag = false;

		vector<container3d*> containers;
		container3d *current = this;
		while(true)
		{
			vector<object3d*>::iterator i;
			for(i = current->objects.begin(); i != current->objects.end(); i++)
			{
				if(*i == object)
				{
					object->parent = NULL;
					object->recompose();
					current->objects.erase(i);
					flag = true;
					break;
				}
				else if(typeid(**i) == typeid(container3d))
				{
					containers.push_back(static_cast<container3d*>(*i));
				}
			}
			if(!flag && containers.size() != 0)
			{
				current = containers.back();
				containers.pop_back();
				continue;
			}
			break;
		}

		if(!flag) throw runtime_error("Error removing child: Can't locate child pointer in target container.");
	}

	void container3d::recompose()
	{
		object3d::recompose();
		vector<object3d*>::iterator i;
		for(i = objects.begin(); i != objects.end(); i++)
		{
			(*i)->recompose();
		}
	}
}