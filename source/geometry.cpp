#include <cmath>

#include "geometry.h"
#include "matrix4.h"
#include "vector4.h"

namespace nest
{
	geometry::geometry()
	{
		vertexData = NULL;
		uvData = NULL;
		normalData = NULL;
		tangentData = NULL;
		indexData = NULL;
		vertexBuffer = 0;
		uvBuffer = 0;
		normalBuffer = 0;
		tangentBuffer = 0;
		indexBuffer = 0;
		numVertices = 0;
		numTriangles = 0;
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
		if(vertexData != NULL) delete [] vertexData;
		if(uvData != NULL) delete [] uvData;
		if(normalData != NULL) delete [] normalData;
		if(tangentData != NULL) delete [] tangentData;
		if(indexData != NULL) delete [] indexData;
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

	void geometry::calculateNormal(geometry &geom)
	{
		if(geom.normalData == NULL) geom.normalData = new GLfloat[geom.numVertices * 3];

		int i, j;
		for(i = 0; i < geom.numVertices; i++)
		{
			j = i * 3;
			geom.normalData[j] = 0;
			geom.normalData[j + 1] = 0;
			geom.normalData[j + 2] = 0;
		}

		GLuint a, b, c;
		GLfloat x1, y1, z1, x2, y2, z2, x3, y3, z3;
		vector4 vt1, vt2;

		for(i = 0; i < geom.numTriangles; i++)
		{
			j = i * 3;
			a = geom.indexData[j] * 3;
			x1 = geom.vertexData[a];
			y1 = geom.vertexData[a + 1];
			z1 = geom.vertexData[a + 2];
			b = geom.indexData[j + 1] * 3;
			x2 = geom.vertexData[b];
			y2 = geom.vertexData[b + 1];
			z2 = geom.vertexData[b + 2];
			c = geom.indexData[j + 2] * 3;
			x3 = geom.vertexData[c];
			y3 = geom.vertexData[c + 1];
			z3 = geom.vertexData[c + 2];

			vt1.x = x2 - x1;
			vt1.y = y2 - y1;
			vt1.z = z2 - z1;
			vt2.x = x3 - x2;
			vt2.y = y3 - y2;
			vt2.z = z3 - z2;
			vt1 = crossProduct(vt1, vt2);
			vt1.normalize();

			geom.normalData[a] += vt1.x;
			geom.normalData[a + 1] += vt1.y;
			geom.normalData[a + 2] += vt1.z;
			geom.normalData[b] += vt1.x;
			geom.normalData[b + 1] += vt1.y;
			geom.normalData[b + 2] += vt1.z;
			geom.normalData[c] += vt1.x;
			geom.normalData[c + 1] += vt1.y;
			geom.normalData[c + 2] += vt1.z;
		}

		for(i = 0; i < geom.numVertices; i++)
		{
			j = i * 3;
			x3 = geom.normalData[j];
			y3 = geom.normalData[j + 1];
			z3 = geom.normalData[j + 2];
			x1 = 1 / sqrt(x3 * x3 + y3 * y3 + z3 * z3);
			geom.normalData[j] *= x1;
			geom.normalData[j + 1] *= x1;
			geom.normalData[j + 2] *= x1;
		}
	}

	void geometry::calculateTangent(geometry &geom)
	{
		if(geom.normalData == NULL) calculateNormal(geom);
		if(geom.tangentData == NULL) geom.tangentData = new GLfloat[geom.numVertices * 3];

		int i, j;
		for(i = 0; i < geom.numVertices; i++)
		{
			j = i * 3;
			geom.tangentData[j] = 0;
			geom.tangentData[j + 1] = 0;
			geom.tangentData[j + 2] = 0;
		}

		GLuint a, b, c, d;
		GLfloat u1, u2, u3, v1, v2, v3;
		vector4 p1, p2, p3, p4, p5;

		for(i = 0; i < geom.numTriangles; i++)
		{
			j = i * 3;
			a = geom.indexData[j] * 3;
			p1.x = geom.vertexData[a];
			p1.y = geom.vertexData[a + 1];
			p1.z = geom.vertexData[a + 2];
			b = geom.indexData[j + 1] * 3;
			p2.x = geom.vertexData[b];
			p2.y = geom.vertexData[b + 1];
			p2.z = geom.vertexData[b + 2];
			c = geom.indexData[j + 2] * 3;
			p3.x = geom.vertexData[c];
			p3.y = geom.vertexData[c + 1];
			p3.z = geom.vertexData[c + 2];

			d = geom.indexData[j] * 2;
			u1 = geom.uvData[d];
			v1 = geom.uvData[d + 1];
			d = geom.indexData[j + 1] * 2;
			u2 = geom.uvData[d];
			v2 = geom.uvData[d + 1];
			d = geom.indexData[j + 2] * 2;
			u3 = geom.uvData[d];
			v3 = geom.uvData[d + 1];

			p4 = (p2 - p1) * (v3 - v1);
			p5 = (p3 - p1) * (v2 - v1);
			p4 -= p5;
			p4 *= 1 / ((u2 - u1) * (v3 - v1) - (v2 - v1) * (u3 - u1));

			geom.tangentData[a] += p4.x;
			geom.tangentData[a + 1] += p4.y;
			geom.tangentData[a + 2] += p4.z;
			geom.tangentData[b] += p4.x;
			geom.tangentData[b + 1] += p4.y;
			geom.tangentData[b + 2] += p4.z;
			geom.tangentData[c] += p4.x;
			geom.tangentData[c + 1] += p4.y;
			geom.tangentData[c + 2] += p4.z;
		}

		for (i = 0; i < geom.numVertices; i++) {
			j = i * 3;
			v1 = geom.tangentData[j];
			v2 = geom.tangentData[j + 1];
			v3 = geom.tangentData[j + 2];
			u1 = 1 / sqrt(v1 * v1 + v2 * v2 + v3 * v3);
			geom.tangentData[j] *= u1;
			geom.tangentData[j + 1] *= u1;
			geom.tangentData[j + 2] *= u1;
		}
	}

	void geometry::configure(geometry &geom, int params)
	{
		GLuint count = 0;
		if(geom.attributeArray != 0) glDeleteVertexArrays(1, &geom.attributeArray);
		glGenVertexArrays(1, &geom.attributeArray);
		glBindVertexArray(geom.attributeArray);
		if(params & GEOM_VERTEX)
		{
			if(geom.vertexBuffer != 0) glDeleteBuffers(1, &geom.vertexBuffer);
			geom.vertexBuffer = 0;
			if(geom.vertexData != NULL)
			{
				glGenBuffers(1, &geom.vertexBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.vertexBuffer);
				glBufferData(GL_ARRAY_BUFFER, geom.numVertices * 3 * sizeof(GLfloat), geom.vertexData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(count);
				glVertexAttribPointer(count++, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		if(params & GEOM_UV)
		{
			if(geom.uvBuffer != 0) glDeleteBuffers(1, &geom.uvBuffer);
			geom.uvBuffer = 0;
			if(geom.uvData != NULL)
			{
				glGenBuffers(1, &geom.uvBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.uvBuffer);
				glBufferData(GL_ARRAY_BUFFER, geom.numVertices * 2 * sizeof(GLfloat), geom.uvData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(count);
				glVertexAttribPointer(count++, 2, GL_FLOAT, GL_FALSE, 0, 0);
			}
		}
		if(params & GEOM_NORMAL)
		{
			if(geom.normalBuffer != 0) glDeleteBuffers(1, &geom.normalBuffer);
			geom.normalBuffer = 0;
			if(geom.normalData != NULL)
			{
				glGenBuffers(1, &geom.normalBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.normalBuffer);
				glBufferData(GL_ARRAY_BUFFER, geom.numVertices * 3 * sizeof(GLfloat), geom.normalData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(count);
				glVertexAttribPointer(count++, 3, GL_FLOAT, GL_TRUE, 0, 0);
			}
		}
		if(params & GEOM_TANGENT)
		{
			if(geom.tangentBuffer != 0) glDeleteBuffers(1, &geom.tangentBuffer);
			geom.tangentBuffer = 0;
			if(geom.tangentData != NULL)
			{
				glGenBuffers(1, &geom.tangentBuffer);
				glBindBuffer(GL_ARRAY_BUFFER, geom.tangentBuffer);
				glBufferData(GL_ARRAY_BUFFER, geom.numVertices * 3 * sizeof(GLfloat), geom.tangentData, GL_STATIC_DRAW);
				glEnableVertexAttribArray(count);
				glVertexAttribPointer(count++, 3, GL_FLOAT, GL_TRUE, 0, 0);
			}
		}
		if(geom.indexBuffer != 0) glDeleteBuffers(1, &geom.indexBuffer);
		geom.indexBuffer = 0;
		if(geom.indexData != NULL)
		{
			glGenBuffers(1, &geom.indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom.numTriangles * 3 * sizeof(GLuint), geom.indexData, GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}