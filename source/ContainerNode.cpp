#include <iterator>
#include <stdexcept>
#include <typeinfo>

#include "ContainerNode.h"

namespace nest
{
	using namespace std;

	ContainerNode::~ContainerNode()
	{
		vector<ContainerNode*> containers;
		ContainerNode *current = this;
		ObjectNode *object0 = NULL;
		while(true)
		{
			while(current->objects.size() != 0)
			{
				object0 = current->objects.back();
				current->objects.pop_back();
				if(typeid(*object0) == typeid(ContainerNode))
					containers.push_back(static_cast<ContainerNode*>(object0));
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

	void ContainerNode::addChild(ObjectNode *object)
	{
		objects.push_back(object);
		object->parent = this;
		object->recompose();
	}

	void ContainerNode::removeChild(ObjectNode *object)
	{
		bool flag = false;

		vector<ContainerNode*> containers;
		ContainerNode *current = this;
		while(true)
		{
			vector<ObjectNode*>::iterator i;
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
				else if(typeid(**i) == typeid(ContainerNode))
				{
					containers.push_back(static_cast<ContainerNode*>(*i));
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

	void ContainerNode::recompose()
	{
		ObjectNode::recompose();
		vector<ObjectNode*>::iterator i;
		for(i = objects.begin(); i != objects.end(); i++)
		{
			(*i)->recompose();
		}
	}
}