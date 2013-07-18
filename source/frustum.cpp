#include <cmath>

#include "frustum.h"
#include "geometry.h"
#include "vector4.h"

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
		frustum::createPlane(planes[0], fTR, nTR, nTL);
		// bottom
		frustum::createPlane(planes[1], nBL, nBR, fBR);
		// left
		frustum::createPlane(planes[2], nTL, nBL, fBL);
		// right
		frustum::createPlane(planes[3], fBR, nBR, nTR);
		// near
		frustum::createPlane(planes[4], nTL, nTR, nBR);
		// far
		frustum::createPlane(planes[5], fTR, fTL, fBL);
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
		geometry::transformAABB(ivm, bound, target);

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

	void frustum::createPlane(vector4 &p, const vector4 &v1, const vector4 &v2, const vector4 &v3)
	{
		p = crossProduct(v2 - v1, v3 - v1);
		p.normalize();
		p.w = - (p.x * v1.x + p.y * v1.y + p.z * v1.z);
	}
}