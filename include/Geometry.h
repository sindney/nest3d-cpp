#ifndef N3D_GEOMETRY_H
#define N3D_GEOMETRY_H

#include <string>

#include "AABB.h"

namespace nest
{
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
		 *	<p>You should make sure that it's uv/normal/tangent are in the right size or just delete and NULL them.</p>
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
	};
}

#endif