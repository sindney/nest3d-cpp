#ifndef N3D_FRUSTUM_H
#define N3D_FRUSTUM_H

#include "aabb.h"
#include "matrix4.h"
#include "vector4.h"

namespace nest
{
	class frustum
	{
	public:

		vector4 planes[6];

		void create(float fov, float ratio, float near, float far);

		/**
		 *	Transform p(world space) to view space first.
		 */
		bool classifyPoint(const vector4 &p);

		/**
		 *	Transform center(world space) to view space first.
		 */
		bool classifyBSphere(const vector4 &center, float radius);

		/**
		 *	Make sure your bounding-box is in world space, and pass the right invertViewMatrix.
		 */
		bool classifyAABB(const aabb &bound, const matrix4 &ivm);

	};

}

#endif