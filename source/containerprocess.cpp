#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <vector>

#include "container3d.h"
#include "containerprocess.h"
#include "mesh.h"
#include "object3d.h"

namespace nest
{
	void containerprocess::calculate()
	{
		vector<container3d*> containers;
		vector<object3d*>::iterator i;
		container3d *current = container;
		mesh *mesh0;

		objects.clear();
		alphaObjects.clear();
		numObjects = 0;
		numTriangles = 0;
		numVertices = 0;

		glBindFramebuffer(GL_FRAMEBUFFER, target ? target->frameBuffer : 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		while(true)
		{
			if(current->visible)
			{
				for(i = current->objects.begin(); i != current->objects.end(); i++)
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
							numTriangles += mesh0->geom->indexDataSize;
							numVertices += mesh0->geom->vertexDataSize;
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

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(alphaObjects.size() > 1) std::sort(alphaObjects.begin(), alphaObjects.end(), *this);

		vector<mesh*>::iterator j;
		for(j = alphaObjects.begin(); j != alphaObjects.end(); j++)
			drawMesh(static_cast<mesh*>(*j));

		glBlendFunc(GL_ONE, GL_ZERO);
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

		glBindVertexArray(mesh0->geom->attributeArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh0->geom->indexBuffer);

		glDrawElements(GL_TRIANGLES, mesh0->geom->indexDataSize, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}