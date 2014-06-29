#ifndef N3D_QUATERNION_H
#define N3D_QUATERNION_H

#include "Vector4.h"

namespace nest
{
	class Quaternion
	{
	public:

		static GLfloat dotProduct(const Quaternion &a, const Quaternion &b);

		static Quaternion slerp(const Quaternion &a, const Quaternion &b, GLfloat t);

		static Quaternion conjugate(const Quaternion &a);

		static Quaternion pow(const Quaternion &a, GLfloat exp);

		/**
		 *	Translate quaternion rotation to axis radian rotation.
		 */
		static void quatToAxisRadian(Quaternion &quat, Vector4 &axis, GLfloat &radian);
		
		GLfloat x, y, z, w;
		
		Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		
		Quaternion(const Quaternion &a) : x(a.x), y(a.y), z(a.z), w(a.w) {}
		
		Quaternion(GLfloat nx, GLfloat ny, GLfloat nz, GLfloat nw) : x(nx), y(ny), z(nz), w(nw) {}
		
		void identity() 
		{
			x = y = z = 0.0f;
			w = 1.0f;
		}
		
		void rotate(const Vector4 &axis, GLfloat radian);

		/**
		 *	YAW, PITCH, ROLL (y axis, x axis, z axis)
		 */
		void rotate(const Vector4 &euler);
		
		void normalize();
		
		Quaternion operator * (const Quaternion &a) const;
		
		Quaternion &operator *= (const Quaternion &a);
	};
}

#endif