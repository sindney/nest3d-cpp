#ifndef N3D_MESH_H
#define N3D_MESH_H

#include "Object3d.h"

namespace nest
{
	class OcNode;

	class MeshRender;
	
	/**
	 *	You'd rewrite recompose function when you extend this class.
	 *	First, call Object3d::recompose() then translate it's bbox, bound = worldMatrix * geometry->bound.
	 *	Finally transform it's node, node->belonging->transformChild(this) and you're done.
	 *	Don't forget to rewrite numVts, numTris as well.
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

		MeshRender *render;

		AABB bound;

		Mesh(MeshRender *render);

		~Mesh();

		virtual int numVts();

		virtual int numTris();
	};
}

#endif