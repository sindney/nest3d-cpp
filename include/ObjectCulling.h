#ifndef N3D_OBJECTCULLING_H
#define N3D_OBJECTCULLING_H

#include "AABB.h"

namespace nest
{
	class ObjectCulling
	{
	public:

		Vector4 planes[6];

		/**
		 *	When your projection matrix is perspective, use this.
		 */
		void create(float fov, float ratio, float near, float far);

		/**
		 *	When your projection matrix is orthographic, use this.
		 */
		void create(float left, float right, float bottom, float top, float near, float far);

		/**
		 *	Test if taget point is in frustum.
		 *	
		 *	@param p Point in view space.
		 */
		bool classifyPoint(const Vector4 &p);

		/**
		 *	Test if taget bounding sphere is in frustum.
		 *	
		 *	@param center Bounding sphere's center in view space.
		 *	@param radius Bounding sphere's radius in view space.
		 */
		bool classifyBSphere(const Vector4 &center, float radius);

		/**
		 *	Test if taget axis aligned bounding box is in frustum.
		 *	
		 *	@param bound Axis aligned bounding box in view space.
		 */
		bool classifyAABB(const AABB &bound);

	};
}

#endif