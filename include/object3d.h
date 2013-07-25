#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <string>

#include "eventdispatcher.h"
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

		eventdispatcher *dispatcher;

		object3d *parent;

		object3d() : dispatcher(NULL), parent(NULL) {}

		virtual ~object3d()
		{
			if(dispatcher != NULL) delete dispatcher;
			dispatcher = NULL;
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