#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector4.h"

namespace nest
{
	class quaternion
	{
	public:
		
		float x, y, z, w;
		
		quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		
		quaternion(const quaternion &a) : x(a.x), y(a.y), z(a.z), w(a.w) {}
		
		quaternion(float nx, float ny, float nz, float nw) : x(nx), y(ny), z(nz), w(nw) {}
		
		void identity() 
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		
		void rotate(const vector4 &axis, float theta);
		
		void normalize();
		
		vector4 getRotation();
		
		quaternion operator * (const quaternion &a) const;
		
		quaternion &operator *= (const quaternion &a);
	};

	float dotProduct(const quaternion &a, const quaternion &b);

	quaternion slerp(const quaternion &a, const quaternion &b, float t);

	quaternion conjugate(const quaternion &a);

	quaternion pow(const quaternion &a, float exp);
}

#endif