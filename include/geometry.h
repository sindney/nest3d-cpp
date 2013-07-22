#ifndef GEOMETRY_H
#define GEOMETRY_H

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

		geometry();

		~geometry();

		/**
		 *	Calculate target vertex array's bounding-box.
		 */
		static void setupAABB(aabb &bound, const float *vertexData, const int vertexDataSize);

		/**
		 *	Setup target geometry object by corresponding geometrydata for opengl rendering pipeline.
		 */
		static void setupGeometry(geometry &geom, const geometrydata &data, bool vertex, bool uv, bool normal, bool tangent, bool index);

		/**
		 *	Transform target bounding-box by corresponding matrix.
		 */
		static void transformAABB(const matrix4 &mat, const aabb &target, aabb &dest);

		/**
		 *	Calculate normal data for corresponding geometrydata.
		 */
		static void calculateNormal(geometrydata &data);

		/**
		 *	Calculate tangent data for corresponding geometrydata.
		 */
		static void calculateTangent(geometrydata &data);

		/**
		 *	Free memories allocated by target geometrydata.
		 *	Call this when you're about to delete target geometrydata.
		 */
		static void emptyGeometryData(geometrydata &data);
	};
}

#endif