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
		GLfloat cosom = dotProduct(a, b);
		Quaternion end = b;
		if (cosom < 0.0f) {
			cosom = -cosom;
			end.x = -end.x;
			end.y = -end.y;
			end.z = -end.z;
			end.w = -end.w;
		}
		
		GLfloat k0, k1;
		if(cosom > 0.9999f)
		{
			k0 = 1.0f - t;
			k1 = t;
		}
		else
		{
			GLfloat omega = acos(cosom);
			GLfloat sinom = sin(omega);
			k0 = sin((1.0f - t) * omega) / sinom;
			k1 = sin(t * omega) / sinom;
		}
		
		end.x = k0 * a.x + k1 * end.x;
		end.y = k0 * a.y + k1 * end.y;
		end.z = k0 * a.z + k1 * end.z;
		end.w = k0 * a.w + k1 * end.w;
		
		return end;
	}

	Quaternion Quaternion::conjugate(const Quaternion &a)
	{
		Quaternion result;
		result.w = a.w;
		result.x = -a.x;
		result.y = -a.y;
		result.z = -a.z;

		return result;
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

	void Quaternion::rotate(const Vector4 &angles)
	{
		GLfloat c1 = cos(angles.x / 2);
		GLfloat s1 = sin(angles.x / 2);
		GLfloat c2 = cos(angles.y / 2);
		GLfloat s2 = sin(angles.y / 2);
		GLfloat c3 = cos(angles.z / 2);
		GLfloat s3 = sin(angles.z / 2);
		w = c1 * c2 * c3 - s1 * s2 * s3;
		x = s1 * s2 * c3 + c1 * c2 * s3;
		y = s1 * c2 * c3 + c1 * s2 * s3;
		z = c1 * s2 * c3 - s1 * c2 * s3;
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

	void Quaternion::getRotation(Vector4 &axis, GLfloat &theta)
	{
		theta = acos(w) * 2.0f;
		
		GLfloat a = 1.0f - w * w;
		if(a <= 0.0f)
		{
			axis.x = 1.0f;
			axis.y = axis.z = 0.0f;
		}
		else
		{
			GLfloat b = 1.0f / sqrt(a);
			axis.x = x * b;
			axis.y = y * b;
			axis.z = z * b;
		}
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