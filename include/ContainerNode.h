#ifndef N3D_CONTAINERNODE_H
#define N3D_CONTAINERNODE_H

#include <string>
#include <vector>

#include "ObjectNode.h"

namespace nest
{
	class ContainerNode : public ObjectNode
	{
	public:

		std::vector<ObjectNode*> objects;

		bool visible;

		ContainerNode() : visible(true) {}

		virtual ~ContainerNode();

		/**
		 *	If target object is MeshNode.
		 *	<p>You'd call MeshNode's update function to update mesh's bounds and ocTree info.</p>
		 */
		void addChild(ObjectNode *object);

		/**
		 *	If target object is MeshNode.
		 *	<p>You'd call MeshNode's update function to update mesh's bounds and ocTree info.</p>
		 */
		void removeChild(ObjectNode *object);

		/**
		 *	Update ContainerNode's transform by localMatrix.
		 */
		virtual void recompose(float dt = 1.0f);

		/**
		 *	Reload dispatch function to broadcast event to child objects.
		 *	
		 *	@param depthFirst Traverse the container with depth first order.
		 */
		virtual void dispatch(const Event *event, bool depthFirst);
	};
}

#endif