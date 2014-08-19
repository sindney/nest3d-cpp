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

		/**
		 *	Set to true to sort visible meshNodes by alphaKey.
		 *	<p>So that we can render mesh with alpha channels correctlly.</p>
		 *	
		 *	@see ContainerCulling
		 *	@see OcTreeCulling
		 */
		bool alphaSort;

		/**
		 *	When alphaSort is true.
		 *	<p>This stores MeshNode's distance to view camera.</p>
		 */
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
		 : mesh(mesh), alphaSort(false), alphaKey(0.0f), node(NULL), tree(NULL), cliping(true), visible(true) {}

		virtual ~MeshNode();
		
		/**
		 *	Update matrices, bound and octree info if it's attached to one.
		 */
		virtual void recompose(float dt = 0.0f);

		/**
		 *	For skined mesh to update it's aniamtion keys.
		 */
		virtual void advanceTime(float dt) {}

		/**
		 *	For skinned mesh to transfer data from key frames to joint matrices.
		 */
		virtual void displayAnimation(float dt);

		/**
		 *	Compare function to sort nodes for rendering alpha sort enabled object.
		 */
		bool operator() (const MeshNode *node0, const MeshNode *node1) const
		{ 
			return node0->alphaKey > node1->alphaKey;
		}
	};
}

#endif