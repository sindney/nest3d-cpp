#ifndef N3D_CONTAINERNODE_H
#define N3D_CONTAINERNODE_H

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

		void addChild(ObjectNode *object);

		void removeChild(ObjectNode *object);

		virtual void recompose(float dt = 1.0f);

		/**
		 *	Reload dispatch function to broadcast event to child objects.
		 */
		virtual void dispatch(const Event *event, bool depthFirst);
	};
}

#endif