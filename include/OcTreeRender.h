#ifndef N3D_OCTREERENDER_H
#define N3D_OCTREERENDER_H

#include <vector>

#include "OcTree.h"
#include "ObjectRender.h"

namespace nest
{
	class OcTreeRender : public ObjectRender
	{
	public:

		OcTree *tree;

		OcTreeRender(OcTree *tree, ORDraw *draw, Camera3d *camera)
		 : tree(tree), ObjectRender(draw, camera) {}

		~OcTreeRender()
		{
			if(tree != NULL) delete tree;
		}

		void calculate();

	};
}

#endif