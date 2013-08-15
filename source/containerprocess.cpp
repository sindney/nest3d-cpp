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
					else if(typeid(**i) == typeid(mesh))
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
								drawMesh(mesh0);
								objects.push_back(*i);
							}
							numObjects++;
							numTriangles += mesh0->geom->numTriangles;
							numVertices += mesh0->geom->numVertices;
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
			vector<mesh*>::iterator k;
			ocnode *node0 = partition->root;
			ocnode *node1 = NULL;
			bool all;

			while(true)
			{
				if(camera->culling.classifyAABB(node0->bound, camera->invertWorldMatrix))
				{
					all = camera->culling.classifyPoint(node0->bound.min) && 
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
							if(mesh0->visible && (all || camera->culling.classifyAABB(mesh0->bound, camera->invertWorldMatrix)))
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
									drawMesh(mesh0);
									objects.push_back(*k);
								}
								numObjects++;
								numTriangles += mesh0->geom->numTriangles;
								numVertices += mesh0->geom->numVertices;
							}
						}
					}
					for(j = node0->childs.begin(); j != node0->childs.end(); j++)
					{
						node1 = *j;
						if(node1 != NULL && camera->culling.classifyAABB(node1->bound, camera->invertWorldMatrix))
						{
							nodes.push_back(node1);
						}
					}
				}
				if(nodes.size() != 0)
				{
					node0 = nodes.back();
					nodes.pop_back();
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
			drawMesh(static_cast<mesh*>(*j));
		
		glDisable(GL_BLEND);
	}

	void containerprocess::drawMesh(mesh *mesh0)
	{
		if(mesh0->faceCulling)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(mesh0->face);
		}
		else glDisable(GL_CULL_FACE);

		glUseProgram(mesh0->shader->program);
		glUniformMatrix4fv(glGetUniformLocation(mesh0->shader->program, shader3d::SHADER_WORLD_MATRIX), 1, false, mesh0->worldMatrix.raw);
		glUniformMatrix4fv(glGetUniformLocation(mesh0->shader->program, shader3d::SHADER_INVERT_VIEW_MATRIX), 1, false, camera->invertWorldMatrix.raw);
		glUniformMatrix4fv(glGetUniformLocation(mesh0->shader->program, shader3d::SHADER_PROJECTION_MATRIX), 1, false, camera->projectionMatrix.raw);

		int i, j = mesh0->shader->parts.size();
		for(i = 0; i < j; i++)
			mesh0->shader->parts[i]->upload();

		glBindVertexArray(mesh0->geom->attributeArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh0->geom->indexBuffer);

		glDrawElements(GL_TRIANGLES, mesh0->geom->numTriangles * 3, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}