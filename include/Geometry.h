#ifndef N3D_GEOMETRY_H
#define N3D_GEOMETRY_H

#include <string>

#include "AABB.h"

namespace nest
{
	enum GeomParams
	{
		GEOM_VERTEX		= 1 << 0, 
		GEOM_UV			= 1 << 2, 
		GEOM_NORMAL		= 1 << 3, 
		GEOM_TANGENT	= 1 << 4, 
		GEOM_INDICES	= 1 << 5, 
		GEOM_WEIGHTS	= 1 << 6
	};
	
	class Geometry
	{
	protected:

		GLuint vboOffset;

	public:

		std::string name;

		AABB bound;

		GLfloat *vertexData, *uvData, *normalData, *tangentData, *weightsData;

		GLuint *indicesData, *indexData;

		GLuint vertexBuffer, uvBuffer, normalBuffer, tangentBuffer, indicesBuffer, weightsBuffer;

		GLuint indexBuffer;

		GLuint numVts, numTris;

		/**
		 *	0 - 4
		 */
		GLuint jointPerVertex;

		Geometry();

		virtual ~Geometry();

		/**
		 *	Call createVBOs(params) first, then use this.
		 *	
		 *	@param params Flags to id which data buffer should be attached to VAO.
		 *	
		 *	@see GeomParams
		 */
		void bindVBOtoVAO(GLuint vao, int params);

		/**
		 *	Generate VBOs.
		 * 	
		 *	@param params Flags to id which data buffer should be generated.
		 *	
		 *	@see GeomParams
		 */
		void createVBOs(int params);

		GLuint getVboOffset()
		{
			return vboOffset;
		}
	};
}

#endif