#include "geometry.h"
#include "matrix4.h"

namespace nest
{
	void geometry::setupAABB(aabb &bound, const float *vertexData, const int vertexDataSize)
	{
		bound.max.x = bound.min.x = vertexData[0];
		bound.max.y = bound.min.y = vertexData[1];
		bound.max.z = bound.min.z = vertexData[2];
		
		float x, y, z;
		int i;

		for(i = 0; i < vertexDataSize; i += 3)
		{
			x = vertexData[i];
			y = vertexData[i + 1];
			z = vertexData[i + 2];
			if(x > bound.max.x) 
				bound.max.x = x;
			else if(x < bound.min.x)
				bound.min.x = x;
			if(y > bound.max.y) 
				bound.max.y = y;
			else if(y < bound.min.y)
				bound.min.y = y;
			if(z > bound.max.z) 
				bound.max.z = z;
			else if(z < bound.min.z)
				bound.min.z = z;
		}
	}

	void geometry::setupGeometry(geometry &geom, const geometrydata &data, bool vertex = false, bool uv = false, bool normal = false, bool tangent = false, bool index = false)
	{
		if(geom.attributeArray != 0) glDeleteVertexArrays(1, &geom.attributeArray);
		glGenVertexArrays(1, &geom.attributeArray);
		glBindVertexArray(geom.attributeArray);
		if(vertex)
		{
			if(geom.vertexBuffer != 0) glDeleteBuffers(1, &geom.vertexBuffer);
			geom.vertexBuffer = 0;
			if(data.vertexData != NULL)
			{
				glGenBuffers(1, &geom.vertexBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, data.vertexDataSize * sizeof(GLfloat), data.vertexData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
				geom.vertexDataSize = data.vertexDataSize;
			}
		}
		if(uv)
		{
			if(geom.uvBuffer != 0) glDeleteBuffers(1, &geom.uvBuffer);
			geom.uvBuffer = 0;
			if(data.uvData != NULL)
			{
				glGenBuffers(1, &geom.uvBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.uvBuffer);
				glBufferData(GL_ARRAY_BUFFER, data.uvDataSize * sizeof(GLfloat), data.uvData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
				geom.uvDataSize = data.uvDataSize;
			}
		}
		if(normal)
		{
			if(geom.normalBuffer != 0) glDeleteBuffers(1, &geom.normalBuffer);
			geom.normalBuffer = 0;
			if(data.normalData != NULL)
			{
				glGenBuffers(1, &geom.normalBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.normalBuffer);
				glBufferData(GL_ARRAY_BUFFER, data.normalDataSize * sizeof(GLfloat), data.normalData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, 0);
				geom.normalDataSize = data.normalDataSize;
			}
		}
		if(tangent)
		{
			if(geom.tangentBuffer != 0) glDeleteBuffers(1, &geom.tangentBuffer);
			geom.tangentBuffer = 0;
			if(data.tangentData != NULL)
			{
				glGenBuffers(1, &geom.tangentBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.tangentBuffer);
				glBufferData(GL_ARRAY_BUFFER, data.tangentDataSize * sizeof(GLfloat), data.tangentData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_TRUE, 0, 0);
				geom.tangentDataSize = data.tangentDataSize;
			}
		}
		if(index)
		{
			if(geom.indexBuffer != 0) glDeleteBuffers(1, &geom.indexBuffer);
			geom.indexBuffer = 0;
			if(data.indexData != NULL)
			{
				glGenBuffers(1, &geom.indexBuffer);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.indexBuffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indexDataSize * sizeof(GLuint), data.indexData, GL_STATIC_DRAW);
				geom.indexDataSize = data.indexDataSize;
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void geometry::transformAABB(const matrix4 &mat, const aabb &target, aabb &dest)
	{
		vector4 vertices[] = {
			target.min * mat, 
			vector4(target.max.x, target.min.y, target.min.z, 1.0f) * mat, 
			vector4(target.min.x, target.max.y, target.min.z, 1.0f) * mat, 
			vector4(target.max.x, target.max.y, target.min.z, 1.0f) * mat, 
			vector4(target.min.x, target.min.y, target.max.z, 1.0f) * mat, 
			vector4(target.max.x, target.min.y, target.max.z, 1.0f) * mat, 
			vector4(target.min.x, target.max.y, target.max.z, 1.0f) * mat, 
			target.max * mat, 
		};

		dest.max = dest.min = vertices[0];

		int i;
		vector4 *vt;
		for(i = 0; i < 8; i++)
		{
			vt = &vertices[i];
			if(vt->x > dest.max.x) 
				dest.max.x = vt->x;
			else if(vt->x < dest.min.x)
				dest.min.x = vt->x;
			if(vt->y > dest.max.y) 
				dest.max.y = vt->y;
			else if(vt->y < dest.min.y)
				dest.min.y = vt->y;
			if(vt->z > dest.max.z) 
				dest.max.z = vt->z;
			else if(vt->z < dest.min.z)
				dest.min.z = vt->z;
		}
	}
}