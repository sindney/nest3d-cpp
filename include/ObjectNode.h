#ifndef N3D_SCENENODE_H
#define N3D_SCENENODE_H

#include <string>

#include "Matrix4.h"

namespace nest
{
	class ObjectNode
	{
	public:

		std::string name;

		Matrix4 localMatrix;

		Matrix4 invertLocalMatrix;

		Matrix4 worldMatrix;

		Matrix4 invertWorldMatrix;

		ObjectNode *parent;

		ObjectNode() : parent(NULL) {}

		virtual ~ObjectNode()
		{
			parent = NULL;
		}

		/**
		 *	Update ObjectNode's transform by localMatrix.
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