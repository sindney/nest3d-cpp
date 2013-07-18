#ifndef VECTOR4_H
#define VECTOR4_H

#include <cmath>

namespace nest
{
	class vector4 
	{
	public:

		float x, y, z, w;
	
		vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		
		vector4(const vector4 &a) : x(a.x), y(a.y), z(a.z), w(a.w) {}
		
		vector4(float lx, float ly, float lz, float lw) : x(lx), y(ly), z(lz), w(lw) {}
		
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
			float mag = x * x + y * y + z * z;
			if(mag > 0.0f)
			{
				float a = 1.0f / sqrt(mag);
				x *= a; y *= a; z *= a;
			}
			w = 1.0f;
		}
		
		vector4 &operator = (const vector4 &a) 
		{
			x = a.x; y = a.y; z = a.z; w = a.w;
			return *this;
		}
		
		bool operator == (const vector4 &a) const 
		{
			return x == a.x && y == a.y && z == a.z && w == a.w;
		}
		
		bool operator != (const vector4 &a) const 
		{
			return x != a.x || y != a.y || z != a.z || w != a.w;
		}
		
		vector4 operator - () const 
		{
			return vector4(-x, -y, -z, 1.0f);
		}
		
		vector4 operator + (const vector4 &a) const 
		{
			return vector4(x + a.x, y + a.y, z + a.z, 1.0f);
		}
		
		vector4 operator - (const vector4 &a) const 
		{
			return vector4(x - a.x, y - a.y, z - a.z, 1.0f);
		}
		
		vector4 operator * (float a) const 
		{
			return vector4(x * a, y * a, z * a, 1.0f);
		}
		
		vector4 operator / (float a) const 
		{
			float b = 1.0f / a;
			return vector4(x * b, y * b, z * b, 1.0f);
		}
		
		vector4 &operator += (const vector4 &a) 
		{
			x += a.x; y += a.y; z += a.z;
			return *this;
		}
		
		vector4 &operator -= (const vector4 &a)
		{
			x -= a.x; y -= a.y; z -= a.z;
			return *this;
		}
		
		vector4 &operator *= (float a)
		{
			x *= a; y *= a; z *= a;
			return *this;
		}
		
		vector4 &operator /= (float a)
		{
			a = 1.0f / a;
			x *= a; y *= a; z *= a;
			return *this;
		}
		
		float operator * (const vector4 &a)
		{
			return x * a.x + y * a.y + z * a.z;
		}
	};

	inline vector4 operator * (float a, const vector4 &b)
	{
		return vector4(a * b.x, a * b.y, a * b.z, 1.0f);
	}

	inline vector4 crossProduct(const vector4 &a, const vector4 &b)
	{
		return vector4(
			a.y * b.z - a.z * b.y, 
			a.z * b.x - a.x * b.z, 
			a.x * b.y - a.y * b.x, 
			1.0f
		);
	}

	inline float distance(const vector4 &a, const vector4 &b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;
		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	inline float magnet(const vector4 &a)
	{
		return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	const vector4 X_AXIS = vector4(1.0f, 0.0f, 0.0f, 1.0f);
	const vector4 Y_AXIS = vector4(0.0f, 1.0f, 0.0f, 1.0f);
	const vector4 Z_AXIS = vector4(0.0f, 0.0f, 1.0f, 1.0f);
	const vector4 ZERO;
}

#endif