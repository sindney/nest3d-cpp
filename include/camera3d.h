#ifndef N3D_CAMERA3D_H
#define N3D_CAMERA3D_H

#include "culling3d.h"
#include "matrix4.h"
#include "object3d.h"

namespace nest
{
	class camera3d : public object3d
	{
	public:

		matrix4 projectionMatrix;

		culling3d culling;

		float fov, ratio, near, far;

		void configure(float fov, float ratio, float near, float far)
		{
			this->fov = fov;
			this->ratio = ratio;
			this->near = near;
			this->far = far;
			culling.create(fov, ratio, near, far);
			matrix4::perspectiveFov(projectionMatrix, fov, ratio, near, far);
		}
		
	};
}

#endif