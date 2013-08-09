#include <stdexcept>
#include <typeinfo>
#include <vector>

#include "collision.h"
#include "container3d.h"
#include "octree.h"

namespace nest
{
	void octree::removeMesh(mesh *object)
	{
		if(object->node)
		{
			bool flag = false;
			vector<mesh*>::iterator i;
			for(i = object->node->objects.begin(); i != object->node->objects.end(); i++)
			{
				if(*i == object) 
				{
					object->node->objects.erase(i);
					if(object->node->objects.size() == 0)
					{
						if(object->node->parent)
						{
							object->node->parent->childs[object->node->id] = NULL;
							delete object->node;
						}
					}
					object->node = NULL;
					flag = true;
					break;
				}
			}
			if(!flag) throw runtime_error("Error removing child: Can't locate child.");
		}
		else 
		{
			throw runtime_error("Error removing child: Target has a NULL node pointer.");
		}
	}

	bool octree::findNode(vector4 *objMax, vector4 *objMin, vector4 *nodeMax, vector4 *nodeMin, unsigned int *id, vector4 *max, vector4 *min)
	{
		float size = nodeMax->x - nodeMin->x;
		float halfsize = size / 2;
		// top top left
		vector4 maxTTL = *nodeMax;
		vector4 minTTL = *nodeMin + vector4(halfsize, halfsize, halfsize, 1.0f);
		bool BTTL = collision::aabb_aabb(*objMax, *objMin, maxTTL, minTTL);
		// top top right
		vector4 maxTTR = *nodeMax - vector4(halfsize, 0.0f, 0.0f, 1.0f);
		vector4 minTTR = *nodeMin + vector4(0.0f, halfsize, halfsize, 1.0f);
		bool BTTR = collision::aabb_aabb(*objMax, *objMin, maxTTR, minTTR);
		// top bottom left
		vector4 maxTBL = *nodeMax - vector4(0.0f, 0.0f, halfsize, 1.0f);
		vector4 minTBL = *nodeMin + vector4(halfsize, halfsize, 0.0f, 1.0f);
		bool BTBL = collision::aabb_aabb(*objMax, *objMin, maxTBL, minTBL);
		// top bottom right
		vector4 maxTBR = *nodeMax - vector4(halfsize, 0.0f, halfsize, 1.0f);
		vector4 minTBR = *nodeMin + vector4(0.0f, halfsize, 0.0f, 1.0f);
		bool BTBR = collision::aabb_aabb(*objMax, *objMin, maxTBR, minTBR);
		// bottom top left
		vector4 maxBTL = *nodeMax - vector4(0.0f, halfsize, 0.0f, 1.0f);
		vector4 minBTL = *nodeMin + vector4(halfsize, 0.0f, halfsize, 1.0f);
		bool BBTL = collision::aabb_aabb(*objMax, *objMin, maxBTL, minBTL);
		// bottom top right
		vector4 maxBTR = *nodeMax - vector4(halfsize, halfsize, 0.0f, 1.0f);
		vector4 minBTR = *nodeMin + vector4(0.0f, 0.0f, halfsize, 1.0f);
		bool BBTR = collision::aabb_aabb(*objMax, *objMin, maxBTR, minBTR);
		// bottom bottom left
		vector4 maxBBL = *nodeMax - vector4(0.0f, halfsize, halfsize, 1.0f);
		vector4 minBBL = *nodeMin + vector4(halfsize, 0.0f, 0.0f, 1.0f);
		bool BBBL = collision::aabb_aabb(*objMax, *objMin, maxBBL, minBBL);
		// bottom bottom right
		vector4 maxBBR = *nodeMax - vector4(halfsize, halfsize, halfsize, 1.0f);
		vector4 minBBR = *nodeMin;
		bool BBBR = collision::aabb_aabb(*objMax, *objMin, maxBBR, minBBR);

		if(BTTL && (BTTR || BTBL || BTBR || BBTL || BBTR || BBBL || BBBR) || 
			BTTR && (BTTL || BTBL || BTBR || BBTL || BBTR || BBBL || BBBR) || 
			BTBL && (BTTL || BTTR || BTBR || BBTL || BBTR || BBBL || BBBR) || 
			BTBR && (BTTL || BTTR || BTBL || BBTL || BBTR || BBBL || BBBR) ||
			BBTL && (BBTR || BBBL || BBBR || BTTL || BTTR || BTBL || BTBR) || 
			BBTR && (BBTL || BBBL || BBBR || BTTL || BTTR || BTBL || BTBR) || 
			BBBL && (BBTL || BBTR || BBBR || BTTL || BTTR || BTBL || BTBR) || 
			BBBR && (BBTL || BBTR || BBBL || BTTL || BTTR || BTBL || BTBR)) 
		{
			return true;
		}
		else if(BTTL)
		{
			*id = 0;
			*max = maxTTL;
			*min = minTTL;
		}
		else if(BTTR)
		{
			*id = 1;
			*max = maxTTR;
			*min = minTTR;
		}
		else if(BTBL)
		{
			*id = 2;
			*max = maxTBL;
			*min = minTBL;
		}
		else if(BTBR)
		{
			*id = 3;
			*max = maxTBR;
			*min = minTBR;
		}
		else if(BBTL)
		{
			*id = 4;
			*max = maxBTL;
			*min = minBTL;
		}
		else if(BBTR)
		{
			*id = 5;
			*max = maxBTR;
			*min = minBTR;
		}
		else if(BBBL)
		{
			*id = 6;
			*max = maxBBL;
			*min = minBBL;
		}
		else 
		{
			*id = 7;
			*max = maxBBR;
			*min = minBBR;
		}
		return false;
	}

	void octree::addMesh(mesh *object)
	{
		ocnode *current = root;
		if(collision::aabb_aabb(object->bound.max, object->bound.min, current->bound.max, current->bound.min))
		{
			unsigned int id;
			vector4 max, min;
			ocnode *node0;
			while(true)
			{
				if(findNode(&object->bound.max, &object->bound.min, &current->bound.max, &current->bound.min, &id, &max, &min))
				{
					object->node = current;
					current->objects.push_back(object);
					break;
				}

				node0 = current->childs[id];
				if(node0 == NULL) 
				{
					node0 = new ocnode(current, id, current->depth + 1);
					node0->bound.max = max;
					node0->bound.min = min;
					current->childs[id] = node0;

					if(node0->parent->objects.size() == 1 && 
						!findNode(&node0->parent->objects[0]->bound.max, &node0->parent->objects[0]->bound.min, &current->bound.max, &current->bound.min, &id, &max, &min))
					{
						ocnode *node1 = current->childs[id];
						if(node1 == NULL)
						{
							node1 = new ocnode(current, id, current->depth + 1);
							node1->bound.max = max;
							node1->bound.min = min;
							current->childs[id] = node1;
						}
						mesh *mesh0 = static_cast<mesh*>(node0->parent->objects.back());
						node0->parent->objects.pop_back();
						mesh0->node = node1;
						node1->objects.push_back(mesh0);
					}

					object->node = node0;
					node0->objects.push_back(object);
					break;
				}
				if(node0->depth + 1 < depth) 
				{
					current = node0;
					continue;
				}
				object->node = node0;
				node0->objects.push_back(object);
				break;
			}
		}
		else
			throw runtime_error("Error adding child: Target's transform out of range.");
	}

	octree::octree(float size, unsigned int depth)
	{
		this->depth = depth;
		vector4 halfsize = vector4(size / 2, size / 2, size / 2, 1.0f);
		root = new ocnode(NULL, 0, 0);
		root->bound.max = halfsize;
		root->bound.min = -halfsize;
	}

	octree::~octree()
	{
		if(root != NULL) delete root;
	}

	void octree::addChild(object3d *object)
	{
		bool is_container = typeid(*object) == typeid(container3d);

		if(typeid(*object) != typeid(mesh) && !is_container) throw runtime_error("Error adding child: Invalid target type.");

		if(is_container)
		{
			vector<object3d*>::iterator i;
			vector<container3d*> containers;
			container3d *current = static_cast<container3d*>(object);
			while(true)
			{
				for(i = current->objects.begin(); i != current->objects.end(); i++)
				{
					if(typeid(**i) == typeid(container3d))
					{
						containers.push_back(static_cast<container3d*>(*i));
					} 
					else if(typeid(**i) == typeid(mesh))
					{
						addMesh(static_cast<mesh*>(*i));
					}
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
		else 
		{
			addMesh(static_cast<mesh*>(object));
		}
	}
	
	void octree::removeChild(object3d *object)
	{
		bool is_container = typeid(*object) == typeid(container3d);

		if(typeid(*object) != typeid(mesh) && !is_container) throw runtime_error("Error removing child: Invalid target type.");

		if(is_container)
		{
			vector<object3d*>::iterator i;
			vector<container3d*> containers;
			container3d *current = static_cast<container3d*>(object);
			while(true)
			{
				for(i = current->objects.begin(); i != current->objects.end(); i++)
				{
					if(typeid(**i) == typeid(container3d))
					{
						containers.push_back(static_cast<container3d*>(*i));
					} 
					else if(typeid(**i) == typeid(mesh))
					{
						removeMesh(static_cast<mesh*>(*i));
					}
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
		else 
		{
			removeMesh(static_cast<mesh*>(object));
		}
	}
}