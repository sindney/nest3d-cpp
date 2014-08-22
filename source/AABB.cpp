#include "AABB.h"

namespace nest
{
	void AABB::create(AABB &bound, const float *vertexData, const int vertexDataSize)
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
}