#include <stdexcept>

#include "matrix4.h"

namespace nest
{
	void matrix4::identity()
	{
		raw[0] = 1.0f; raw[1] = 0.0f; raw[2] = 0.0f; raw[3] = 0.0f;
		raw[4] = 0.0f; raw[5] = 1.0f; raw[6] = 0.0f; raw[7] = 0.0f;
		raw[8] = 0.0f; raw[9] = 0.0f; raw[10] = 1.0f; raw[11] = 0.0f;
		raw[12] = 0.0f; raw[13] = 0.0f; raw[14] = 0.0f; raw[15] = 1.0f;
	}

	void matrix4::translate(const vector4 &a)
	{
		raw[12] = a.x; raw[13] = a.y; raw[14] = a.z;
	}

	void matrix4::rotate(const vector4 &axis, float theta)
	{
		float s = sin(theta);
		float c = cos(theta);
		
		float a = 1.0f - c;
		float ax = a * axis.x;
		float ay = a * axis.y;
		float az = a * axis.z;
		
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

	// Y, X, Z, YAW, PITCH, ROLL
	void matrix4::rotate(const vector4 &angles)
	{
		float sa = sin(angles.x);
		float ca = cos(angles.x);
		float sb = sin(angles.y);
		float cb = cos(angles.y);
		float sc = sin(angles.z);
		float cc = cos(angles.z);

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

	void matrix4::rotate(const quaternion &a)
	{
		float ww = 2.0f * a.w;
		float xx = 2.0f * a.x;
		float yy = 2.0f * a.y;
		float zz = 2.0f * a.z;
		
		raw[0] = 1.0f - yy * a.y - zz * a.z;
		raw[1] = xx * a.y + ww * a.z;
		raw[2] = xx * a.z - ww * a.x;
		raw[4] = xx * a.y - ww * a.z;
		raw[5] = 1.0f - xx * a.x - zz * a.z;
		raw[6] = yy * a.z + ww * a.x;
		raw[8] = xx * a.z + ww * a.y;
		raw[9] = yy * a.z - ww * a.x;
		raw[10] = 1.0f - xx * a.x - yy * a.y;
	}

	void matrix4::scale(const vector4 &a)
	{
		raw[0] = a.x;  raw[1] = 0.0f; raw[2] = 0.0f;
		raw[4] = 0.0f; raw[5] = a.y;  raw[6] = 0.0f;
		raw[8] = 0.0f; raw[9] = 0.0f; raw[10] = a.z;
	}

	matrix4 matrix4::inverse()
	{
		float det = raw[0] * (raw[5] * raw[10] - raw[6] * raw[9])
					+ raw[1] * (raw[6] * raw[8] - raw[4] * raw[10])
					+ raw[2] * (raw[4] * raw[9] - raw[5] * raw[8]);
		if(det == 0) throw std::runtime_error("Error inversing target matrix.");
		
		float det2 = 1.0f / det;
		
		matrix4 result;
		
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

	matrix4 matrix4::clone()
	{
		return matrix4(this->raw);
	}

	void matrix4::perspectiveProjection(matrix4 &a, float fov, float ratio, float near, float far)
	{
		float ys = 1.0f / tan(fov / 2.0f);
		float xs = ys / ratio;
		int i;
		for(i = 0; i < 16; i++) a.raw[i] = 0.0f;
		a.raw[0] = ys;
		a.raw[5] = xs;
		a.raw[10] = (far + near) / (near - far);
		a.raw[11] = -1;
		a.raw[14] = 2 * far * near / (near - far);
	}

	void matrix4::orthographicProjection(matrix4 &a, float fov, float ratio, float near, float far)
	{
		float scale = tan(fov * 0.5) * near;
		float r = ratio * scale;
		int i;
		for(i = 0; i < 16; i++) a.raw[i] = 0.0f;
		a.raw[0] = 1 / r;
		a.raw[5] = 1 / scale;
		a.raw[10] = -2 / (far - near);
		a.raw[11] = (far + near) / (near - far);
		a.raw[15] = 1;
	}

	vector4 operator * (const vector4 &v, const matrix4 &m)
	{
		return vector4(
			v.x * m.raw[0] + v.y * m.raw[4] + v.z * m.raw[8] + v.w * m.raw[12], 
			v.x * m.raw[1] + v.y * m.raw[5] + v.z * m.raw[9] + v.w * m.raw[13], 
			v.x * m.raw[2] + v.y * m.raw[6] + v.z * m.raw[10] + v.w * m.raw[14], 
			1.0f
		);
	}

	vector4 &operator *= (vector4 &v, const matrix4 &m)
	{
		v = v * m;
		return v;
	}

	matrix4 operator * (const matrix4 &a, const matrix4 &b)
	{
		matrix4 result;
		
		result.raw[0] = a.raw[0] * b.raw[0] + a.raw[1] * b.raw[4] + a.raw[2] * b.raw[8];
		result.raw[1] = a.raw[0] * b.raw[1] + a.raw[1] * b.raw[5] + a.raw[2] * b.raw[9];
		result.raw[2] = a.raw[0] * b.raw[2] + a.raw[1] * b.raw[6] + a.raw[2] * b.raw[10];
		result.raw[3] = 0.0f;
		result.raw[4] = a.raw[4] * b.raw[0] + a.raw[5] * b.raw[4] + a.raw[6] * b.raw[8];
		result.raw[5] = a.raw[4] * b.raw[1] + a.raw[5] * b.raw[5] + a.raw[6] * b.raw[9];
		result.raw[6] = a.raw[4] * b.raw[2] + a.raw[5] * b.raw[6] + a.raw[6] * b.raw[10];
		result.raw[7] = 0.0f;
		result.raw[8] = a.raw[8] * b.raw[0] + a.raw[9] * b.raw[4] + a.raw[10] * b.raw[8];
		result.raw[9] = a.raw[8] * b.raw[1] + a.raw[9] * b.raw[5] + a.raw[10] * b.raw[9];
		result.raw[10] = a.raw[8] * b.raw[2] + a.raw[9] * b.raw[6] + a.raw[10] * b.raw[10];
		result.raw[11] = 0.0f;
		result.raw[12] = a.raw[12] * b.raw[0] + a.raw[13] * b.raw[4] + a.raw[14] * b.raw[8] + b.raw[12];
		result.raw[13] = a.raw[12] * b.raw[1] + a.raw[13] * b.raw[5] + a.raw[14] * b.raw[9] + b.raw[13];
		result.raw[14] = a.raw[12] * b.raw[2] + a.raw[13] * b.raw[6] + a.raw[14] * b.raw[10] + b.raw[14];
		result.raw[15] = 1.0f;
		
		return result;
	}

	matrix4 &operator *= (matrix4 &a, const matrix4 &b)
	{
		a = a * b;
		return a;
	}
}