#include <iterator>
#include <stdexcept>
#include <typeinfo>

#include "Container3d.h"

namespace nest
{
	using namespace std;

	Container3d::~Container3d()
	{
		vector<Container3d*> containers;
		Container3d *current = this;
		Object3d *object0 = NULL;
		while(true)
		{
			while(current->objects.size() != 0)
			{
				object0 = current->objects.back();
				current->objects.pop_back();
				if(typeid(*object0) == typeid(Container3d))
					containers.push_back(static_cast<Container3d*>(object0));
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

	void Container3d::addChild(Object3d *object)
	{
		objects.push_back(object);
		object->parent = this;
		object->recompose();
	}

	void Container3d::removeChild(Object3d *object)
	{
		bool flag = false;

		vector<Container3d*> containers;
		Container3d *current = this;
		while(true)
		{
			vector<Object3d*>::iterator i;
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
				else if(typeid(**i) == typeid(Container3d))
				{
					containers.push_back(static_cast<Container3d*>(*i));
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

	void Container3d::recompose()
	{
		Object3d::recompose();
		vector<Object3d*>::iterator i;
		for(i = objects.begin(); i != objects.end(); i++)
		{
			(*i)->recompose();
		}
	}

	void Container3d::dispatch(const string *event, bool depthFirst)
	{
		vector<Object3d*>::iterator i;
		Object3d *object0 = NULL;
		if(depthFirst)
		{
			for(i = objects.begin(); i != objects.end(); i++)
			{
				object0 = *i;
				object0->dispatch(event);
				if(typeid(**i) == typeid(Container3d))
					static_cast<Container3d*>(object0)->dispatch(event, depthFirst);
			}
		}
		else 
		{
			vector<Container3d*> containers;
			Container3d *current = this;
			while(true)
			{
				for(i = current->objects.begin(); i != current->objects.end(); i++)
				{
					object0 = *i;
					object0->dispatch(event);
					if(typeid(**i) == typeid(Container3d))
						containers.push_back(static_cast<Container3d*>(object0));
				}
				if(containers.size() != 0)
				{
					current = containers.back();
					containers.pop_back();
					continue;
				}
				break;
			}
		}
	}
}