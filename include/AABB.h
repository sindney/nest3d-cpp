#ifndef N3D_AABB_H
#define N3D_AABB_H

#include "Vector4.h"

namespace nest
{
	class AABB
	{
	public:

		Vector4 max, min;

		AABB &operator = (const AABB &a) 
		{
			max = a.max;
			min = a.min;
			return *this;
		}

		/**
		 *	Calculate target vertex array's bounding-box.
		 *	
		 *	@param bound Result bounding-box.
		 *	@param vertexData Vertex array pointer.
		 *	@param vertexDataSize Vertex array size.
		 */
		static void configure(AABB &bound, const float *vertexData, const int vertexDataSize);

	};
}

#endif