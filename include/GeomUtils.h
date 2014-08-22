#ifndef N3D_GEOMUTILS_H
#define N3D_GEOMUTILS_H

#include <vector>

#include "Geometry.h"

namespace nest
{
	/**
	 *	Ray & Geometry intersection test result.
	 *	<p>Result = Orgion + t * Delta</p>
	 */
	typedef struct
	{
		float t, u, v;

		unsigned int index;

	} RayGeomResult;

	class GeomUtils
	{
	public:

		static bool BSBS(Vector4 &center0, float radius0, Vector4 &center1, float radius1);

		static bool AABBBS(Vector4 &max, Vector4 &min, Vector4 &center, float radius);

		static bool AABBAABB(Vector4 &max0, Vector4 &min0, Vector4 &max1, Vector4 &min1);

		static bool rayBS(Vector4 &result, Vector4 &orgion, Vector4 &delta, Vector4 &center, float radius);

		static bool rayAABB(Vector4 &result, Vector4 &orgion, Vector4 &delta, Vector4 &max, Vector4 &min);

		static bool rayTri(float* t, float* u, float* v, Vector4 &orgion, Vector4 &delta, Vector4 &p0, Vector4 &p1, Vector4 &p2);

		/**
		 *	You need to transform your ray to Geometry space(aka mesh local space) first.
		 */
		static bool rayGeom(std::vector<RayGeomResult> *results, RayGeomResult *result, bool uv, Vector4 &orgion, Vector4 &delta, Geometry &geom);
	};
}

#endif