#ifndef N3D_MESH_H
#define N3D_MESH_H

#include "Object3d.h"

namespace nest
{
	class OcNode;

	class OcTree;

	class MeshRender;
	
	/**
	 *	You'd rewrite recompose function when you extend this class.
	 *	Object3d::recompose(); // Update transfoem matriices.
	 *	bound = worldMatrix * geometry->bound; // Translate Bounding box.
	 *	tree->transformChild(this); // Translate node in octree.
	 */
	class Mesh : public Object3d 
	{
	public:

		bool alphaTest;

		float alphaKey;

		bool cliping;

		bool visible;

		bool faceCulling;

		GLenum face;

		OcNode *node;

		OcTree *tree;

		/**
		 *	You take care of this pointer's deletion.
		 */
		MeshRender *render;

		AABB bound;

		Mesh(MeshRender *render);

		~Mesh();

		virtual int numVts() = 0;

		virtual int numTris() = 0;
	};
}

#endif