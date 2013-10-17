#ifndef N3D_OBJECTCULLING_H
#define N3D_OBJECTCULLING_H

#include "AABB.h"

namespace nest
{
	class ObjectCulling
	{
	public:

		/**
		 *	Point in view space.
		 */
		virtual bool classifyPoint(const Vector4 &p) = 0;

		/**
		 *	BSphere in view space.
		 */
		virtual bool classifyBSphere(const Vector4 &center, float radius) = 0;

		/**
		 *	AABB in view space.
		 */
		virtual bool classifyAABB(const AABB &bound) = 0;

	};

}

#endif