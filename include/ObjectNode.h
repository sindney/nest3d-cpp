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

		/**
		 *	Matrix for rotation & position info in local space.
		 */
		Matrix4 rtMatrix;

		/**
		 *	Matrix for scale info in local space.
		 */
		Matrix4 sMatrix;

		Matrix4 localMatrix;

		Matrix4 invertLocalMatrix;

		Matrix4 worldMatrix;

		Matrix4 invertWorldMatrix;

		ObjectNode *parent;

		ObjectNode() : parent(NULL) 
		{
			sMatrix.scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		virtual ~ObjectNode()
		{
			parent = NULL;
		}

		/**
		 *	Update ObjectNode's transform by localMatrix.
		 */
		virtual void recompose()
		{
			localMatrix = rtMatrix * sMatrix;
			invertLocalMatrix = localMatrix.inverse();
			worldMatrix = localMatrix;
			if(parent != NULL) worldMatrix = parent->worldMatrix * worldMatrix;
			invertWorldMatrix = worldMatrix.inverse();
		}
	};
}

#endif