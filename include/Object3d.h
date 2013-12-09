#ifndef N3D_OBJECT3D_H
#define N3D_OBJECT3D_H

#include <string>

#include "EventActor.h"
#include "Matrix4.h"

namespace nest
{
	class Object3d : public EventDispatcher
	{
	public:

		std::string name;

		Matrix4 localMatrix;

		Matrix4 invertLocalMatrix;

		Matrix4 worldMatrix;

		Matrix4 invertWorldMatrix;

		Object3d *parent;

		Object3d() : parent(NULL) {}

		virtual ~Object3d()
		{
			parent = NULL;
		}

		/**
		 *	Update Object3d's transform by localMatrix.
		 */
		virtual void recompose()
		{
			invertLocalMatrix = localMatrix.inverse();
			worldMatrix = localMatrix;
			if(parent != NULL) worldMatrix = parent->worldMatrix * worldMatrix;
			invertWorldMatrix = worldMatrix.inverse();
		}
	};
}

#endif