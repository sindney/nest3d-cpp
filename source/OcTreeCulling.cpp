#include <algorithm>
#include <iterator>

#include "CameraNode.h"
#include "Mesh.h"
#include "MeshNode.h"
#include "OcNode.h"
#include "OcTree.h"
#include "OcTreeCulling.h"

namespace nest
{
	using namespace std;

	void OcTreeCulling::classify(
		OcTree *tree, CameraNode *camera, 
		vector<MeshNode*> *result0, vector<MeshNode*> *result1, vector<MeshNode*> *result2
	)
	{
		// meshes pased culling test with alphaSort off
		bool mark0 = result0 != NULL;
		// meshes pased culling test with alphaSort on
		bool mark1 = result1 != NULL;
		// meshes didn't pased culling test
		bool mark2 = result2 != NULL;

		vector<MeshNode*>::iterator it0;
		MeshNode *meshNode;
		AABB bound;
		Matrix4 mat0;

		vector<OcNode*> nodes;
		vector<OcNode*>::iterator it1;
		OcNode *node0 = tree->root;
		OcNode *node1 = NULL;

		vector<bool> marks;
		bool current;
		bool parent = false;

		while(true)
		{
			bound = camera->invertWorldMatrix * node0->bound;
			if(parent || camera->culling.classifyAABB(bound))
			{
				current = parent ? true : camera->culling.classifyPoint(bound.min) && camera->culling.classifyPoint(bound.max);
				if(node0->objects.size() != 0)
				{
					for(it0 = node0->objects.begin(); it0 != node0->objects.end(); ++it0)
					{
						meshNode = *it0;
						if(meshNode->visible)
						{
							if(!meshNode->cliping || current || camera->culling.classifyAABB(camera->invertWorldMatrix * meshNode->bound))
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
				for(it1 = node0->childs.begin(); it1 != node0->childs.end(); ++it1)
				{
					node1 = *it1;
					if(node1 != NULL && (current || camera->culling.classifyAABB(camera->invertWorldMatrix * node1->bound)))
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
		// sort meshes with alphaSort on
		if(mark1) sort(result1->begin(), result1->end(), MeshNode(NULL));
	}

}