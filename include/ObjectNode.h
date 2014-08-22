#ifndef N3D_OBJECTNODE_H
#define N3D_OBJECTNODE_H

#include <string>

#include "EventDispatcher.h"
#include "Matrix4.h"

namespace nest
{
	class ObjectNode : public EventDispatcher
	{
	public:

		std::string name;

		/**
		 *	If it's true, localMatrix is interpolated from PRS value when recompose is called.
		 */
		bool usePRS;

		Vector4 oldP, newP;

		Quaternion oldR, newR;

		Vector4 oldS, newS;

		Matrix4 localMatrix;

		Matrix4 invertLocalMatrix;

		Matrix4 worldMatrix;

		Matrix4 invertWorldMatrix;

		ObjectNode *parent;

		ObjectNode() : parent(NULL), usePRS(false)
		{
			oldS.x = oldS.y = oldS.z = newS.x = newS.y = newS.z = 1.0f;
		}

		virtual ~ObjectNode()
		{
			parent = NULL;
		}

		/**
		 *	Update ObjectNode's transform by localMatrix.
		 *	
		 *	@param dt Delta time for rendering.
		 */
		virtual void recompose(float dt = 0.0f)
		{
			// calculate local matrix.
			if(usePRS)
			{
				// append rotation and position info.
				localMatrix.identity();
				localMatrix.rotate(Quaternion::slerp(oldR, newR, dt));
				localMatrix.translate(oldP + (newP - oldP) * dt);
				// append scaling info.
				Matrix4 sMatrix;
				sMatrix.scale(oldS + (newS - oldS) * dt);
				localMatrix *= sMatrix;
			}
			invertLocalMatrix = localMatrix.inverse();
			worldMatrix = localMatrix;
			if(parent != NULL) worldMatrix = parent->worldMatrix * worldMatrix;
			invertWorldMatrix = worldMatrix.inverse();
		}
	};
}

#endif