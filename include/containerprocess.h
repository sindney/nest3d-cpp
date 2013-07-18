#ifndef CONTAINERROCESS_H
#define CONTAINERROCESS_H

#include <vector>

#include "camera3d.h"
#include "container3d.h"
#include "mesh.h"
#include "renderprocess.h"
#include "utils.h"

namespace nest
{
	class containerprocess : public renderprocess
	{
	public:

		container3d *container;

		camera3d *camera;

		vector<object3d*> objects;

		vector<mesh*> alphaObjects;

		int numObjects;

		int numTriangles;

		int numVertices;

		containerprocess(container3d *container, camera3d *camera)
		 : container(container), camera(camera), numObjects(0), numTriangles(0), numVertices(0) {}

		~containerprocess() 
		{
			container = NULL;
			camera = NULL;
			objects.clear();
			alphaObjects.clear();
		}

		virtual void calculate();

		virtual void drawMesh(mesh *mesh0);

		bool operator() (mesh *i, mesh *j) { return i->alphaKey > j->alphaKey; }
	};
}

#endif