#ifndef N3D_MESH_H
#define N3D_MESH_H

#include "AABB.h"
#include "Object3d.h"

namespace nest
{
	class ORDraw;

	class OcNode;
	
	/**
	 *	When you extend this class, include OcNode.h + OcTree.h + ORDraw.h
	 *	And you'll need to rewrite all those virtual functions.
	 *	delocate()
	 *	{
	 *		if(node != NULL) node->belonging->removeChild(this);
	 *		draw = NULL;
	 *		...
	 *	}
	 *	recompose()
	 *	{
	 *		Object3d::recompose();
	 *		bound = worldMatrix * geom->bound;
	 *		if(node != NULL) node->belonging->transformChild(this);
	 *		...
	 *	}
	 **/
	class Mesh : public Object3d 
	{
	public:

		bool alphaTest;

		float alphaKey;

		bool cliping;

		bool visible;

		bool faceCulling;

		GLenum face;
		
		AABB bound;

		OcNode *node;

		ORDraw *draw;

		Mesh(ORDraw *draw)
		 : alphaTest(false), alphaKey(0.0), cliping(true), visible(true), faceCulling(true), face(GL_BACK), node(NULL), draw(draw) {}

		virtual unsigned int numVts() = 0;

		virtual unsigned int numTris() = 0;
	};
}

#endif