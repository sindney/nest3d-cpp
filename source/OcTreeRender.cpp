#include <algorithm>
#include <iterator>
#include <typeinfo>

#include "MeshRender.h"
#include "OcNode.h"
#include "OcTree.h"
#include "OcTreeRender.h"

namespace nest
{
	using namespace std;
	
	void OcTreeRender::draw(Camera3d *camera, int id, std::vector<Mesh*> *result0, std::vector<Mesh*> *result1, std::vector<Mesh*> *result2) 
	{
		bool mark0 = result0 != NULL;
		bool mark1 = result1 != NULL;
		bool mark2 = result2 != NULL;

		numMeshes = numTris = numVts = 0;

		vector<Mesh*>::iterator i;
		Mesh *mesh;
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
						mesh = *i;
						if(mesh->visible)
						{
							if(!mesh->cliping || camera->culling->classifyAABB(camera->invertWorldMatrix * mesh->bound))
							{
								if(!mesh->alphaTest)
								{
									mesh->render->draw(id, mesh, &camera->invertWorldMatrix, &camera->projectionMatrix);
									numMeshes++;
									numTris += mesh->numTris();
									numVts += mesh->numVts();
									if(mark0) result0->push_back(mesh);
								}
								else if(mark1)
								{
									mesh->alphaKey = mesh->worldMatrix.raw[12] * mesh->worldMatrix.raw[12] + 
													mesh->worldMatrix.raw[13] * mesh->worldMatrix.raw[13] + 
													mesh->worldMatrix.raw[14] * mesh->worldMatrix.raw[14];
									result1->push_back(mesh);
								}
							}
							else if(mark2)
							{
								result2->push_back(mesh);
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