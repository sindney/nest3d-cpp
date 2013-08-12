#ifndef N3D_GEOMETRY_H
#define N3D_GEOMETRY_H

#include <string>

#include "aabb.h"

namespace nest
{
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