#ifndef N3D_MESH_H
#define N3D_MESH_H

#include "aabb.h"
#include "camera3d.h"
#include "object3d.h"

namespace nest
{
	class ocnode;
	
	/**
	 *	When you extend this class, include ocnode.h + octree.h, 
	 *	and you'll need to rewrite: 
	 *	delocate()
	 *	{
	 *		geom = NULL;
	 *		shader = NULL;
	 *		if(node != NULL) node->belonging->removeChild(this);
	 *		...
	 *	}
	 *	recompose()
	 *	{
	 *		object3d::recompose();
	 *		aabb::transform(worldMatrix, geom->bound, bound);
	 *		if(node != NULL) node->belonging->transformChild(this);
	 *		...
	 *	}
	 *	draw(int process, camera3d *camera)
	 *	{
	 *		// Process passes an int to id the current rendering process.
	 *		// So that we can use a switch to draw different approaches in different conditions.
	 *		// And you can overlad draw() function to suit your custom containerprocess/renderprocess classes.
	 *	}
	 *	numVertices()
	 *	numTriangles()
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

		mesh()
		 : alphaTest(false), alphaKey(0.0), cliping(true), visible(true), faceCulling(true), face(GL_BACK), node(NULL) {}

		virtual void draw(int process, camera3d *camera) = 0;

		virtual unsigned int numVertices() = 0;

		virtual unsigned int numTriangles() = 0;
	};
}

#endif