#ifndef N3D_CAMERANODE_H
#define N3D_CAMERANODE_H

#include "ObjectNode.h"
#include "ObjectCulling.h"

namespace nest
{
	class CameraNode : public ObjectNode
	{
	public:

		Matrix4 projectionMatrix;

		ObjectCulling *culling;

		/**
		 *	@param culling Camera's culling part.
		 */
		CameraNode(ObjectCulling *culling) : culling(culling) {}

		virtual ~CameraNode()
		{
			if(culling != NULL) delete culling;
		}
	};
}

#endif