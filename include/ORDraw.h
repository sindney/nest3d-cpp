#ifndef N3D_ORDRAW_H
#define N3D_ORDRAW_H

#include <iterator>

#include "Mesh.h"

namespace nest
{
	class ORDraw
	{
	public:

		virtual void calculate(Mesh *mesh, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix)
		{
			if(mesh->faceCulling)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(mesh->face);
			}
			else glDisable(GL_CULL_FACE);

			glUseProgram(mesh->shader->program);
			glUniformMatrix4fv(glGetUniformLocation(mesh->shader->program, Shader::WORLD_MATRIX), 1, false, mesh->worldMatrix.raw);
			glUniformMatrix4fv(glGetUniformLocation(mesh->shader->program, Shader::INVERT_VIEW_MATRIX), 1, false, invertViewMatrix->raw);
			glUniformMatrix4fv(glGetUniformLocation(mesh->shader->program, Shader::PROJECTION_MATRIX), 1, false, projectionMatrix->raw);

			std::vector<ShaderPart*>::iterator i;
			for(i = mesh->shader->parts.begin(); i != mesh->shader->parts.end(); i++)
				(*i)->upload();

			glBindVertexArray(mesh->geometry->attributeArray);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->geometry->indexBuffer);

			glDrawElements(GL_TRIANGLES, mesh->geometry->numTris * 3, GL_UNSIGNED_INT, 0);

			glUseProgram(0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	};
}

#endif