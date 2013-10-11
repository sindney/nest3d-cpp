#ifndef N3D_CONTAINERROCESS_H
#define N3D_CONTAINERROCESS_H

#include <vector>

#include "camera3d.h"
#include "container3d.h"
#include "mesh.h"
#include "octree.h"
#include "renderprocess.h"

namespace nest
{
	class containerprocess : public renderprocess
	{
	public:

		octree *partition;

		container3d *container;

		camera3d *camera;

		vector<object3d*> objects;

		vector<mesh*> alphaObjects;

		int numObjects;

		int numTriangles;

		int numVertices;

		int id;

		containerprocess(int id, rendertarget *target, octree *partition, container3d *container, camera3d *camera)
		 : id(id), renderprocess(target), partition(partition), container(container), camera(camera), numObjects(0), numTriangles(0), numVertices(0) {}

		~containerprocess();

		void calculate();

		bool operator() (mesh *i, mesh *j) { return i->alphaKey > j->alphaKey; }
	};
}

#endif