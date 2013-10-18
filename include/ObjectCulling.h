#ifndef N3D_OBJECTCULLING_H
#define N3D_OBJECTCULLING_H

#include "AABB.h"

namespace nest
{
	class ObjectCulling
	{
	public:

		/**
		 *	@param p Point in view space.
		 */
		virtual bool classifyPoint(const Vector4 &p) = 0;

		/**
		 *	@param center Bounding sphere's center in view space.
		 *	@param radius Bounding sphere's radius in view space.
		 */
		virtual bool classifyBSphere(const Vector4 &center, float radius) = 0;

		/**
		 *	@param bound Axis aligned bounding box in view space.
		 */
		virtual bool classifyAABB(const AABB &bound) = 0;

	};

}

#endif