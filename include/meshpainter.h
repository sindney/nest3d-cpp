#ifndef N3D_MESHPAINTER_H
#define N3D_MESHPAINTER_H

#include "camera3d.h"

namespace nest
{
	class mesh;

	class meshpainter
	{
	public:
		/**
		 *	Pass an int to id current rendering process.
		 *	Then we can use a switch to draw different approaches in different conditions.
		 */
		virtual void draw(mesh *object, camera3d *camera, int process) = 0;
	};
}

#endif