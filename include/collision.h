#ifndef N3D_COLLISION_H
#define N3D_COLLISION_H

#include "vector4.h"

namespace nest
{
	class collision
	{
	public:

		static bool sphere_sphere(vector4 &center0, float radius0, vector4 &center1, float radius1);

		static bool aabb_sphere(vector4 &max, vector4 &min, vector4 &center, float radius);

		static bool aabb_aabb(vector4 &max0, vector4 &min0, vector4 &max1, vector4 &min1);

	};
}

#endif