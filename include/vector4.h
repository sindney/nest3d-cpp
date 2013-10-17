#ifndef N3D_VECTOR4_H
#define N3D_VECTOR4_H

#include <cmath>

#include "GL/glew.h"

namespace nest
{
	class Vector4 
	{
	public:

		GLfloat x, y, z, w;
	
		Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		
		Vector4(const Vector4 &a) : x(a.x), y(a.y), z(a.z), w(a.w) {}
		
		Vector4(GLfloat lx, GLfloat ly, GLfloat lz, GLfloat lw) : x(lx), y(ly), z(lz), w(lw) {}
		
		void absolute()
		{
			x = std::abs(x);
			y = std::abs(y);
			z = std::abs(z);
			w = 1.0f;
		}

		void zero()
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		
		void normalize()
		{
			GLfloat mag = x * x + y * y + z * z;
			if(mag > 0.0f)
			{
				GLfloat a = 1.0f / sqrt(mag);
				x *= a; y *= a; z *= a;
			}
			w = 1.0f;
		}
		
		Vector4 &operator = (const Vector4 &a) 
		{
			x = a.x; y = a.y; z = a.z; w = a.w;
			return *this;
		}
		
		bool operator == (const Vector4 &a) const 
		{
			return x == a.x && y == a.y && z == a.z && w == a.w;
		}
		
		bool operator != (const Vector4 &a) const 
		{
			return x != a.x || y != a.y || z != a.z || w != a.w;
		}
		
		Vector4 operator - () const 
		{
			return Vector4(-x, -y, -z, 1.0f);
		}
		
		Vector4 operator + (const Vector4 &a) const 
		{
			return Vector4(x + a.x, y + a.y, z + a.z, 1.0f);
		}
		
		Vector4 operator - (const Vector4 &a) const 
		{
			return Vector4(x - a.x, y - a.y, z - a.z, 1.0f);
		}
		
		Vector4 operator * (GLfloat a) const 
		{
			return Vector4(x * a, y * a, z * a, 1.0f);
		}
		
		Vector4 operator / (GLfloat a) const 
		{
			GLfloat b = 1.0f / a;
			return Vector4(x * b, y * b, z * b, 1.0f);
		}
		
		Vector4 &operator += (const Vector4 &a) 
		{
			x += a.x; y += a.y; z += a.z;
			return *this;
		}
		
		Vector4 &operator -= (const Vector4 &a)
		{
			x -= a.x; y -= a.y; z -= a.z;
			return *this;
		}
		
		Vector4 &operator *= (GLfloat a)
		{
			x *= a; y *= a; z *= a;
			return *this;
		}
		
		Vector4 &operator /= (GLfloat a)
		{
			a = 1.0f / a;
			x *= a; y *= a; z *= a;
			return *this;
		}
		
		GLfloat operator * (const Vector4 &a)
		{
			return x * a.x + y * a.y + z * a.z;
		}

		static const Vector4 X_AXIS, Y_AXIS, Z_AXIS, ZERO;
	};

	inline Vector4 operator * (GLfloat a, const Vector4 &b)
	{
		return Vector4(a * b.x, a * b.y, a * b.z, 1.0f);
	}

	inline Vector4 crossProduct(const Vector4 &a, const Vector4 &b)
	{
		return Vector4(
			a.y * b.z - a.z * b.y, 
			a.z * b.x - a.x * b.z, 
			a.x * b.y - a.y * b.x, 
			1.0f
		);
	}

	inline GLfloat distance(const Vector4 &a, const Vector4 &b)
	{
		GLfloat dx = a.x - b.x;
		GLfloat dy = a.y - b.y;
		GLfloat dz = a.z - b.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	inline GLfloat magnet(const Vector4 &a)
	{
		return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}
}

#endif