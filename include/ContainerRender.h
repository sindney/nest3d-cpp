#ifndef N3D_CONTAINERRENDER_H
#define N3D_CONTAINERRENDER_H

#include <vector>

#include "Container3d.h"
#include "ObjectRender.h"

namespace nest
{
	class ContainerRender : public ObjectRender
	{
	public:

		Container3d *root;

		ContainerRender(Container3d *root, ORDraw *draw, Camera3d *camera)
		 : root(root), ObjectRender(draw, camera) {}

		~ContainerRender()
		{
			if(root != NULL) delete root;
		}

		void calculate();

	};
}

#endif