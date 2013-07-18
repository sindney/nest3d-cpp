#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "geometry.h"
#include "matrix4.h"
#include "vector4.h"

namespace nest
{
	class frustum
	{
	public:

		vector4 planes[6];

		void create(float angle, float ratio, float near, float far);

		/**
		 *	Transform p(world space) to view space first.
		 */
		bool classifyPoint(const vector4 &p);

		/**
		 *	Transform center(world space) to view space first.
		 */
		bool classifyBSphere(const vector4 &center, float radius);

		/**
		 *	Make sure your bound is in world space, and pass the right invertViewMatrix.
		 */
		bool classifyAABB(const aabb &bound, const matrix4 &ivm);

		static void createPlane(vector4 &p, const vector4 &v1, const vector4 &v2, const vector4 &v3);

	};

}

#endif