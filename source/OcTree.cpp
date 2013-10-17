#include <stdexcept>
#include <typeinfo>
#include <vector>

#include "Geomath.h"
#include "OcNode.h"
#include "OcTree.h"

namespace nest
{
	bool OcTree::findNode(Vector4 *objMax, Vector4 *objMin, Vector4 *nodeMax, Vector4 *nodeMin, int *id, Vector4 *max, Vector4 *min)
	{
		float size = nodeMax->x - nodeMin->x;
		float halfsize = size / 2;
		// top top left
		Vector4 maxTTL = *nodeMax;
		Vector4 minTTL = *nodeMin + Vector4(halfsize, halfsize, halfsize, 1.0f);
		bool BTTL = Geomath::AABBAABB(*objMax, *objMin, maxTTL, minTTL);
		// top top right
		Vector4 maxTTR = *nodeMax - Vector4(halfsize, 0.0f, 0.0f, 1.0f);
		Vector4 minTTR = *nodeMin + Vector4(0.0f, halfsize, halfsize, 1.0f);
		bool BTTR = Geomath::AABBAABB(*objMax, *objMin, maxTTR, minTTR);
		// top bottom left
		Vector4 maxTBL = *nodeMax - Vector4(0.0f, 0.0f, halfsize, 1.0f);
		Vector4 minTBL = *nodeMin + Vector4(halfsize, halfsize, 0.0f, 1.0f);
		bool BTBL = Geomath::AABBAABB(*objMax, *objMin, maxTBL, minTBL);
		// top bottom right
		Vector4 maxTBR = *nodeMax - Vector4(halfsize, 0.0f, halfsize, 1.0f);
		Vector4 minTBR = *nodeMin + Vector4(0.0f, halfsize, 0.0f, 1.0f);
		bool BTBR = Geomath::AABBAABB(*objMax, *objMin, maxTBR, minTBR);
		// bottom top left
		Vector4 maxBTL = *nodeMax - Vector4(0.0f, halfsize, 0.0f, 1.0f);
		Vector4 minBTL = *nodeMin + Vector4(halfsize, 0.0f, halfsize, 1.0f);
		bool BBTL = Geomath::AABBAABB(*objMax, *objMin, maxBTL, minBTL);
		// bottom top right
		Vector4 maxBTR = *nodeMax - Vector4(halfsize, halfsize, 0.0f, 1.0f);
		Vector4 minBTR = *nodeMin + Vector4(0.0f, 0.0f, halfsize, 1.0f);
		bool BBTR = Geomath::AABBAABB(*objMax, *objMin, maxBTR, minBTR);
		// bottom bottom left
		Vector4 maxBBL = *nodeMax - Vector4(0.0f, halfsize, halfsize, 1.0f);
		Vector4 minBBL = *nodeMin + Vector4(halfsize, 0.0f, 0.0f, 1.0f);
		bool BBBL = Geomath::AABBAABB(*objMax, *objMin, maxBBL, minBBL);
		// bottom bottom right
		Vector4 maxBBR = *nodeMax - Vector4(halfsize, halfsize, halfsize, 1.0f);
		Vector4 minBBR = *nodeMin;
		bool BBBR = Geomath::AABBAABB(*objMax, *objMin, maxBBR, minBBR);

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

	OcTree::OcTree(float size, int depth)
	{
		this->depth = depth;
		Vector4 halfsize = Vector4(size / 2, size / 2, size / 2, 1.0f);
		root = new OcNode(this, NULL, 0, 0);
		root->bound.max = halfsize;
		root->bound.min = -halfsize;
	}

	OcTree::~OcTree()
	{
		if(root != NULL) delete root;
	}

	void OcTree::addChild(Mesh *object)
	{
		OcNode *current = root;
		if(Geomath::AABBAABB(object->bound.max, object->bound.min, current->bound.max, current->bound.min))
		{
			int id;
			Vector4 max, min;
			OcNode *node0;
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
					node0 = new OcNode(this, current, id, current->depth + 1);
					node0->bound.max = max;
					node0->bound.min = min;
					current->childs[id] = node0;

					if(node0->parent->objects.size() == 1 && 
						!findNode(&node0->parent->objects[0]->bound.max, &node0->parent->objects[0]->bound.min, &current->bound.max, &current->bound.min, &id, &max, &min))
					{
						OcNode *node1 = current->childs[id];
						if(node1 == NULL)
						{
							node1 = new OcNode(this, current, id, current->depth + 1);
							node1->bound.max = max;
							node1->bound.min = min;
							current->childs[id] = node1;
						}
						Mesh *Mesh0 = static_cast<Mesh*>(node0->parent->objects.back());
						node0->parent->objects.pop_back();
						Mesh0->node = node1;
						node1->objects.push_back(Mesh0);
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

	void OcTree::removeChild(Mesh *object)
	{
		if(object->node)
		{
			bool flag = false;
			vector<Mesh*>::iterator i;
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

	void OcTree::transformChild(Mesh *object)
	{
		if(object->node)
		{
			if(object->bound.max.x >= object->node->bound.max.x ||  
				object->bound.max.y >= object->node->bound.max.y ||  
				object->bound.max.z >= object->node->bound.max.z ||  
				object->bound.min.x <= object->node->bound.min.x ||  
				object->bound.min.y <= object->node->bound.min.y ||  
				object->bound.min.z <= object->node->bound.min.z)
			{
				removeChild(object);
				addChild(object);
			}
		}
		else 
		{
			throw runtime_error("Error transforming child: Target has a NULL node pointer.");
		}
	}
}