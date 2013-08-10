#ifndef N3D_GEOMATH_H
#define N3D_GEOMATH_H

#include "vector4.h"

namespace nest
{
	class geomath
	{
	public:

		static bool collisionBSBS(vector4 &center0, float radius0, vector4 &center1, float radius1);

		static bool collisionAABBBS(vector4 &max, vector4 &min, vector4 &center, float radius);

		static bool collisionAABBAABB(vector4 &max0, vector4 &min0, vector4 &max1, vector4 &min1);

		static void createPlane(vector4 &p, const vector4 &v1, const vector4 &v2, const vector4 &v3);
	};
}

#endif