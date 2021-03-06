#include <algorithm>
#include <iterator>
#include <vector>

#include "Mesh.h"
#include "MeshRender.h"

namespace nest
{
	using namespace std;

	void MeshRender::draw(Mesh *mesh, Matrix4 *worldMatrix, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix)
	{
		this->invertViewMatrix = invertViewMatrix;
		this->projectionMatrix = projectionMatrix;
		this->worldMatrix = worldMatrix;
		if(mesh->skin != NULL)
			draw(mesh->skin->root);
		else
			draw(mesh);
	}

	void MeshRender::draw(Joint *joint)
	{
		if(joint->mesh != NULL)
			draw(joint->mesh, &joint->combinedMatrix);
		if(joint->sibling != NULL)
			draw(joint->sibling);
		if(joint->firstChild != NULL)
			draw(joint->firstChild);
	}

	void MeshRender::draw(Mesh *mesh, Matrix4 *combinedMatrix)
	{
		// find shader
		map<string, ShaderInfo>::iterator it0;
		it0 = mesh->shaderMap.find(flag);
		if(it0 != mesh->shaderMap.end())
		{
			ShaderInfo sInfo = it0->second;
			Shader *shader = static_cast<Shader*>(sInfo.shader);
			// link shader
			glUseProgram(shader->program);
			if(combinedMatrix != NULL) 
				glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::COMBINED_MATRIX), 1, false, combinedMatrix->raw);
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::WORLD_MATRIX), 1, false, worldMatrix->raw);
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::INVERT_VIEW_MATRIX), 1, false, invertViewMatrix->raw);
			glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::PROJECTION_MATRIX), 1, false, projectionMatrix->raw);
			// link shader textures
			map<string, TextureInfo>::iterator it1;
			int id = 0, gl_id = GL_TEXTURE0;
			TextureInfo tInfo;
			for(it1 = shader->textureMap.begin(); it1 != shader->textureMap.end(); ++it1)
			{
				tInfo = it1->second;
				glActiveTexture(gl_id);
				glBindTexture(tInfo.target, tInfo.texture);
				glUniform1i(glGetUniformLocation(shader->program, it1->first.c_str()), id);
				gl_id++;
				id++;
			}
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
					copy(matrix, matrix + 16, matrices + j);
					j += 16;
				}
				// upload joint array's finalMatrices
				glUniformMatrix4fv(glGetUniformLocation(shader->program, Shader::SKELETON), k, false, matrices);
				delete matrices;
			}
			// upload shaderparts
			vector<ShaderPart*>::iterator part;
			for(part = shader->parts.begin(); part != shader->parts.end(); ++part)
				(*part)->upload();
			// link vao
			glBindVertexArray(shader->vao);
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
}