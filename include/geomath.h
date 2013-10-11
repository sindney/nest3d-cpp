#ifndef N3D_GEOMATH_H
#define N3D_GEOMATH_H

#include <vector>

#include "geometry.h"
#include "vector4.h"

namespace nest
{
	using namespace std;

	typedef struct
	{
		float t, u, v;

		unsigned int index;

	} rayGEOMResult;

	class geomath
	{
	public:

		static bool BSBS(vector4 &center0, float radius0, vector4 &center1, float radius1);

		static bool AABBBS(vector4 &max, vector4 &min, vector4 &center, float radius);

		static bool AABBAABB(vector4 &max0, vector4 &min0, vector4 &max1, vector4 &min1);

		static void createPlane(vector4 &p, const vector4 &v1, const vector4 &v2, const vector4 &v3);

		static bool rayBS(vector4 &result, vector4 &orgion, vector4 &delta, vector4 &center, float radius);

		static bool rayAABB(vector4 &result, vector4 &orgion, vector4 &delta, vector4 &max, vector4 &min);

		static bool rayTRI(float* t, float* u, float* v, vector4 &orgion, vector4 &delta, vector4 &p0, vector4 &p1, vector4 &p2);

		/**
		 *	You need to transform your ray to geometry space(same as mesh local space).
		 */
		static bool rayGEOM(vector<rayGEOMResult> *results, rayGEOMResult *result, bool uv, vector4 &orgion, vector4 &delta, geometry &geom);
	};
}

#endif