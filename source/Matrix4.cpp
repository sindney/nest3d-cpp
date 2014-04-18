#include <stdexcept>

#include "Matrix4.h"

namespace nest
{
	void Matrix4::perspectiveFov(Matrix4 &a, GLfloat fov, GLfloat ratio, GLfloat near, GLfloat far)
	{
		GLfloat ys = 1.0f / tan(fov / 2.0f);
		GLfloat xs = ys / ratio;
		int i;
		for(i = 0; i < 16; i++) a.raw[i] = 0.0f;
		a.raw[0] = ys;
		a.raw[5] = xs;
		a.raw[10] = (far + near) / (near - far);
		a.raw[11] = -1;
		a.raw[14] = 2 * far * near / (near - far);
	}

	void Matrix4::perspectiveOffCenter(Matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
	{
		int i;
		for(i = 0; i < 16; i++) a.raw[i] = 0.0f;
		a.raw[0] = 2 * near / (right - left);
		a.raw[5] = 2 * near / (top - bottom);
		a.raw[8] = (right + left) / (right - left);
		a.raw[9] = (top + bottom) / (top - bottom);
		a.raw[10] = (far + near) / (near - far);
		a.raw[11] = -1;
		a.raw[14] = 2 * far * near / (near - far);
	}

	void Matrix4::orthoOffCenter(Matrix4 &a, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
	{
		int i;
		for(i = 0; i < 16; i++) a.raw[i] = 0.0f;
		a.raw[0] = 2 / (right - left);
		a.raw[5] = 2 / (top - bottom);
		a.raw[10] = -2 / (far - near);
		a.raw[12] = (right + left) / (left - right);
		a.raw[13] = (top + bottom) / (bottom - top);
		a.raw[14] = (far + near) / (near - far);
		a.raw[15] = 1;
	}

	void Matrix4::identity()
	{
		raw[0] = 1.0f; raw[4] = 0.0f; raw[8] = 0.0f; raw[12] = 0.0f;
		raw[1] = 0.0f; raw[5] = 1.0f; raw[9] = 0.0f; raw[13] = 0.0f;
		raw[2] = 0.0f; raw[6] = 0.0f; raw[10] = 1.0f; raw[14] = 0.0f;
		raw[3] = 0.0f; raw[7] = 0.0f; raw[11] = 0.0f; raw[15] = 1.0f;
	}

	void Matrix4::translate(const Vector4 &a)
	{
		raw[12] = a.x; raw[13] = a.y; raw[14] = a.z;
	}

	void Matrix4::rotate(const Vector4 &axis, GLfloat theta)
	{
		GLfloat s = sin(theta);
		GLfloat c = cos(theta);
		
		GLfloat a = 1.0f - c;
		GLfloat ax = a * axis.x;
		GLfloat ay = a * axis.y;
		GLfloat az = a * axis.z;
		
		raw[0] = ax * axis.x + c;
		raw[1] = ax * axis.y + axis.z * s;
		raw[2] = ax * axis.z - axis.y * s;
		raw[4] = ay * axis.x - axis.z * s;
		raw[5] = ay * axis.y + c;
		raw[6] = ay * axis.z + axis.x * s;
		raw[8] = az * axis.x + axis.y * s;
		raw[9] = az * axis.y - axis.x * s;
		raw[10] = az * axis.z + c;
	}
	
	void Matrix4::rotate(const Vector4 &angles)
	{
		GLfloat sa = sin(angles.x);
		GLfloat ca = cos(angles.x);
		GLfloat sb = sin(angles.y);
		GLfloat cb = cos(angles.y);
		GLfloat sc = sin(angles.z);
		GLfloat cc = cos(angles.z);

		raw[0] = cb * cc + sb * sa * sc;
		raw[1] = ca * sc;
		raw[2] = cb * sa * sc - sb * cc;
		raw[4] = sb * sa * cc - cb * sc;
		raw[5] = ca * cc;
		raw[6] = sb * sc + cb * sa * cc;
		raw[8] = sb * ca;
		raw[9] = -sa;
		raw[10] = cb * ca;
	}

	void Matrix4::rotate(const Quaternion &a)
	{
		GLfloat ww = 2.0f * a.w;
		GLfloat xx = 2.0f * a.x;
		GLfloat yy = 2.0f * a.y;
		GLfloat zz = 2.0f * a.z;
		
		raw[0] = 1.0f - yy * a.y - zz * a.z;
		raw[1] = xx * a.y + ww * a.z;
		raw[2] = xx * a.z - ww * a.y;
		raw[4] = xx * a.y - ww * a.z;
		raw[5] = 1.0f - xx * a.x - zz * a.z;
		raw[6] = yy * a.z + ww * a.x;
		raw[8] = xx * a.z + ww * a.y;
		raw[9] = yy * a.z - ww * a.x;
		raw[10] = 1.0f - xx * a.x - yy * a.y;
	}

	void Matrix4::scale(const Vector4 &a)
	{
		raw[0] = a.x;  raw[4] = 0.0f; raw[8] = 0.0f;
		raw[1] = 0.0f; raw[5] = a.y;  raw[9] = 0.0f;
		raw[2] = 0.0f; raw[6] = 0.0f; raw[10] = a.z;
	}

	Matrix4 Matrix4::inverse()
	{
		GLfloat det = raw[0] * (raw[5] * raw[10] - raw[6] * raw[9])
					+ raw[1] * (raw[6] * raw[8] - raw[4] * raw[10])
					+ raw[2] * (raw[4] * raw[9] - raw[5] * raw[8]);
		if(det == 0) throw std::runtime_error("Error inversing target matrix.");
		
		GLfloat det2 = 1.0f / det;
		
		Matrix4 result;
		
		result.raw[0] = (raw[5] * raw[10] - raw[6] * raw[9]) * det2;
		result.raw[1] = (raw[2] * raw[9] - raw[1] * raw[10]) * det2;
		result.raw[2] = (raw[1] * raw[6] - raw[2] * raw[5]) * det2;
		result.raw[3] = 0.0f;
		result.raw[4] = (raw[6] * raw[8] - raw[4] * raw[10]) * det2;
		result.raw[5] = (raw[0] * raw[10] - raw[2] * raw[8]) * det2;
		result.raw[6] = (raw[2] * raw[4] - raw[0] * raw[6]) * det2;
		result.raw[7] = 0.0f;
		result.raw[8] = (raw[4] * raw[9] - raw[5] * raw[8]) * det2;
		result.raw[9] = (raw[1] * raw[8] - raw[0] * raw[9]) * det2;
		result.raw[10] = (raw[0] * raw[5] - raw[1] * raw[4]) * det2;
		result.raw[11] = 0.0f;
		result.raw[12] = -(raw[12] * result.raw[0] + raw[13] * result.raw[4] + raw[14] * result.raw[8]);
		result.raw[13] = -(raw[12] * result.raw[1] + raw[13] * result.raw[5] + raw[14] * result.raw[9]);
		result.raw[14] = -(raw[12] * result.raw[2] + raw[13] * result.raw[6] + raw[14] * result.raw[10]);
		result.raw[15] = 1.0f;
		
		return result;
	}

	Matrix4 Matrix4::clone() const
	{
		return Matrix4(this->raw);
	}

	AABB operator * (const Matrix4 &m, const AABB &b)
	{
		Vector4 vertices[] = {
			m * b.min, 
			m * Vector4(b.max.x, b.min.y, b.min.z, 1.0f), 
			m * Vector4(b.min.x, b.max.y, b.min.z, 1.0f), 
			m * Vector4(b.max.x, b.max.y, b.min.z, 1.0f), 
			m * Vector4(b.min.x, b.min.y, b.max.z, 1.0f), 
			m * Vector4(b.max.x, b.min.y, b.max.z, 1.0f), 
			m * Vector4(b.min.x, b.max.y, b.max.z, 1.0f), 
			m * b.max
		};

		AABB result;
		result.max = result.min = vertices[0];

		int i;
		Vector4 *vt;
		for(i = 0; i < 8; i++)
		{
			vt = &vertices[i];
			if(vt->x > result.max.x) 
				result.max.x = vt->x;
			else if(vt->x < result.min.x)
				result.min.x = vt->x;
			if(vt->y > result.max.y) 
				result.max.y = vt->y;
			else if(vt->y < result.min.y)
				result.min.y = vt->y;
			if(vt->z > result.max.z) 
				result.max.z = vt->z;
			else if(vt->z < result.min.z)
				result.min.z = vt->z;
		}

		return result;
	}

	Vector4 operator * (const Matrix4 &m, const Vector4 &v)
	{
		return Vector4(
			v.x * m.raw[0] + v.y * m.raw[4] + v.z * m.raw[8] + v.w * m.raw[12], 
			v.x * m.raw[1] + v.y * m.raw[5] + v.z * m.raw[9] + v.w * m.raw[13], 
			v.x * m.raw[2] + v.y * m.raw[6] + v.z * m.raw[10] + v.w * m.raw[14], 
			1.0f
		);
	}

	Matrix4 operator * (const Matrix4 &a, const Matrix4 &b)
	{
		Matrix4 result;
		
		result.raw[0] = a.raw[0] * b.raw[0] + a.raw[4] * b.raw[1] + a.raw[8] * b.raw[2];
		result.raw[1] = a.raw[1] * b.raw[0] + a.raw[5] * b.raw[1] + a.raw[9] * b.raw[2];
		result.raw[2] = a.raw[2] * b.raw[0] + a.raw[6] * b.raw[1] + a.raw[10] * b.raw[2];
		result.raw[3] = 0.0f;
		result.raw[4] = a.raw[0] * b.raw[4] + a.raw[4] * b.raw[5] + a.raw[8] * b.raw[6];
		result.raw[5] = a.raw[1] * b.raw[4] + a.raw[5] * b.raw[5] + a.raw[9] * b.raw[6];
		result.raw[6] = a.raw[2] * b.raw[4] + a.raw[6] * b.raw[5] + a.raw[10] * b.raw[6];
		result.raw[7] = 0.0f;
		result.raw[8] = a.raw[0] * b.raw[8] + a.raw[4] * b.raw[9] + a.raw[8] * b.raw[10];
		result.raw[9] = a.raw[1] * b.raw[8] + a.raw[5] * b.raw[9] + a.raw[9] * b.raw[10];
		result.raw[10] = a.raw[2] * b.raw[8] + a.raw[6] * b.raw[9] + a.raw[10] * b.raw[10];
		result.raw[11] = 0.0f;
		result.raw[12] = a.raw[0] * b.raw[12] + a.raw[4] * b.raw[13] + a.raw[8] * b.raw[14] + a.raw[12];
		result.raw[13] = a.raw[1] * b.raw[12] + a.raw[5] * b.raw[13] + a.raw[9] * b.raw[14] + a.raw[13];
		result.raw[14] = a.raw[2] * b.raw[12] + a.raw[6] * b.raw[13] + a.raw[10] * b.raw[14] + a.raw[14];
		result.raw[15] = 1.0f;
		
		return result;
	}

	Matrix4 &operator *= (Matrix4 &a, const Matrix4 &b)
	{
		a = a * b;
		return a;
	}
}