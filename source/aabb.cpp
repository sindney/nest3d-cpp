#include "aabb.h"

namespace nest
{
	void aabb::configure(aabb &bound, const float *vertexData, const int vertexDataSize)
	{
		bound.max.x = bound.min.x = vertexData[0];
		bound.max.y = bound.min.y = vertexData[1];
		bound.max.z = bound.min.z = vertexData[2];
		
		float x, y, z;
		int i;

		for(i = 0; i < vertexDataSize; i += 3)
		{
			x = vertexData[i];
			y = vertexData[i + 1];
			z = vertexData[i + 2];
			if(x > bound.max.x) 
				bound.max.x = x;
			else if(x < bound.min.x)
				bound.min.x = x;
			if(y > bound.max.y) 
				bound.max.y = y;
			else if(y < bound.min.y)
				bound.min.y = y;
			if(z > bound.max.z) 
				bound.max.z = z;
			else if(z < bound.min.z)
				bound.min.z = z;
		}
	}

	void aabb::transform(const matrix4 &mat, const aabb &target, aabb &dest)
	{
		vector4 vertices[] = {
			mat * target.min, 
			mat * vector4(target.max.x, target.min.y, target.min.z, 1.0f), 
			mat * vector4(target.min.x, target.max.y, target.min.z, 1.0f), 
			mat * vector4(target.max.x, target.max.y, target.min.z, 1.0f), 
			mat * vector4(target.min.x, target.min.y, target.max.z, 1.0f), 
			mat * vector4(target.max.x, target.min.y, target.max.z, 1.0f), 
			mat * vector4(target.min.x, target.max.y, target.max.z, 1.0f), 
			mat * target.max
		};

		dest.max = dest.min = vertices[0];

		int i;
		vector4 *vt;
		for(i = 0; i < 8; i++)
		{
			vt = &vertices[i];
			if(vt->x > dest.max.x) 
				dest.max.x = vt->x;
			else if(vt->x < dest.min.x)
				dest.min.x = vt->x;
			if(vt->y > dest.max.y) 
				dest.max.y = vt->y;
			else if(vt->y < dest.min.y)
				dest.min.y = vt->y;
			if(vt->z > dest.max.z) 
				dest.max.z = vt->z;
			else if(vt->z < dest.min.z)
				dest.min.z = vt->z;
		}
	}
}