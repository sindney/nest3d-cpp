#include <cmath>

#include "Quaternion.h"

namespace nest
{
	GLfloat Quaternion::dotProduct(const Quaternion &a, const Quaternion &b)
	{
		return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
	}

	Quaternion Quaternion::slerp(const Quaternion &a, const Quaternion &b, GLfloat t)
	{
		Quaternion result;
		
		GLfloat cos = dotProduct(a, b);
		if (cos == 0.0f) {
			result.x = a.x;
			result.y = a.y;
			result.z = a.z;
			result.w = a.w;
			return result;
		}
		
		GLfloat k0, k1;
		if (cos > .9999f) {
			k0 = 1.0f - t;
			k1 = t;
		} 
		else 
		{
			GLfloat a = sqrt(1.0f - cos * cos);
			GLfloat b = atan2(a, cos);
			GLfloat c = 1.0f / a;
			k0 = sin((1.0f - t) * b) * c;
			k1 = sin(t * b) * c;
		}
		
		result.w = k0 * a.w + k1 * b.w;
		result.x = k0 * a.x + k1 * b.x;
		result.y = k0 * a.y + k1 * b.y;
		result.z = k0 * a.z + k1 * b.z;
		
		return result;
	}

	Quaternion Quaternion::conjugate(const Quaternion &a)
	{
		Quaternion result;
		result.w = a.w;
		result.x = -a.x;
		result.y = -a.y;
		result.z = -a.z;
	}

	Quaternion Quaternion::pow(const Quaternion &a, GLfloat exp)
	{
		if(fabs(a.w) > .9999f) return a;
		
		GLfloat alpha = acos(a.w);
		GLfloat alpha2 = alpha * exp;
		GLfloat mul = sin(alpha2) / sin(alpha);
		
		Quaternion result;
		result.w = cos(alpha2);
		result.x = a.x * mul;
		result.y = a.y * mul;
		result.z = a.z * mul;
		
		return result;
	}
	
	void Quaternion::rotate(const Vector4 &axis, GLfloat theta)
	{
		GLfloat t2 = theta * .5f;
		GLfloat st2 = sin(t2);
		w = cos(t2);
		x = axis.x * st2;
		y = axis.y * st2;
		z = axis.z * st2;
	}

	void Quaternion::normalize()
	{
		GLfloat mag = (GLfloat)sqrt(x * x + y * y + z * z + w * w);
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

	Vector4 Quaternion::getRotation()
	{
		Vector4 result;
		result.w = acos(w) * 2.0f;
		
		GLfloat a = 1.0f - w * w;
		if(a <= 0.0f)
		{
			result.x = 1.0f;
			result.y = result.z = 0.0f;
			return result;
		}
		
		GLfloat b = 1.0f / sqrt(a);
		result.x = x * b;
		result.y = y * b;
		result.z = z * b;
		
		return result;
	}

	Quaternion Quaternion::operator * (const Quaternion &a) const 
	{
		Quaternion result;
		result.w = w * a.w - x * a.x - y * a.y - z * a.z;
		result.x = w * a.x + x * a.w + z * a.y - y * a.z;
		result.y = w * a.y + y * a.w + x * a.z - z * a.x;
		result.z = w * a.z + z * a.w + y * a.x - x * a.y;
		return result;
	}

	Quaternion &Quaternion::operator *= (const Quaternion &a)
	{
		*this = *this * a;
		return *this;
	}
}