#include <cmath>

#include "Geometry.h"

namespace nest
{
	Geometry::Geometry()
	{
		vertexData = NULL;
		uvData = NULL;
		normalData = NULL;
		tangentData = NULL;
		indicesData = NULL;
		weightsData = NULL;
		indexData = NULL;
		vertexBuffer = 0;
		uvBuffer = 0;
		normalBuffer = 0;
		tangentBuffer = 0;
		indicesBuffer = 0;
		weightsBuffer = 0;
		indexBuffer = 0;
		numVts = 0;
		numTris = 0;
		jointPerVertex = 0;
		attributeArray = 0;
	}

	Geometry::~Geometry()
	{
		if(vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
		if(uvBuffer != 0) glDeleteBuffers(1, &uvBuffer);
		if(normalBuffer != 0) glDeleteBuffers(1, &normalBuffer);
		if(tangentBuffer != 0) glDeleteBuffers(1, &tangentBuffer);
		if(indicesBuffer != 0) glDeleteBuffers(1, &indicesBuffer);
		if(weightsBuffer != 0) glDeleteBuffers(1, &weightsBuffer);
		if(indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
		if(attributeArray != 0) glDeleteVertexArrays(1, &attributeArray);
		if(vertexData != NULL) delete [] vertexData;
		if(uvData != NULL) delete [] uvData;
		if(normalData != NULL) delete [] normalData;
		if(tangentData != NULL) delete [] tangentData;
		if(indicesData != NULL) delete [] indicesData;
		if(weightsData != NULL) delete [] weightsData;
		if(indexData != NULL) delete [] indexData;
	}

	void Geometry::configure(bool software)
	{
		
	}

	void Geometry::calculateNormal(Geometry *geom)
	{
		if(geom->normalData == NULL) geom->normalData = new GLfloat[geom->numVts * 3];

		int i, j;
		for(i = 0; i < geom->numVts; i++)
		{
			j = i * 3;
			geom->normalData[j] = 0;
			geom->normalData[j + 1] = 0;
			geom->normalData[j + 2] = 0;
		}

		GLuint a, b, c;
		GLfloat x1, y1, z1, x2, y2, z2, x3, y3, z3;
		Vector4 vt1, vt2;

		for(i = 0; i < geom->numTris; i++)
		{
			j = i * 3;
			a = geom->indexData[j] * 3;
			x1 = geom->vertexData[a];
			y1 = geom->vertexData[a + 1];
			z1 = geom->vertexData[a + 2];
			b = geom->indexData[j + 1] * 3;
			x2 = geom->vertexData[b];
			y2 = geom->vertexData[b + 1];
			z2 = geom->vertexData[b + 2];
			c = geom->indexData[j + 2] * 3;
			x3 = geom->vertexData[c];
			y3 = geom->vertexData[c + 1];
			z3 = geom->vertexData[c + 2];

			vt1.x = x2 - x1;
			vt1.y = y2 - y1;
			vt1.z = z2 - z1;
			vt2.x = x3 - x2;
			vt2.y = y3 - y2;
			vt2.z = z3 - z2;
			vt1 = Vector4::crossProduct(vt1, vt2);
			vt1.normalize();

			geom->normalData[a] += vt1.x;
			geom->normalData[a + 1] += vt1.y;
			geom->normalData[a + 2] += vt1.z;
			geom->normalData[b] += vt1.x;
			geom->normalData[b + 1] += vt1.y;
			geom->normalData[b + 2] += vt1.z;
			geom->normalData[c] += vt1.x;
			geom->normalData[c + 1] += vt1.y;
			geom->normalData[c + 2] += vt1.z;
		}

		for(i = 0; i < geom->numVts; i++)
		{
			j = i * 3;
			x3 = geom->normalData[j];
			y3 = geom->normalData[j + 1];
			z3 = geom->normalData[j + 2];
			x1 = 1 / sqrt(x3 * x3 + y3 * y3 + z3 * z3);
			geom->normalData[j] *= x1;
			geom->normalData[j + 1] *= x1;
			geom->normalData[j + 2] *= x1;
		}
	}

	void Geometry::calculateTangent(Geometry *geom)
	{
		if(geom->normalData == NULL) calculateNormal(geom);
		if(geom->tangentData == NULL) geom->tangentData = new GLfloat[geom->numVts * 3];

		int i, j;
		for(i = 0; i < geom->numVts; i++)
		{
			j = i * 3;
			geom->tangentData[j] = 0;
			geom->tangentData[j + 1] = 0;
			geom->tangentData[j + 2] = 0;
		}

		GLuint a, b, c, d;
		GLfloat u1, u2, u3, v1, v2, v3;
		Vector4 p1, p2, p3, p4, p5;

		for(i = 0; i < geom->numTris; i++)
		{
			j = i * 3;
			a = geom->indexData[j] * 3;
			p1.x = geom->vertexData[a];
			p1.y = geom->vertexData[a + 1];
			p1.z = geom->vertexData[a + 2];
			b = geom->indexData[j + 1] * 3;
			p2.x = geom->vertexData[b];
			p2.y = geom->vertexData[b + 1];
			p2.z = geom->vertexData[b + 2];
			c = geom->indexData[j + 2] * 3;
			p3.x = geom->vertexData[c];
			p3.y = geom->vertexData[c + 1];
			p3.z = geom->vertexData[c + 2];

			d = geom->indexData[j] * 2;
			u1 = geom->uvData[d];
			v1 = geom->uvData[d + 1];
			d = geom->indexData[j + 1] * 2;
			u2 = geom->uvData[d];
			v2 = geom->uvData[d + 1];
			d = geom->indexData[j + 2] * 2;
			u3 = geom->uvData[d];
			v3 = geom->uvData[d + 1];

			p4 = (p2 - p1) * (v3 - v1);
			p5 = (p3 - p1) * (v2 - v1);
			p4 -= p5;
			p4 *= 1 / ((u2 - u1) * (v3 - v1) - (v2 - v1) * (u3 - u1));

			geom->tangentData[a] += p4.x;
			geom->tangentData[a + 1] += p4.y;
			geom->tangentData[a + 2] += p4.z;
			geom->tangentData[b] += p4.x;
			geom->tangentData[b + 1] += p4.y;
			geom->tangentData[b + 2] += p4.z;
			geom->tangentData[c] += p4.x;
			geom->tangentData[c + 1] += p4.y;
			geom->tangentData[c + 2] += p4.z;
		}

		for (i = 0; i < geom->numVts; i++) {
			j = i * 3;
			v1 = geom->tangentData[j];
			v2 = geom->tangentData[j + 1];
			v3 = geom->tangentData[j + 2];
			u1 = 1 / sqrt(v1 * v1 + v2 * v2 + v3 * v3);
			geom->tangentData[j] *= u1;
			geom->tangentData[j + 1] *= u1;
			geom->tangentData[j + 2] *= u1;
		}
	}
}