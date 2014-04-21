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

	void Geometry::configure(int params)
	{
		GLuint count = 0;
		if(attributeArray != 0) glDeleteVertexArrays(1, &attributeArray);
		glGenVertexArrays(1, &attributeArray);
		glBindVertexArray(attributeArray);
		// vertex
		if(vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
		vertexBuffer = 0;
		if(vertexData != NULL && params & GEOM_VERTEX)
		{
			glGenBuffers(1, &vertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVts * 3 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(count);
			glVertexAttribPointer(count++, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		// uv
		if(uvBuffer != 0) glDeleteBuffers(1, &uvBuffer);
		uvBuffer = 0;
		if(uvData != NULL && params & GEOM_UV)
		{
			glGenBuffers(1, &uvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVts * 2 * sizeof(GLfloat), uvData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(count);
			glVertexAttribPointer(count++, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}
		// normal
		if(normalBuffer != 0) glDeleteBuffers(1, &normalBuffer);
		normalBuffer = 0;
		if(normalData != NULL && params & GEOM_NORMAL)
		{
			glGenBuffers(1, &normalBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVts * 3 * sizeof(GLfloat), normalData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(count);
			glVertexAttribPointer(count++, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		// tangent
		if(tangentBuffer != 0) glDeleteBuffers(1, &tangentBuffer);
		tangentBuffer = 0;
		if(tangentData != NULL && params & GEOM_TANGENT)
		{
			glGenBuffers(1, &tangentBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVts * 3 * sizeof(GLfloat), tangentData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(count);
			glVertexAttribPointer(count++, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		// joint indices
		if(indicesBuffer != 0) glDeleteBuffers(1, &indicesBuffer);
		indicesBuffer = 0;
		if(indicesData != NULL && params & GEOM_INDICES)
		{
			glGenBuffers(1, &indicesBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, indicesBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVts * jointPerVertex * sizeof(GLuint), indicesData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(count);
			glVertexAttribIPointer(count++, jointPerVertex, GL_UNSIGNED_INT, 0, 0);
		}
		// joint weights
		if(weightsBuffer != 0) glDeleteBuffers(1, &weightsBuffer);
		weightsBuffer = 0;
		if(weightsData != NULL && params & GEOM_WEIGHTS)
		{
			glGenBuffers(1, &weightsBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, weightsBuffer);
			glBufferData(GL_ARRAY_BUFFER, numVts * (jointPerVertex - 1) * sizeof(GLfloat), weightsData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(count);
			glVertexAttribPointer(count++, jointPerVertex - 1, GL_FLOAT, GL_FALSE, 0, 0);
		}
		if(indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
		indexBuffer = 0;
		if(indexData != NULL)
		{
			glGenBuffers(1, &indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numTris * 3 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}