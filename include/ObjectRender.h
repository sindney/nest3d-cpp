#ifndef N3D_OBJECTRENDER_H
#define N3D_OBJECTRENDER_H

#include <vector>

#include "Camera3d.h"
#include "ORDraw.h"

namespace nest
{
	class ObjectRender
	{
	public:

		ORDraw *draw;

		Camera3d *camera;

		int numMeshes, numTris, numVts;

		ObjectRender(ORDraw *draw, Camera3d *camera)
		 : draw(draw), camera(camera), numMeshes(0), numTris(0), numVts(0) {}

		virtual ~ObjectRender()
		{
			if(draw != NULL) delete draw;
			if(camera != NULL) delete camera;
		}

		/**
		 *	Test the visibility of all root's child and call their draw function.
		 *	
		 *	@param id Identity int to pass to draw function.
		 *	@param passed0 Stores those meshes with alphaTest off and passed specific culling function.
		 *	@param passed1 Stores those meshes with alphaTest on and passed specific culling function.
		 *	@param rejected Stores those meshes who didn't pass the culling function.
		 */
		virtual void calculate(int id, std::vector<Mesh*> *passed0, std::vector<Mesh*> *passed1, std::vector<Mesh*> *rejected) = 0;

	};
}

#endif