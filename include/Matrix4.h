#ifndef N3D_MATRIX4_H
#define N3D_MATRIX4_H

#include "AABB.h"
#include "Quaternion.h"

namespace nest
{
	class Matrix4 
	{
	public:

		static void perspectiveFov(Matrix4 &a, GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far);

		static void perspectiveOffCenter(Matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

		static void orthoOffCenter(Matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);
		
		/**
		 *	Translate axis & angle rotation to euler angle (yaw pitch roll).
		 */
		static void axisAngleToEuler(const Vector4 &axis, GLfloat angle, Vector4 &euler);

		/**
		 *	Translate euler angle (yaw pitch roll) rotation to axis & angle.
		 */
		static void eulerToAxisAngle(const Vector4 &euler, Vector4 &axis, GLfloat &angle);

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
		
		/**
		 *	YAW, PITCH, ROLL (y axis, x axis, z axis)
		 */
		void rotate(const Vector4 &angles);

		void rotate(const Quaternion &a);
		
		void scale(const Vector4 &a);
		
		Matrix4 inverse();

		Matrix4 clone() const;
	};

	AABB operator * (const Matrix4 &m, const AABB &b);
	
	Vector4 operator * (const Matrix4 &m, const Vector4 &v);

	Matrix4 operator * (const Matrix4 &a, const Matrix4 &b);

	Matrix4 &operator *= (Matrix4 &a, const Matrix4 &b);
}

#endif