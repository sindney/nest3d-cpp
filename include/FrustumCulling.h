#ifndef N3D_FRUSTUMCULLING_H
#define N3D_FRUSTUMCULLING_H

#include "ObjectCulling.h"

namespace nest
{
	/**
	 *	Frustum culling approach, for camera3d class.
	 */
	class FrustumCulling : public ObjectCulling
	{
	public:

		Vector4 planes[6];

		/**
		 *	@param fov Field of view.
		 *	@param ratio Aspect ratio.
		 *	@param near Near cliping plane.
		 *	@param far Far cliping plane.
		 */
		FrustumCulling(float fov, float ratio, float near, float far);

		/**
		 *	@param fov Field of view.
		 *	@param ratio Aspect ratio.
		 *	@param near Near cliping plane.
		 *	@param far Far cliping plane.
		 */
		void create(float fov, float ratio, float near, float far);

		/**
		 *	Test if taget point is in frustum.
		 *	
		 *	@param p Point in view space.
		 */
		virtual bool classifyPoint(const Vector4 &p);

		/**
		 *	Test if taget bounding sphere is in frustum.
		 *	
		 *	@param center Bounding sphere's center in view space.
		 *	@param radius Bounding sphere's radius in view space.
		 */
		virtual bool classifyBSphere(const Vector4 &center, float radius);

		/**
		 *	Test if taget axis aligned bounding box is in frustum.
		 *	
		 *	@param bound Axis aligned bounding box in view space.
		 */
		virtual bool classifyAABB(const AABB &bound);

	};
}

#endif