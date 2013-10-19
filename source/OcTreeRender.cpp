#include <algorithm>
#include <iterator>
#include <typeinfo>

#include "OcNode.h"
#include "OcTreeRender.h"

namespace nest
{
	using namespace std;

	void OcTreeRender::calculate(int id, vector<Mesh*> *passed0, vector<Mesh*> *passed1, vector<Mesh*> *rejected) 
	{
		bool recordPassed0 = passed0 != NULL;
		bool recordPassed1 = passed1 != NULL;
		bool recordRejected = rejected != NULL;

		numMeshes = numTris = numVts = 0;

		vector<Mesh*>::iterator i;
		Mesh *mesh;
		ORDraw *draw;

		vector<OcNode*> nodes;
		vector<OcNode*>::iterator j;
		OcNode *node0 = tree->root;
		OcNode *node1 = NULL;

		vector<bool> marks;
		bool current;
		bool parent = false;

		while(true)
		{
			if(parent || camera->culling->classifyAABB(camera->invertWorldMatrix * node0->bound))
			{
				current = parent ? true : camera->culling->classifyPoint(node0->bound.min) && 
						camera->culling->classifyPoint(Vector4(node0->bound.max.x, node0->bound.min.y, node0->bound.min.z, 1.0f)) && 
						camera->culling->classifyPoint(Vector4(node0->bound.min.x, node0->bound.max.y, node0->bound.min.z, 1.0f)) && 
						camera->culling->classifyPoint(Vector4(node0->bound.min.x, node0->bound.min.y, node0->bound.max.z, 1.0f)) && 
						camera->culling->classifyPoint(Vector4(node0->bound.min.x, node0->bound.max.y, node0->bound.max.z, 1.0f)) && 
						camera->culling->classifyPoint(Vector4(node0->bound.max.x, node0->bound.min.y, node0->bound.max.z, 1.0f)) && 
						camera->culling->classifyPoint(Vector4(node0->bound.max.x, node0->bound.max.y, node0->bound.min.z, 1.0f)) && 
						camera->culling->classifyPoint(node0->bound.max);
				if(node0->objects.size() != 0)
				{
					for(i = node0->objects.begin(); i != node0->objects.end(); i++)
					{
						mesh = *i;
						if(mesh->visible)
						{
							if(current || camera->culling->classifyAABB(camera->invertWorldMatrix * mesh->bound))
							{
								if(!mesh->alphaTest)
								{
									draw = mesh->draw ? mesh->draw : this->draw;
									draw->calculate(mesh, &camera->invertWorldMatrix, &camera->projectionMatrix, id);
									if(recordPassed0) passed0->push_back(mesh);
									numMeshes++;
									numTris += mesh->numTris();
									numVts += mesh->numVts();
								}
								else if(recordPassed1) 
								{
									mesh->alphaKey = mesh->worldMatrix.raw[12] * mesh->worldMatrix.raw[12] + 
														mesh->worldMatrix.raw[13] * mesh->worldMatrix.raw[13] + 
														mesh->worldMatrix.raw[14] * mesh->worldMatrix.raw[14];
									passed1->push_back(mesh);
								}
							}
							else if(recordRejected)
							{
								rejected->push_back(mesh);
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