#ifndef N3D_MATRIX4_H
#define N3D_MATRIX4_H

#include "vector4.h"
#include "quaternion.h"

namespace nest
{
	class matrix4 
	{
	public:

		static void project(matrix4 &m, const vector4 &v);
		
		static void reflect(matrix4 &m, const vector4 &v);

		static void perspectiveProjection(matrix4 &a, float fov, float aspect, float near, float far);

		static void orthographicProjection(matrix4 &a, float r, float t, float n, float f);
		
		GLfloat raw[16];

		matrix4()
		{
			identity();
		}

		matrix4(const float a[])
		{
			int i;
			for(i = 0; i < 16; i++) raw[i] = a[i];
		}

		matrix4(const matrix4 &a) 
		{
			int i;
			for(i = 0; i < 16; i++) raw[i] = a.raw[i];
		}

		void identity();
		
		void translate(const vector4 &a);
		
		void rotate(const vector4 &axis, float theta);
		
		void rotate(const vector4 &angles);

		void rotate(const quaternion &a);
		
		void scale(const vector4 &a);
		
		matrix4 inverse();

		matrix4 clone();
	};

	vector4 operator * (const vector4 &v, const matrix4 &m);

	vector4 &operator *= (vector4 &v, const matrix4 &m);

	matrix4 operator * (const matrix4 &a, const matrix4 &b);

	matrix4 &operator *= (matrix4 &a, const matrix4 &b);
}

#endif