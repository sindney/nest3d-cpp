#include <cmath>

#include "frustum.h"
#include "geomath.h"

namespace nest
{
	void frustum::create(float angle, float ratio, float near, float far)
	{
		float r = tan(angle * 0.5);
		float nH = near * r;
		float nW = nH * ratio;
		float fH = far * r;
		float fW = fH * ratio;

		vector4 zv(0, 0, 1, 1);
		vector4 xv(1, 0, 0, 1);
		vector4 yv(0, 1, 0, 1);

		vector4 vn = - (zv * near);
		vector4 vf = - (zv * far);

		vector4 nTL = vn + (yv * nH) - (xv * nW);
		vector4 nTR = vn + (yv * nH) + (xv * nW);
		vector4 nBL = vn - (yv * nH) - (xv * nW);
		vector4 nBR = vn - (yv * nH) + (xv * nW);

		vector4 fTL = vf + (yv * fH) - (xv * fW);
		vector4 fTR = vf + (yv * fH) + (xv * fW);
		vector4 fBL = vf - (yv * fH) - (xv * fW);
		vector4 fBR = vf - (yv * fH) + (xv * fW);

		// top
		geomath::createPlane(planes[0], fTR, nTR, nTL);
		// bottom
		geomath::createPlane(planes[1], nBL, nBR, fBR);
		// left
		geomath::createPlane(planes[2], nTL, nBL, fBL);
		// right
		geomath::createPlane(planes[3], fBR, nBR, nTR);
		// near
		geomath::createPlane(planes[4], nTL, nTR, nBR);
		// far
		geomath::createPlane(planes[5], fTR, fTL, fBL);
	}

	bool frustum::classifyPoint(const vector4 &p)
	{
		int i;
		vector4 *plane;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			if(plane->x * p.x + plane->y * p.y + plane->z * p.z + plane->w < 0) return false;
		}
		return true;
	}

	bool frustum::classifyBSphere(const vector4 &center, float radius)
	{
		int i;
		float d;
		vector4 *plane;
		for(i = 0; i < 6; i++)
		{
			plane = &planes[i];
			d = plane->x * center.x + plane->y * center.y + plane->z * center.z + plane->w;
			if(d < 0 && radius < -d) return false;
		}
		return true;
	}

	bool frustum::classifyAABB(const aabb &bound, const matrix4 &ivm)
	{
		aabb target;
		aabb::transform(ivm, bound, target);

		vector4 center = (target.max + target.min) / 2;
		vector4 half = target.max - center;
		half.absolute();

		int i;
		vector4 *plane;
		vector4 normal;
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