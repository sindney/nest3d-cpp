#ifndef N3D_OBJECT3D_H
#define N3D_OBJECT3D_H

#include <string>

#include "matrix4.h"

namespace nest
{
	class object3d
	{
	public:

		std::string name;

		matrix4 localMatrix;

		matrix4 invertLocalMatrix;

		matrix4 worldMatrix;

		matrix4 invertWorldMatrix;

		object3d *parent;

		object3d() : parent(NULL) {}

		virtual ~object3d()
		{
			parent = NULL;
		}

		/**
		 *	Update object3d's transform by localMatrix.
		 */
		virtual void recompose()
		{
			invertLocalMatrix = localMatrix.inverse();
			worldMatrix = localMatrix;
			if(parent != NULL) worldMatrix *= parent->worldMatrix;
			invertWorldMatrix = worldMatrix.inverse();
		}
	};
}

#endif