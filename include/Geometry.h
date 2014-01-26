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

		GLfloat *vertexData, *uvData, *normalData, *tangentData, *indicesData, *weightsData;

		GLuint *indexData;

		GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, indicesBuffer, weightsBuffer;

		GLuint indexBuffer;

		GLuint numVts, numTris, jointPerVertex;

		GLuint attributeArray;

		/**
		 *	When you change the lenght of vertexData.
		 *	<p>You should make sure that it's uv/normal/tangent/indices/weights are in the right size or just delete and NULL them.</p>
		 *	@param flag Indicates whether vertexData array like vertexData, uvData etc should be delocated in geometries delocation.
		 */
		Geometry(bool flag = true);

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
		 *	Configure vertex data buffers.
		 *	@param software Skin rendering technique flag.
		 */
		static void configure(Geometry &geom, bool software = true);

	private:

		bool flag;
	};
}

#endif