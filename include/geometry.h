#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GL/glew.h"

#include "matrix4.h"
#include "vector4.h"

namespace nest
{
	typedef struct 
	{
		vector4 max, min;
	} aabb;

	typedef struct 
	{
		GLfloat *vertexData;

		GLfloat *uvData;

		GLfloat *normalData;

		GLfloat *tangentData;

		GLuint *indexData;

		GLuint vertexDataSize;

		GLuint uvDataSize;

		GLuint normalDataSize;

		GLuint tangentDataSize;

		GLuint indexDataSize;

	} geometrydata;

	class geometry
	{
	public:

		aabb bound;

		GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer;

		GLuint indexBuffer;

		GLuint vertexDataSize, uvDataSize, normalDataSize, tangentDataSize;

		GLuint indexDataSize;

		GLuint attributeArray;

		geometry()
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

		~geometry()
		{
			if(vertexBuffer != 0) glDeleteBuffers(1, &vertexBuffer);
			if(uvBuffer != 0) glDeleteBuffers(1, &uvBuffer);
			if(normalBuffer != 0) glDeleteBuffers(1, &normalBuffer);
			if(tangentBuffer != 0) glDeleteBuffers(1, &tangentBuffer);
			if(indexBuffer != 0) glDeleteBuffers(1, &indexBuffer);
			if(attributeArray != 0) glDeleteVertexArrays(1, &attributeArray);
		}

		static void setupAABB(aabb &bound, const float *vertexData, const int vertexDataSize);

		static void setupGeometry(geometry &geom, const geometrydata &data, bool vertex, bool uv, bool normal, bool tangent, bool index);

		static void transformAABB(const matrix4 &mat, const aabb &target, aabb &dest);
	};
}

#endif