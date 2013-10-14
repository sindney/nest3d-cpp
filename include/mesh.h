#ifndef N3D_MESH_H
#define N3D_MESH_H

#include "aabb.h"
#include "camera3d.h"
#include "object3d.h"

namespace nest
{
	class meshpainter;

	class ocnode;
	
	/**
	 *	When you extend this class, include ocnode.h + octree.h.
	 *	And you'll need to rewrite all those virtual functions.
	 *	delocate()
	 *	{
	 *		if(node != NULL) node->belonging->removeChild(this);
	 *		node = NULL;
	 *		painter = NULL;
	 *		...
	 *	}
	 *	recompose()
	 *	{
	 *		object2d::recompose();
	 *		aabr::transform(worldMatrix, geom->bound, bound);
	 *		if(node != NULL) node->belonging->transformChild(this);
	 *		...
	 *	}
	 **/
	class mesh : public object3d 
	{
	public:

		bool alphaTest;

		float alphaKey;

		bool cliping;

		bool visible;

		bool faceCulling;

		GLenum face;
		
		aabb bound;

		ocnode *node;

		meshpainter *painter;

		mesh(meshpainter *painter)
		 : alphaTest(false), alphaKey(0.0), cliping(true), visible(true), faceCulling(true), face(GL_BACK), node(NULL), painter(painter) {}

		virtual unsigned int numVertices() = 0;

		virtual unsigned int numTriangles() = 0;
	};
}

#endif