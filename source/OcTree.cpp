#include <stdexcept>
#include <typeinfo>
#include <vector>

#include "Geomath.h"
#include "OcTree.h"
#include "OcNode.h"

namespace nest
{
	using namespace std;
	
	OcTree::OcTree(float size, int depth)
	{
		this->depth = depth;
		Vector4 halfsize = Vector4(size / 2, size / 2, size / 2, 1.0f);
		root = new OcNode(NULL, 0, 0);
		root->bound.max = halfsize;
		root->bound.min = -halfsize;
	}

	OcTree::~OcTree()
	{
		if(root != NULL) delete root;
	}

	int OcTree::getDepth()
	{
		return depth;
	}

	void OcTree::addChild(MeshNode *object)
	{
		if(object->node == NULL && object->tree == NULL)
		{
			// we start the search process from root node.
			OcNode *current = root;
			// check if mesh is collided or with-in root node.
			if(Geomath::AABBAABB(object->bound.max, object->bound.min, current->bound.max, current->bound.min))
			{
				int id;
				Vector4 max, min;
				OcNode *node0;
				// loop through the tree to find the right location for mesh.
				while(true)
				{
					// check current node's child to see where to store our mesh.
					if(findNode(&object->bound.max, &object->bound.min, &current->bound.max, &current->bound.min, &id, &max, &min))
					{
						// store our mesh in current node.
						object->node = current;
						object->tree = this;
						current->objects.push_back(object);
						// then break the loop.
						break;
					}

					// store our mesh in current node's child[id] node.
					node0 = current->childs[id];
					if(node0 == NULL) 
					{
						// this node0 is not allocated, so we new it here.
						node0 = new OcNode(current, id, current->depth + 1);
						node0->bound.max = max;
						node0->bound.min = min;
						// and store it to current.childs[id]
						current->childs[id] = node0;

						if(node0->parent->objects.size() == 1 && 
							!findNode(&node0->parent->objects[0]->bound.max, &node0->parent->objects[0]->bound.min, &current->bound.max, &current->bound.min, &id, &max, &min))
						{
							// if you passed the if expression above.
							// it means there's one mesh in current node already and it's necessary to divide current node for that mesh.
							// then we find the node to store that old mesh.
							OcNode *node1 = current->childs[id];
							if(node1 == NULL)
							{
								// new it if it's null.
								node1 = new OcNode(current, id, current->depth + 1);
								node1->bound.max = max;
								node1->bound.min = min;
								current->childs[id] = node1;
							}
							// then we push that old mesh to node1.
							MeshNode *mesh0 = static_cast<MeshNode*>(node0->parent->objects.back());
							node0->parent->objects.pop_back();
							mesh0->node = node1;
							node1->objects.push_back(mesh0);
						}

						// finally we push our mesh to node0, aka current node's child[id].
						object->node = node0;
						object->tree = this;
						node0->objects.push_back(object);
						// we're done here so break the loop.
						break;
					}
					// here means node0 i allocated already.
					// so we check if we'v reached depth limition.
					if(node0->depth + 1 < depth) 
					{
						// if not, value current to node0 and start the loop all over again.
						current = node0;
						continue;
					}
					// if so, we just push our mesh to node0 and we're done.
					object->node = node0;
					object->tree = this;
					node0->objects.push_back(object);
					break;
				}
			}
			else
				throw runtime_error("Error adding child: Target's transform out of range.");
		}
		else 
			throw runtime_error("Error adding child: Target has a tree pointer, remove it from that tree first.");
	}

	void OcTree::removeChild(MeshNode *object)
	{
		if(object->node != NULL && object->tree != NULL)
		{
			bool flag = false;
			vector<MeshNode*>::iterator i;
			// loop through node's objects vector to find target mesh.
			for(i = object->node->objects.begin(); i != object->node->objects.end(); ++i)
			{
				if(*i == object) 
				{
					// if we found it, we earse it from node.
					object->node->objects.erase(i);
					if(object->node->objects.size() == 0)
					{
						// there's no mesh in node after delete process.
						// then we remove the node from memory.
						if(object->node->parent)
						{
							object->node->parent->childs[object->node->id] = NULL;
							delete object->node;
						}
					}
					object->node = NULL;
					object->tree = NULL;
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

	void OcTree::transformChild(MeshNode *object)
	{
		if(object->node != NULL && object->tree != NULL)
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
}