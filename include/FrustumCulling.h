#ifndef N3D_FRUSTUMCULLING_H
#define N3D_FRUSTUMCULLING_H

#include "AABB.h"
#include "ObjectCulling.h"

namespace nest
{
	class FrustumCulling : public ObjectCulling
	{
	public:

		Vector4 planes[6];

		FrustumCulling(float fov, float ratio, float near, float far);

		void create(float fov, float ratio, float near, float far);

		bool classifyPoint(const Vector4 &p);

		bool classifyBSphere(const Vector4 &center, float radius);

		bool classifyAABB(const AABB &bound);

	};
}

#endif