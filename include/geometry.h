#ifndef N3D_GEOMETRY_H
#define N3D_GEOMETRY_H

#include <string>

#include "AABB.h"

namespace nest
{
	enum GeomParams
	{
		GEOM_VERTEX 	= 1 << 0, 
		GEOM_UV			= 1 << 2, 
		GEOM_NORMAL		= 1 << 3, 
		GEOM_TANGENT	= 1 << 4
	};

	class Geometry
	{
	public:

		std::string name;

		AABB bound;

		GLfloat *vertexData, *uvData, *normalData, *tangentData;

		GLuint *indexData;

		GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer;

		GLuint indexBuffer;

		GLuint numVts, numTris;

		GLuint attributeArray;

		/**
		 *	When you change the lenght of vertexData.
		 *	You should make sure that it's uv/normal/tangent are in the right size or just delete and NULL them.
		 */
		Geometry();

		~Geometry();

		/**
		 *	Calculate normal data for corresponding Geometrydata.
		 */
		static void calculateNormal(Geometry &geom);

		/**
		 *	Calculate tangent data for corresponding Geometrydata.
		 */
		static void calculateTangent(Geometry &geom);

		/**
		 *	Setup target Geometry object by corresponding Geometrydata for opengl rendering pipeline.
		 */
		static void configure(Geometry &geom, int params);
	};
}

#endif