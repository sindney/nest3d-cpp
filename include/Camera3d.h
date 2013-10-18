#ifndef N3D_CAMERA3D_H
#define N3D_CAMERA3D_H

#include "Object3d.h"
#include "ObjectCulling.h"

namespace nest
{
	class Camera3d : public Object3d
	{
	public:

		Matrix4 projectionMatrix;

		ObjectCulling *culling;

		/**
		 *	@param culling Camera's culling part.
		 */
		Camera3d(ObjectCulling *culling) : culling(culling) {}

		~Camera3d()
		{
			if(culling != NULL) delete culling;
		}
	};
}

#endif