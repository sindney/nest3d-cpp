#include <algorithm>
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
		if(node->mesh->skin)
			draw(node->mesh->skin->root);
		else
			draw(node->mesh);
	}

	void MeshRender::draw(Joint *joint)
	{
		if(joint->mesh != NULL)
			draw(joint->mesh, joint->mesh->skin != NULL ? NULL : &joint->combinedMatrix);
		if(joint->sibling != NULL)
			draw(joint->sibling);
		if(joint->firstChild != NULL)
			draw(joint->firstChild);
	}

	void MeshRender::draw(Mesh *mesh, Matrix4 *combinedMatrix)
	{
		if(mesh->faceCulling)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(mesh->face);
		}
		else glDisable(GL_CULL_FACE);
		// link shaders
		Shader *shader = mesh->shaders[flag];
		glUseProgram(shader->program);
		if(combinedMatrix == NULL)
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::WORLD_MATRIX), 1, false, worldMatrix->raw);
		else
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::WORLD_MATRIX), 1, false, ((*worldMatrix) * (*combinedMatrix)).raw);
		glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::INVERT_VIEW_MATRIX), 1, false, invertViewMatrix->raw);
		glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::PROJECTION_MATRIX), 1, false, projectionMatrix->raw);
		// skin info
		SkinInfo *skin = mesh->skin;
		if(skin != NULL)
		{
			// draw skined mesh with hardware
			int i, j = 0, k = skin->joints.size();
			GLfloat *matrices = new GLfloat[k * 16];
			GLfloat *matrix = NULL;
			for(i = 0; i < k; i++)
			{
				matrix = &skin->joints[i]->finalMatrix.raw[0];
				std::copy(matrix, matrix + 16, matrices + j);
				j += 16;
			}
			// upload joint array's finalMatrices
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::SKELETON), k, false, matrices);
			delete matrices;
		}
		// upload shaderparts
		std::vector<ShaderPart*>::iterator part;
		for(part = shader->parts.begin(); part != shader->parts.end(); part++)
			(*part)->upload();
		// link geometry buffers
		glBindVertexArray(mesh->geometry->attributeArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->geometry->indexBuffer);
		// draw mesh
		glDrawElements(GL_TRIANGLES, mesh->geometry->numTris * 3, GL_UNSIGNED_INT, 0);
		// unlink data
		glUseProgram(0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// update counter
		numDraws++;
		numTris += mesh->geometry->numTris;
		numVts += mesh->geometry->numVts;
	}
}