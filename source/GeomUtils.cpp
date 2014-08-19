#include <stdexcept>

#include "GeomUtils.h"

namespace nest
{
	using namespace std;
	
	bool GeomUtils::BSBS(Vector4 &center0, float radius0, Vector4 &center1, float radius1)
	{
		float x = center0.x - center1.x;
		float y = center0.y - center1.y;
		float z = center0.z - center1.z;
		return (x * x + y * y + z * z) <= (radius0 + radius1) * (radius0 + radius1);
	}

	bool GeomUtils::AABBBS(Vector4 &max, Vector4 &min, Vector4 &center, float radius)
	{
		float x = center.x;
		float y = center.y;
		float z = center.z;

		if(x < min.x)
			x = min.x;
		else if(x > max.x)
			x = max.x;
		if(y < min.y)
			y = min.y;
		else if(y > max.y)
			y = max.y;
		if(z < min.z)
			z = min.z;
		else if(z > max.z)
			z = max.z;

		x -= center.x;
		y -= center.y;
		z -= center.z;

		return (x * x + y * y + z * z) <= (radius * radius);
	}

	bool GeomUtils::AABBAABB(Vector4 &max0, Vector4 &min0, Vector4 &max1, Vector4 &min1)
	{
		if(min0.x > max1.x) return false;
		if(max0.x < min1.x) return false;
		if(min0.y > max1.y) return false;
		if(max0.y < min1.y) return false;
		if(min0.z > max1.z) return false;
		if(max0.z < min1.z) return false;
		return true;
	}

	void GeomUtils::createPlane(Vector4 &p, const Vector4 &v1, const Vector4 &v2, const Vector4 &v3)
	{
		p = Vector4::crossProduct(v2 - v1, v3 - v1);
		p.normalize();
		p.w = - (p.x * v1.x + p.y * v1.y + p.z * v1.z);
	}

	bool GeomUtils::rayBS(Vector4 &result, Vector4 &orgion, Vector4 &delta, Vector4 &center, float radius)
	{
		Vector4 e = center - orgion;
		float lens = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
		float len = sqrt(lens);
		float a = e * delta / len;
		float f = radius * radius - lens + a * a;
		if(f < 0)
			return false;
		f = a - sqrt(f);
		if(f > len || f < 0)
			return false;
		result = delta * f + orgion;
		return true;
	}

	bool GeomUtils::rayAABB(Vector4 &result, Vector4 &orgion, Vector4 &delta, Vector4 &max, Vector4 &min)
	{
		bool inside = true;
		float xt, xn;
		if(orgion.x < min.x)
		{
			xt = min.x - orgion.x;
			if(xt > delta.x) return false;
			xt /= delta.x;
			inside = false;
			xn = -1;
		}
		else if(orgion.x > max.x)
		{
			xt = max.x - orgion.x;
			if(xt < delta.x) return false;
			xt /= delta.x;
			inside = false;
			xn = 1;
		}
		else 
		{
			xt = -1;
		}
		float yt, yn;
		if(orgion.y < min.y)
		{
			yt = min.y - orgion.y;
			if(yt > delta.y) return false;
			yt /= delta.y;
			inside = false;
			yn = -1;
		}
		else if(orgion.y > max.y)
		{
			yt = max.y - orgion.y;
			if(yt < delta.y) return false;
			yt /= delta.y;
			inside = false;
			yn = 1;
		}
		else 
		{
			yt = -1;
		}
		float zt, zn;
		if(orgion.z < min.z)
		{
			zt = min.z - orgion.z;
			if(zt > delta.z) return false;
			zt /= delta.z;
			inside = false;
			zn = -1;
		}
		else if(orgion.z > max.z)
		{
			zt = max.z - orgion.z;
			if(zt < delta.z) return false;
			zt /= delta.z;
			inside = false;
			zn = 1;
		}
		else 
		{
			zt = -1;
		}
		if(inside) return false;
		int which = 0;
		float t = xt;
		if(yt > t)
		{
			which = 1;
			t = yt;
		}
		if(zt > t)
		{
			which = 2;
			t = zt;
		}
		float x, y, z;
		switch(which)
		{
			case 0:
				// yz
				y = orgion.y + delta.y * t;
				if (y < min.y || y > max.y) return false;
				z = orgion.z + delta.z * t;
				if (z < min.z || z > max.z) return false;
				break;
			case 1:
				// xz
				x = orgion.x + delta.x * t;
				if (x < min.x || x > max.x) return false;
				z = orgion.z + delta.z * t;
				if (z < min.z || z > max.z) return false;
				break;
			case 2:
				// xy
				x = orgion.x + delta.x * t;
				if (x < min.x || x > max.x) return false;
				y = orgion.y + delta.y * t;
				if (y < min.y || y > max.y) return false;
				break;
		}
		result = delta * t + orgion;
		return true;
	}

	bool GeomUtils::rayTri(float* t, float* u, float* v, Vector4 &orgion, Vector4 &delta, Vector4 &p0, Vector4 &p1, Vector4 &p2)
	{
		Vector4 e1 = p1 - p0;
		Vector4 e2 = p2 - p0;
		Vector4 p = Vector4::crossProduct(delta, e2);
		float det = e1 * p;
		if(det < 0.0001f) return false;
		Vector4 t0 = orgion - p0;
		Vector4 q = Vector4::crossProduct(t0, e1);
		float ivt = 1.0f / det;
		*t = e2 * q;
		*t *= ivt;
		if(u != NULL)
		{
			*u = t0 * p;
			if(*u < 0.0f || *u > det) return false;
			*v = delta * q;
			if(*v < 0.0f || *u + *v > det) return false;
			*u *= ivt;
			*v *= ivt;
		}
		return true;
	}

	bool GeomUtils::rayGeom(vector<RayGeomResult> *results, RayGeomResult *result, bool uv, Vector4 &orgion, Vector4 &delta, Geometry &geom)
	{
		if(geom.vertexData == NULL || geom.indexData == NULL) 
			throw runtime_error("Error Ray-Geometry intersection test: Geometry's vertexData || indexData not found.");
		Vector4 vt1;
		if(!rayAABB(vt1, orgion, delta, geom.bound.max, geom.bound.min)) return false;
		bool flag = result != NULL;
		RayGeomResult current;
		Vector4 p0, p1, p2;
		float t, u, v;
		int i, j, k;
		for(i = 0; i < geom.numTris; i++)
		{
			j = i * 3;
			k = geom.indexData[j] * 3;
			p0.x = geom.vertexData[k];
			p0.y = geom.vertexData[k + 1];
			p0.z = geom.vertexData[k + 2];
			k = geom.indexData[j + 1] * 3;
			p1.x = geom.vertexData[k];
			p1.y = geom.vertexData[k + 1];
			p1.z = geom.vertexData[k + 2];
			k = geom.indexData[j + 2] * 3;
			p2.x = geom.vertexData[k];
			p2.y = geom.vertexData[k + 1];
			p2.z = geom.vertexData[k + 2];
			if(rayTri(&t, uv ? &u : NULL, uv ? &v : NULL, orgion, delta, p0, p1, p2))
			{
				current.t = t;
				current.index = geom.indexData[j];
				if(uv)
				{
					current.u = u;
					current.v = v;
				}
				if(flag) 
				{
					*result = current;
					return true;
				}
				else
					results->push_back(current);
			}
		}
		return flag ? false : results->size() > 0;
	}
}