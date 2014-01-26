#include <algorithm>
#include <iterator>
#include <typeinfo>

#include "ContainerNode.h"
#include "ContainerProcessor.h"
#include "Mesh.h"
#include "MeshNode.h"
#include "MeshRender.h"

namespace nest
{
	using namespace std;
	
	void ContainerProcessor::calculate(CameraNode *camera, bool flag, std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2)
	{
		bool mark0 = result0 != NULL;
		bool mark1 = result1 != NULL;
		bool mark2 = result2 != NULL;

		vector<ContainerNode*> containers;

		vector<ObjectNode*>::iterator i;
		ContainerNode *current = root;

		MeshNode *meshNode;

		while(true)
		{
			if(current->visible)
			{
				for(i = current->objects.begin(); i != current->objects.end(); i++)
				{
					if(typeid(**i) == typeid(ContainerNode))
					{
						containers.push_back(static_cast<ContainerNode*>(*i));
					} 
					else 
					{
						meshNode = static_cast<MeshNode*>(*i);
						if(meshNode->visible)
						{
							if(!meshNode->cliping || camera->culling->classifyAABB(camera->invertWorldMatrix * meshNode->bound))
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