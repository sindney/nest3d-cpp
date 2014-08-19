#include <algorithm>
#include <iterator>
#include <typeinfo>

#include "CameraNode.h"
#include "ContainerCulling.h"
#include "ContainerNode.h"
#include "Mesh.h"
#include "MeshNode.h"

namespace nest
{
	using namespace std;

	void ContainerCulling::classify(
		ContainerNode *root, CameraNode *camera, 
		vector<MeshNode*> *result0, vector<MeshNode*> *result1, vector<MeshNode*> *result2
	)
	{
		// meshes pased culling test with alphaSort off
		bool mark0 = result0 != NULL;
		// meshes pased culling test with alphaSort on
		bool mark1 = result1 != NULL;
		// meshes didn't pased culling test
		bool mark2 = result2 != NULL;

		vector<ContainerNode*> containers;

		vector<ObjectNode*>::iterator it0;
		ContainerNode *current = root;

		MeshNode *meshNode;
		Matrix4 mat0;

		while(true)
		{
			if(current->visible)
			{
				for(it0 = current->objects.begin(); it0 != current->objects.end(); ++it0)
				{
					if(typeid(**it0) == typeid(ContainerNode))
					{
						containers.push_back(static_cast<ContainerNode*>(*it0));
					} 
					else 
					{
						meshNode = static_cast<MeshNode*>(*it0);
						if(meshNode->visible)
						{
							if(!meshNode->cliping || camera->culling.classifyAABB(camera->invertWorldMatrix * meshNode->bound))
							{
								if(!meshNode->alphaSort && mark0) result0->push_back(meshNode);
								if(meshNode->alphaSort && mark1)
								{
									mat0 = camera->invertWorldMatrix * meshNode->worldMatrix;
									meshNode->alphaKey = mat0.raw[12] * mat0.raw[12] + mat0.raw[13] * mat0.raw[13] + mat0.raw[14] * mat0.raw[14];
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
		// sort meshes with alphaSort on
		if(mark1) sort(result1->begin(), result1->end(), MeshNode(NULL));
	}
}