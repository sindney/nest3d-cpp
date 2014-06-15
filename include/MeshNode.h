#ifndef N3D_MESHNODE_H
#define N3D_MESHNODE_H

#include "ObjectNode.h"

namespace nest
{
	class Mesh;
	
	class OcNode;

	class OcTree;
	
	class MeshNode : public ObjectNode 
	{
	public:

		bool alphaTest;

		float alphaKey;

		bool cliping;

		bool visible;

		OcNode *node;

		OcTree *tree;

		/**
		 *	You take of it's delocation.
		 */
		Mesh *mesh;

		/**
		 *	MeshNode's boundingbox in worldspace.
		 */
		AABB bound;

		MeshNode(Mesh *mesh)
		 : mesh(mesh), alphaTest(false), alphaKey(0.0f), node(NULL), tree(NULL), cliping(true), visible(true) {}

		virtual ~MeshNode();

		/**
		 *	Update MeshNode's bounds first.
		 *	<p>Then update it's position in octree if it's attached to one.</p>
		 */
		virtual void update();
	};
}

#endif