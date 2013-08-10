#include "geomath.h"

namespace nest
{
	bool geomath::collisionBSBS(vector4 &center0, float radius0, vector4 &center1, float radius1)
	{
		float x = center0.x - center1.x;
		float y = center0.y - center1.y;
		float z = center0.z - center1.z;
		return (x * x + y * y + z * z) <= (radius0 + radius1) * (radius0 + radius1);
	}

	bool geomath::collisionAABBBS(vector4 &max, vector4 &min, vector4 &center, float radius)
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

	bool geomath::collisionAABBAABB(vector4 &max0, vector4 &min0, vector4 &max1, vector4 &min1)
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
}