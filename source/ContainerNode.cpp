#include <iterator>
#include <stdexcept>
#include <typeinfo>

#include "ContainerNode.h"

namespace nest
{
	using namespace std;

	ContainerNode::~ContainerNode()
	{
		ObjectNode *object0 = NULL;
		while(objects.size() != 0)
		{
			object0 = objects.back();
			objects.pop_back();
			delete object0;
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
			for(i = current->objects.begin(); i != current->objects.end(); ++i)
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

		if(!flag) throw runtime_error(
			"ContainerNode::removeChild()::Error removing child, can't locate child pointer in target container."
		);
	}

	void ContainerNode::recompose(float dt)
	{
		ObjectNode::recompose(dt);
		vector<ObjectNode*>::iterator i;
		for(i = objects.begin(); i != objects.end(); ++i)
		{
			(*i)->recompose(dt);
		}
	}

	void ContainerNode::dispatch(const Event *event, bool depthFirst)
	{
		vector<ObjectNode*>::iterator i;
		ObjectNode *object0 = NULL;
		if(depthFirst)
		{
			for(i = objects.begin(); i != objects.end(); ++i)
			{
				object0 = *i;
				object0->dispatch(event);
				if(typeid(**i) == typeid(ContainerNode))
					static_cast<ContainerNode*>(object0)->dispatch(event, depthFirst);
			}
		}
		else 
		{
			vector<ContainerNode*> containers;
			ContainerNode *current = this;
			while(true)
			{
				for(i = current->objects.begin(); i != current->objects.end(); ++i)
				{
					object0 = *i;
					object0->dispatch(event);
					if(typeid(**i) == typeid(ContainerNode))
						containers.push_back(static_cast<ContainerNode*>(object0));
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