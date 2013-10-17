#ifndef N3D_OBJECTRENDER_H
#define N3D_OBJECTRENDER_H

#include <vector>

#include "Camera3d.h"
#include "Mesh.h"
#include "ORDraw.h"

namespace nest
{
	class ObjectRender
	{
	public:

		ORDraw *draw;

		Camera3d *camera;

		std::vector<Mesh*> passed0, passed1, rejected;

		bool recordRejectedMeshes;

		int numMeshes, numTris, numVts;

		ObjectRender(ORDraw *draw, Camera3d *camera)
		 : draw(draw), camera(camera), recordRejectedMeshes(false), numMeshes(0), numTris(0), numVts(0) {}

		virtual ~ObjectRender()
		{
			if(draw != NULL) delete draw;
			if(camera != NULL) delete camera;
			passed0.clear();
			passed1.clear();
			rejected.clear();
		}

		virtual void calculate() = 0;

	};
}

#endif