#ifndef N3D_CAMERANODE_H
#define N3D_CAMERANODE_H

#include "ObjectCulling.h"
#include "ObjectNode.h"

namespace nest
{
	class CameraNode : public ObjectNode
	{
	public:

		Matrix4 projectionMatrix;

		ObjectCulling culling;
	};
}

#endif