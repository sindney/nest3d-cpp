#ifndef N3D_AABB_H
#define N3D_AABB_H

#include "matrix4.h"
#include "vector4.h"

namespace nest
{
	class aabb
	{
	public:

		vector4 max, min;

		/**
		 *	Calculate target vertex array's bounding-box.
		 */
		static void configure(aabb &bound, const float *vertexData, const int vertexDataSize);

		/**
		 *	Transform target bounding-box by corresponding matrix.
		 */
		static void transform(const matrix4 &mat, const aabb &target, aabb &dest);
	};
}

#endif