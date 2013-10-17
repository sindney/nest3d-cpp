#include <cmath>

#include "FrustumCulling.h"
#include "Geomath.h"

namespace nest
{
	FrustumCulling::FrustumCulling(float fov, float ratio, float near, float far)
	{
		create(fov, ratio, near, far);
	}

	void FrustumCulling::create(float fov, float ratio, float near, float far)
	{
		float r = tan(fov * 0.5);
		float nH = near * r;
		float nW = nH * ratio;
		float fH = far * r;
		float fW = fH * ratio;

		Vector4 zv(0, 0, 1, 1);
		Vector4 xv(1, 0, 0, 1);
		Vector4 yv(0, 1, 0, 1);

		Vector4 vn = - (zv * near);
		Vector4 vf = - (zv * far);

		Vector4 nTL = vn + (yv * nH) - (xv * nW);
		Vector4 nTR = vn + (yv * nH) + (xv * nW);
		Vector4 nBL = vn - (yv * nH) - (xv * nW);
		Vector4 nBR = vn - (yv * nH) + (xv * nW);

		Vector4 fTL = vf + (yv * fH) - (xv * fW);
		Vector4 fTR = vf + (yv * fH) + (xv * fW);
		Vector4 fBL = vf - (yv * fH) - (xv * fW);
		Vector4 fBR = vf - (yv * fH) + (xv * fW);

		// top
		Geomath::createPlane(planes[0], fTR, nTR, nTL);
		// bottom
		Geomath::createPlane(planes[1], nBL, nBR, fBR);
		// left
		Geomath::createPlane(planes[2], nTL, nBL, fBL);
		// right
		Geomath::createPlane(planes[3], fBR, nBR, nTR);
		// near
		Geomath::createPlane(planes[4], nTL, nTR, nBR);
		// far
		Geomath::createPlane(planes[5], fTR, fTL, fBL);
	}

	bool FrustumCulling::classifyPoint(const Vector4 &p)
	{
		int i;
		Vector4 *plane;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			if(plane->x * p.x + plane->y * p.y + plane->z * p.z + plane->w < 0) return false;
		}
		return true;
	}

	bool FrustumCulling::classifyBSphere(const Vector4 &center, float radius)
	{
		int i;
		float d;
		Vector4 *plane;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			d = plane->x * center.x + plane->y * center.y + plane->z * center.z + plane->w;
			if(d < 0 && radius < -d) return false;
		}
		return true;
	}

	bool FrustumCulling::classifyAABB(const AABB &bound)
	{
		Vector4 center = (bound.max + bound.min) / 2;
		Vector4 half = bound.max - center;
		half.absolute();

		int i;
		Vector4 *plane;
		Vector4 normal;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			normal = (*plane);
			normal.absolute();
			if(center * (*plane) + half * normal < -plane->w) return false;
		}

		return true;
	}
}