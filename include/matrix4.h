#ifndef N3D_MATRIX4_H
#define N3D_MATRIX4_H

#include "vector4.h"
#include "quaternion.h"

namespace nest
{
	/**
	 *	0	4	8	12
	 *	1	5	9	13
	 *	2	6	10	14
	 *	3	7	11	15
	 */
	class matrix4 
	{
	public:

		static void perspectiveFov(matrix4 &a, GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far);

		static void perspectiveOffCenter(matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

		static void orthoOffCenter(matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
		
		GLfloat raw[16];

		matrix4()
		{
			identity();
		}

		matrix4(const GLfloat a[])
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
		
		void rotate(const vector4 &axis, GLfloat theta);
		
		void rotate(const vector4 &fovs);

		void rotate(const quaternion &a);
		
		void scale(const vector4 &a);
		
		matrix4 inverse();

		matrix4 clone();
	};

	vector4 operator * (const matrix4 &m, const vector4 &v);

	matrix4 operator * (const matrix4 &a, const matrix4 &b);

	matrix4 &operator *= (matrix4 &a, const matrix4 &b);
}

#endif