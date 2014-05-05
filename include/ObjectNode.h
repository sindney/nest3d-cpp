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
		 *	If it's true, rtMatrix&sMatrix is interpolated from PRS value when recompose is called.
		 */
		bool usePRS;

		Vector4 oldP, newP;

		Quaternion oldR, newR;

		Vector4 oldS, newS;

		Matrix4 rtMatrix, sMatrix;

		Matrix4 localMatrix;

		Matrix4 invertLocalMatrix;

		Matrix4 worldMatrix;

		Matrix4 invertWorldMatrix;

		ObjectNode *parent;

		ObjectNode() : parent(NULL), usePRS(false)
		{
			oldS.x = oldS.y = oldS.z = newS.x = newS.y = newS.z = 1.0f;
			sMatrix.scale(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		virtual ~ObjectNode()
		{
			parent = NULL;
		}

		/**
		 *	Update ObjectNode's transform by localMatrix.
		 *	@param dt Delta time for rendering.
		 */
		virtual void recompose(float dt = 0.0f)
		{
			if(usePRS)
			{
				// store rotation and position info in rtMatrix.
				rtMatrix.identity();
				rtMatrix.rotate(Quaternion::slerp(oldR, newR, dt));
				rtMatrix.translate(oldP + (newP - oldP) * dt);
				// store scaling info in sMatrix.
				sMatrix.identity();
				sMatrix.scale(oldS + (newS - oldS) * dt);
			}
			// calculate local matrix.
			localMatrix = rtMatrix * sMatrix;
			invertLocalMatrix = localMatrix.inverse();
			worldMatrix = localMatrix;
			if(parent != NULL) worldMatrix = parent->worldMatrix * worldMatrix;
			invertWorldMatrix = worldMatrix.inverse();
		}
	};
}

#endif