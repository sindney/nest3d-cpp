#include <algorithm>
#include <iterator>
#include <typeinfo>

#include "Mesh.h"
#include "MeshNode.h"
#include "MeshRender.h"
#include "OcNode.h"
#include "OcTree.h"
#include "OcTreeProcessor.h"

namespace nest
{	
	void OcTreeProcessor::calculate(CameraNode *camera, bool flag, std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2) 
	{
		using namespace std;
		
		bool mark0 = result0 != NULL;
		bool mark1 = result1 != NULL;
		bool mark2 = result2 != NULL;

		vector<MeshNode*>::iterator i;
		MeshNode *meshNode;
		AABB bound;

		vector<OcNode*> nodes;
		vector<OcNode*>::iterator j;
		OcNode *node0 = tree->root;
		OcNode *node1 = NULL;

		vector<bool> marks;
		bool current;
		bool parent = false;

		while(true)
		{
			bound = camera->invertWorldMatrix * node0->bound;
			if(parent || camera->culling->classifyAABB(bound))
			{
				current = parent ? true : camera->culling->classifyPoint(bound.min) && camera->culling->classifyPoint(bound.max);
				if(node0->objects.size() != 0)
				{
					for(i = node0->objects.begin(); i != node0->objects.end(); i++)
					{
						meshNode = *i;
						if(meshNode->visible)
						{
							if(!meshNode->cliping || current || camera->culling->classifyAABB(camera->invertWorldMatrix * meshNode->bound))
							{
								if(!meshNode->alphaTest)
								{
									if(flag) render->draw(meshNode, &camera->invertWorldMatrix, &camera->projectionMatrix);
									if(mark0) result0->push_back(meshNode);
								}
								else if(mark1)
								{
									meshNode->alphaKey = meshNode->worldMatrix.raw[12] * meshNode->worldMatrix.raw[12] + 
													meshNode->worldMatrix.raw[13] * meshNode->worldMatrix.raw[13] + 
													meshNode->worldMatrix.raw[14] * meshNode->worldMatrix.raw[14];
									result1->push_back(meshNode);
								}
							}
							else if(mark2)
							{
								result2->push_back(meshNode);
							}
						}
					}
				}
				for(j = node0->childs.begin(); j != node0->childs.end(); j++)
				{
					node1 = *j;
					if(node1 != NULL && (current || camera->culling->classifyAABB(camera->invertWorldMatrix * node1->bound)))
					{
						nodes.push_back(node1);
						marks.push_back(current);
					}
				}
			}
			if(nodes.size() != 0)
			{
				node0 = nodes.back();
				nodes.pop_back();
				parent = marks.back();
				marks.pop_back();
				continue;
			}
			break;
		}
	}

}