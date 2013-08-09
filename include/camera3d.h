#ifndef N3D_CAMERA3D_H
#define N3D_CAMERA3D_H

#include "frustum.h"
#include "matrix4.h"
#include "object3d.h"

namespace nest
{
	class camera3d : public object3d
	{
	public:

		matrix4 projectionMatrix;

		frustum culling;

		float fov, aspect, near, far;

		void configure(float fov, float aspect, float near, float far)
		{
			this->fov = fov;
			this->aspect = aspect;
			this->near = near;
			this->far = far;
			culling.create(fov, aspect, near, far);
			matrix4::perspectiveProjection(projectionMatrix, fov, aspect, near, far);
		}
		
	};
}

#endif