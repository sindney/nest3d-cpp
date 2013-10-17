#ifndef N3D_MATRIX4_H
#define N3D_MATRIX4_H

#include "AABB.h"
#include "Quaternion.h"

namespace nest
{
	/**
	 *	0	4	8	12
	 *	1	5	9	13
	 *	2	6	10	14
	 *	3	7	11	15
	 */
	class Matrix4 
	{
	public:

		static void perspectiveFov(Matrix4 &a, GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far);

		static void perspectiveOffCenter(Matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

		static void orthoOffCenter(Matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
		
		GLfloat raw[16];

		Matrix4()
		{
			identity();
		}

		Matrix4(const GLfloat a[])
		{
			int i;
			for(i = 0; i < 16; i++) raw[i] = a[i];
		}

		Matrix4(const Matrix4 &a) 
		{
			int i;
			for(i = 0; i < 16; i++) raw[i] = a.raw[i];
		}

		void identity();
		
		void translate(const Vector4 &a);
		
		void rotate(const Vector4 &axis, GLfloat theta);
		
		void rotate(const Vector4 &fovs);

		void rotate(const Quaternion &a);
		
		void scale(const Vector4 &a);
		
		Matrix4 inverse();

		Matrix4 clone();
	};

	AABB operator * (const Matrix4 &m, const AABB &b);
	
	Vector4 operator * (const Matrix4 &m, const Vector4 &v);

	Matrix4 operator * (const Matrix4 &a, const Matrix4 &b);

	Matrix4 &operator *= (Matrix4 &a, const Matrix4 &b);
}

#endif