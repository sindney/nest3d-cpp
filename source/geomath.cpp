#include <stdexcept>

#include "geomath.h"
namespace nest
{
	bool geomath::BSBS(vector4 &center0, float radius0, vector4 &center1, float radius1)
	{
		float x = center0.x - center1.x;
		float y = center0.y - center1.y;
		float z = center0.z - center1.z;
		return (x * x + y * y + z * z) <= (radius0 + radius1) * (radius0 + radius1);
	}

	bool geomath::AABBBS(vector4 &max, vector4 &min, vector4 &center, float radius)
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

	bool geomath::AABBAABB(vector4 &max0, vector4 &min0, vector4 &max1, vector4 &min1)
	{
		if(min0.x > max1.x) return false;
		if(max0.x < min1.x) return false;
		if(min0.y > max1.y) return false;
		if(max0.y < min1.y) return false;
		if(min0.z > max1.z) return false;
		if(max0.z < min1.z) return false;
		return true;
	}

	void geomath::createPlane(vector4 &p, const vector4 &v1, const vector4 &v2, const vector4 &v3)
	{
		p = crossProduct(v2 - v1, v3 - v1);
		p.normalize();
		p.w = - (p.x * v1.x + p.y * v1.y + p.z * v1.z);
	}

	bool geomath::rayBS(vector4 &result, vector4 &orgion, vector4 &delta, vector4 &center, float radius)
	{
		vector4 e = center - orgion;
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

	bool geomath::rayAABB(vector4 &result, vector4 &orgion, vector4 &delta, vector4 &max, vector4 &min)
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

	bool geomath::rayTRI(float* t, float* u, float* v, vector4 &orgion, vector4 &delta, vector4 &p0, vector4 &p1, vector4 &p2)
	{
		vector4 e1 = p1 - p0;
		vector4 e2 = p2 - p0;
		vector4 p = crossProduct(delta, e2);
		float det = e1 * p;
		if(det < 0.0001f) return false;
		vector4 t0 = orgion - p0;
		vector4 q = crossProduct(t0, e1);
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

	bool geomath::rayGEOM(vector<rayGEOMResult> *results, rayGEOMResult *result, bool uv, vector4 &orgion, vector4 &delta, geometry &geom)
	{
		if(geom.vertexData == NULL || geom.indexData == NULL) 
			throw runtime_error("Error ray geometry intersection test: Geometry's vertexData || indexData not found.");
		vector4 vt1;
		if(!rayAABB(vt1, orgion, delta, geom.bound.max, geom.bound.min)) return false;
		bool flag = result != NULL;
		rayGEOMResult current;
		vector4 p0, p1, p2;
		float t, u, v;
		int i, j, k;
		for(i = 0; i < geom.numTriangles; i++)
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
			if(rayTRI(&t, uv ? &u : NULL, uv ? &v : NULL, orgion, delta, p0, p1, p2))
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