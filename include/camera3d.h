#ifndef CAMERA3D_H
#define CAMERA3D_H

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

		static void setupCamera(camera3d &camera, float fov, float aspect, float near, float far)
		{
			camera.fov = fov;
			camera.aspect = aspect;
			camera.near = near;
			camera.far = far;
			camera.culling.create(fov, aspect, near, far);
			matrix4::perspectiveProjection(camera.projectionMatrix, fov, aspect, near, far);
		}
		
	};
}

#endif