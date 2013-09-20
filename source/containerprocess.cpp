#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <vector>

#include "containerprocess.h"
#include "ocnode.h"
#include "shader3d.h"

namespace nest
{
	containerprocess::~containerprocess() 
	{
		if(container != NULL) delete container;
		if(camera != NULL) delete camera;
		if(partition != NULL) delete partition;
		objects.clear();
		alphaObjects.clear();
	}

	void containerprocess::calculate()
	{
		objects.clear();
		alphaObjects.clear();
		numObjects = 0;
		numTriangles = 0;
		numVertices = 0;

		glBindFramebuffer(GL_FRAMEBUFFER, target->frameBuffer);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glViewport(target->x, target->y, target->width, target->height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		vector<container3d*> containers;
		vector<object3d*>::iterator i;
		container3d *container0 = container;
		mesh *mesh0;

		while(true)
		{
			if(container0->visible)
			{
				for(i = container0->objects.begin(); i != container0->objects.end(); i++)
				{
					if(typeid(**i) == typeid(container3d))
					{
						containers.push_back(static_cast<container3d*>(*i));
					} 
					else 
					{
						mesh0 = static_cast<mesh*>(*i);
						if(!mesh0->cliping || mesh0->visible && camera->culling.classifyAABB(mesh0->bound, camera->invertWorldMatrix))
						{
							if(mesh0->alphaTest)
							{
								mesh0->alphaKey = mesh0->worldMatrix.raw[12] * mesh0->worldMatrix.raw[12] + 
													mesh0->worldMatrix.raw[13] * mesh0->worldMatrix.raw[13] + 
													mesh0->worldMatrix.raw[14] * mesh0->worldMatrix.raw[14];
								alphaObjects.push_back(mesh0);
							}
							else 
							{
								mesh0->draw(id, camera);
								objects.push_back(*i);
							}
							numObjects++;
							numTriangles += mesh0->numTriangles();
							numVertices += mesh0->numVertices();
						}
					}
				}
			}
			if(containers.size() != 0)
			{
				container0 = containers.back();
				containers.pop_back();
				continue;
			}
			break;
		}

		if(partition != NULL)
		{
			vector<ocnode*> nodes;
			vector<ocnode*>::iterator j;
			ocnode *node0 = partition->root;
			ocnode *node1 = NULL;
			vector<mesh*>::iterator k;
			vector<bool> nodemarks;
			bool current;
			bool parent = false;

			while(true)
			{
				if(parent || camera->culling.classifyAABB(node0->bound, camera->invertWorldMatrix))
				{
					current = parent ? true : camera->culling.classifyPoint(node0->bound.min) && 
							camera->culling.classifyPoint(vector4(node0->bound.max.x, node0->bound.min.y, node0->bound.min.z, 1.0f)) && 
							camera->culling.classifyPoint(vector4(node0->bound.min.x, node0->bound.max.y, node0->bound.min.z, 1.0f)) && 
							camera->culling.classifyPoint(vector4(node0->bound.min.x, node0->bound.min.y, node0->bound.max.z, 1.0f)) && 
							camera->culling.classifyPoint(vector4(node0->bound.min.x, node0->bound.max.y, node0->bound.max.z, 1.0f)) && 
							camera->culling.classifyPoint(vector4(node0->bound.max.x, node0->bound.min.y, node0->bound.max.z, 1.0f)) && 
							camera->culling.classifyPoint(vector4(node0->bound.max.x, node0->bound.max.y, node0->bound.min.z, 1.0f)) && 
							camera->culling.classifyPoint(node0->bound.max);
					if(node0->objects.size() != 0)
					{
						for(k = node0->objects.begin(); k != node0->objects.end(); k++)
						{
							mesh0 = *k;
							if(mesh0->visible && (current || camera->culling.classifyAABB(mesh0->bound, camera->invertWorldMatrix)))
							{
								if(mesh0->alphaTest)
								{
									mesh0->alphaKey = mesh0->worldMatrix.raw[12] * mesh0->worldMatrix.raw[12] + 
														mesh0->worldMatrix.raw[13] * mesh0->worldMatrix.raw[13] + 
														mesh0->worldMatrix.raw[14] * mesh0->worldMatrix.raw[14];
									alphaObjects.push_back(mesh0);
								}
								else 
								{
									mesh0->draw(id, camera);
									objects.push_back(*k);
								}
								numObjects++;
								numTriangles += mesh0->numTriangles();
								numVertices += mesh0->numVertices();
							}
						}
					}
					for(j = node0->childs.begin(); j != node0->childs.end(); j++)
					{
						node1 = *j;
						if(node1 != NULL && camera->culling.classifyAABB(node1->bound, camera->invertWorldMatrix))
						{
							nodes.push_back(node1);
							nodemarks.push_back(current);
						}
					}
				}
				if(nodes.size() != 0)
				{
					node0 = nodes.back();
					nodes.pop_back();
					parent = nodemarks.back();
					nodemarks.pop_back();
					continue;
				}
				break;
			}
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(alphaObjects.size() > 1) std::sort(alphaObjects.begin(), alphaObjects.end(), *this);

		vector<mesh*>::iterator j;
		for(j = alphaObjects.begin(); j != alphaObjects.end(); j++)
			static_cast<mesh*>(*j)->draw(id, camera);
		
		glDisable(GL_BLEND);
	}
}