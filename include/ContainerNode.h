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

		void addChild(ObjectNode *object);

		/**
		 *	Remove child from partition tree first, then from it's container.
		 */
		void removeChild(ObjectNode *object);

		/**
		 *	Update ContainerNode's transform by localMatrix.
		 */
		virtual void recompose();
	};
}

#endif