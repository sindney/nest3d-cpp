#include <cmath>

#include "quaternion.h"
#include "vector4.h"

namespace nest
{
	void quaternion::rotate(const vector4 &axis, float theta)
	{
		float t2 = theta * .5f;
		float st2 = sin(t2);
		w = cos(t2);
		x = axis.x * st2;
		y = axis.y * st2;
		z = axis.z * st2;
	}

	void quaternion::normalize()
	{
		float mag = (float)sqrt(x * x + y * y + z * z + w * w);
		if(mag > 0.0f)
		{
			mag = 1 / mag;
			w *= mag;
			x *= mag;
			y *= mag;
			z *= mag;
		} 
		else 
		{
			identity();
		}
	}

	vector4 quaternion::getRotation()
	{
		vector4 result;
		result.w = acos(w) * 2.0f;
		
		float a = 1.0f - w * w;
		if(a <= 0.0f)
		{
			result.x = 1.0f;
			result.y = result.z = 0.0f;
			return result;
		}
		
		float b = 1.0f / sqrt(a);
		result.x = x * b;
		result.y = y * b;
		result.z = z * b;
		
		return result;
	}

	quaternion quaternion::operator * (const quaternion &a) const 
	{
		quaternion result;
		result.w = w * a.w - x * a.x - y * a.y - z * a.z;
		result.x = w * a.x + x * a.w + z * a.y - y * a.z;
		result.y = w * a.y + y * a.w + x * a.z - z * a.x;
		result.z = w * a.z + z * a.w + y * a.x - x * a.y;
		return result;
	}

	quaternion &quaternion::operator *= (const quaternion &a)
	{
		*this = *this * a;
		return *this;
	}

	float dotProduct(const quaternion &a, const quaternion &b)
	{
		return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
	}

	quaternion slerp(const quaternion &a, const quaternion &b, float t)
	{
		quaternion result;
		
		float cos = dotProduct(a, b);
		if (cos == 0.0f) {
			result.x = a.x;
			result.y = a.y;
			result.z = a.z;
			result.w = a.w;
			return result;
		}
		
		float k0, k1;
		if (cos > .9999f) {
			k0 = 1.0f - t;
			k1 = t;
		} 
		else 
		{
			float a = sqrt(1.0f - cos * cos);
			float b = atan2(a, cos);
			float c = 1.0f / a;
			k0 = sin((1.0f - t) * b) * c;
			k1 = sin(t * b) * c;
		}
		
		result.w = k0 * a.w + k1 * b.w;
		result.x = k0 * a.x + k1 * b.x;
		result.y = k0 * a.y + k1 * b.y;
		result.z = k0 * a.z + k1 * b.z;
		
		return result;
	}

	quaternion conjugate(const quaternion &a)
	{
		quaternion result;
		result.w = a.w;
		result.x = -a.x;
		result.y = -a.y;
		result.z = -a.z;
	}

	quaternion pow(const quaternion &a, float exp)
	{
		if(fabs(a.w) > .9999f) return a;
		
		float alpha = acos(a.w);
		float alpha2 = alpha * exp;
		float mul = sin(alpha2) / sin(alpha);
		
		quaternion result;
		result.w = cos(alpha2);
		result.x = a.x * mul;
		result.y = a.y * mul;
		result.z = a.z * mul;
		
		return result;
	}
}