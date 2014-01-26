#include <iterator>
#include <vector>

#include "Mesh.h"
#include "MeshNode.h"
#include "MeshRender.h"

namespace nest
{
	void MeshRender::draw(MeshNode *node, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix)
	{
		this->invertViewMatrix = invertViewMatrix;
		this->projectionMatrix = projectionMatrix;
		this->worldMatrix = &node->worldMatrix;
		this->combinedMatrix = NULL;
		if(node->mesh->skin)
			draw(node->mesh->skin->root);
		else
			draw(node->mesh);
	}

	void MeshRender::draw(Joint *joint)
	{
		if(joint->mesh != NULL)
		{
			Mesh *mesh = joint->mesh;
			if(mesh->skin != NULL)
			{
				SkinInfo *skin = mesh->skin;
				if(skin->software)
				{
					// draw skined mesh with software
					// update mesh's geometry's vertex/normal/tangent with binding joint and it's weights.
					// draw as static mesh then.
				}
				else 
				{
					// draw skined mesh with hardware
					// upload vertex/normal and joint indices/weights to gl.
					// and upload joints' finalMatrices to gl.
					// then compute vertex pos/normal in shader refer to joint indices&weights(int to id it's amount usually 4).
				}
				numMeshes++;
				numTris += mesh->geometry->numTris;
				numVts += mesh->geometry->numVts;
			}
			else
			{
				// draw static mesh with joint->combinedMatrix.
				combinedMatrix = &joint->combinedMatrix;
				draw(mesh);
			}
		}
		if(joint->sibling != NULL)
			draw(joint->sibling);
		if(joint->firstChild)
			draw(joint->firstChild);
	}

	void MeshRender::draw(Mesh *mesh)
	{
		if(mesh->faceCulling)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(mesh->face);
		}
		else glDisable(GL_CULL_FACE);

		Shader *shader = mesh->shaders[flag];

		glUseProgram(shader->program);
		if(combinedMatrix == NULL)
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::WORLD_MATRIX), 1, false, worldMatrix->raw);
		else
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::WORLD_MATRIX), 1, false, ((*worldMatrix) * (*combinedMatrix)).raw);
		glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::INVERT_VIEW_MATRIX), 1, false, invertViewMatrix->raw);
		glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::PROJECTION_MATRIX), 1, false, projectionMatrix->raw);

		std::vector<ShaderPart*>::iterator i;
		for(i = shader->parts.begin(); i != shader->parts.end(); i++)
			(*i)->upload();

		glBindVertexArray(mesh->geometry->attributeArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->geometry->indexBuffer);

		glDrawElements(GL_TRIANGLES, mesh->geometry->numTris * 3, GL_UNSIGNED_INT, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		numMeshes++;
		numTris += mesh->geometry->numTris;
		numVts += mesh->geometry->numVts;
	}
}