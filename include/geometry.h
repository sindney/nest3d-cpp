#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>

#include "matrix4.h"
#include "vector4.h"

namespace nest
{
	typedef struct 
	{
		vector4 max, min;
	} aabb;

	enum geomparams
	{
		GEOM_VERTEX 	= 1 << 0, 
		GEOM_UV			= 1 << 2, 
		GEOM_NORMAL		= 1 << 3, 
		GEOM_TANGENT	= 1 << 4
	};

	class geometry
	{
	public:

		std::string name;

		aabb bound;

		GLfloat *vertexData, *uvData, *normalData, *tangentData;

		GLuint *indexData;

		GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer;

		GLuint indexBuffer;

		GLuint numVertices, numTriangles;

		GLuint attributeArray;

		/**
		 *	When you change the lenght of vertexData.
		 *	You should make sure that it's uv/normal/tangent are in the right size or just delete and NULL them.
		 */
		geometry();

		~geometry();

		/**
		 *	Calculate target vertex array's bounding-box.
		 */
		static void setupAABB(aabb &bound, const float *vertexData, const int vertexDataSize);

		/**
		 *	Transform target bounding-box by corresponding matrix.
		 */
		static void transformAABB(const matrix4 &mat, const aabb &target, aabb &dest);

		/**
		 *	Calculate normal data for corresponding geometrydata.
		 */
		static void calculateNormal(geometry &geom);

		/**
		 *	Calculate tangent data for corresponding geometrydata.
		 */
		static void calculateTangent(geometry &geom);

		/**
		 *	Setup target geometry object by corresponding geometrydata for opengl rendering pipeline.
		 */
		static void configure(geometry &geom, int params);
	};
}

#endif