#include <cmath>

#include "geometry.h"
#include "matrix4.h"
#include "vector4.h"

namespace nest
{
	geometry::geometry()
	{
		vertexBuffer = 0;
		uvBuffer = 0;
		normalBuffer = 0;
		tangentBuffer = 0;
		indexBuffer = 0;
		vertexDataSize = 0;
		uvDataSize = 0;
		normalDataSize = 0;
		tangentDataSize = 0;
		indexDataSize = 0;
		attributeArray = 0;
	}

	geometry::~geometry()
	{
		if(vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
		if(uvBuffer != 0) glDeleteBuffers(1, &uvBuffer);
		if(normalBuffer != 0) glDeleteBuffers(1, &normalBuffer);
		if(tangentBuffer != 0) glDeleteBuffers(1, &tangentBuffer);
		if(indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
		if(attributeArray != 0) glDeleteVertexArrays(1, &attributeArray);
	}

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

	// TODO: check if it's right.
	void calculateNormal(geometrydata &data)
	{
		if(data.normalData == NULL) 
			data.normalData = new GLfloat[data.vertexDataSize * 3];
		else if(data.normalDataSize != data.vertexDataSize * 3)
		{
			delete data.normalData;
			data.normalData = new GLfloat[data.vertexDataSize * 3];
		} 
		else
		{
			int k;
			for(k = 0; k < data.normalDataSize; k++)
				data.normalData[k] = 0;
		} 
		data.normalDataSize = data.vertexDataSize * 3;

		int i, j;
		GLuint a, b, c;
		GLfloat x1, y1, z1, x2, y2, z2, x3, y3, z3;
		vector4 vt1, vt2;

		for(i = 0; i < data.indexDataSize; i++)
		{
			j = i * 3;
			a = data.indexData[j] * 3;
			x1 = data.vertexData[a];
			y1 = data.vertexData[a + 1];
			z1 = data.vertexData[a + 2];
			b = data.indexData[j + 1] * 3;
			x2 = data.vertexData[b];
			y2 = data.vertexData[b + 1];
			z2 = data.vertexData[b + 2];
			c = data.indexData[j + 2] * 3;
			x3 = data.vertexData[c];
			y3 = data.vertexData[c + 1];
			z3 = data.vertexData[c + 2];

			vt1.x = x2 - x1;
			vt1.y = y2 - y1;
			vt1.z = z2 - z1;
			vt2.x = x3 - x2;
			vt2.y = y3 - y2;
			vt2.z = z3 - z2;
			vt1 = crossProduct(vt1, vt2);
			vt1.normalize();

			data.normalData[a] += vt1.x;
			data.normalData[a + 1] += vt1.y;
			data.normalData[a + 2] += vt1.z;
			data.normalData[b] += vt1.x;
			data.normalData[b + 1] += vt1.y;
			data.normalData[b + 2] += vt1.z;
			data.normalData[c] += vt1.x;
			data.normalData[c + 1] += vt1.y;
			data.normalData[c + 2] += vt1.z;
		}

		for(i = 0; i < data.vertexDataSize; i++)
		{
			j = i * 3;
			x3 = data.normalData[j];
			y3 = data.normalData[j + 1];
			z3 = data.normalData[j + 2];
			x1 = 1 / sqrt(x3 * x3 + y3 * y3 + z3 * z3);
			data.normalData[j] *= x1;
			data.normalData[j + 1] *= x1;
			data.normalData[j + 2] *= x1;
		}
	}

	void calculateTangent(geometrydata &data)
	{
		if(data.normalData == NULL) calculateNormal(data);
		if(data.tangentData == NULL) 
			data.tangentData = new GLfloat[data.normalDataSize];
		else if(data.tangentDataSize != data.normalDataSize)
		{
			delete data.tangentData;
			data.tangentData = new GLfloat[data.normalDataSize];
		}
		else
		{
			int k;
			for(k = 0; k < data.tangentDataSize; k++)
				data.tangentData[k] = 0;
		} 
		data.tangentDataSize = data.normalDataSize;

		int i, j;
		GLuint a, b, c, d;
		GLfloat u1, u2, u3, v1, v2, v3;
		vector4 p1, p2, p3, p4, p5;

		for(i = 0; i < data.indexDataSize; i++)
		{
			j = i * 3;
			a = data.indexData[j] * 3;
			p1.x = data.vertexData[a];
			p1.y = data.vertexData[a + 1];
			p1.z = data.vertexData[a + 2];
			b = data.indexData[j + 1] * 3;
			p2.x = data.vertexData[b];
			p2.y = data.vertexData[b + 1];
			p2.z = data.vertexData[b + 2];
			c = data.indexData[j + 2] * 3;
			p3.x = data.vertexData[c];
			p3.y = data.vertexData[c + 1];
			p3.z = data.vertexData[c + 2];

			d = data.indexData[j] * 2;
			u1 = data.uvData[d];
			v1 = data.uvData[d + 1];
			d = data.indexData[j + 1] * 2;
			u2 = data.uvData[d];
			v2 = data.uvData[d + 1];
			d = data.indexData[j + 2] * 2;
			u3 = data.uvData[d];
			v3 = data.uvData[d + 1];

			p4 = (p2 - p1) * (v3 - v1);
			p5 = (p3 - p1) * (v2 - v1);
			p4 -= p5;
			p4 *= 1 / ((u2 - u1) * (v3 - v1) - (v2 - v1) * (u3 - u1));

			data.tangentData[a] += p4.x;
			data.tangentData[a + 1] += p4.y;
			data.tangentData[a + 2] += p4.z;
			data.tangentData[b] += p4.x;
			data.tangentData[b + 1] += p4.y;
			data.tangentData[b + 2] += p4.z;
			data.tangentData[c] += p4.x;
			data.tangentData[c + 1] += p4.y;
			data.tangentData[c + 2] += p4.z;
		}

		for (i = 0; i < data.vertexDataSize; i++) {
			j = i * 3;
			v1 = data.tangentData[j];
			v2 = data.tangentData[j + 1];
			v3 = data.tangentData[j + 2];
			u1 = 1 / sqrt(v1 * v1 + v2 * v2 + v3 * v3);
			data.tangentData[j] *= u1;
			data.tangentData[j + 1] *= u1;
			data.tangentData[j + 2] *= u1;
		}
	}

	void emptyGeometryData(geometrydata &data)
	{
		if(data.vertexData != NULL) delete [] data.vertexData;
		if(data.uvData != NULL) delete [] data.uvData;
		if(data.normalData != NULL) delete [] data.normalData;
		if(data.tangentData != NULL) delete [] data.tangentData;
		if(data.indexData != NULL) delete [] data.indexData;
	}
}