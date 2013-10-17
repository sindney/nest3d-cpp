#ifndef N3D_ORDRAW_H
#define N3D_ORDRAW_H

#include "Mesh.h"

namespace nest
{
	class ORDraw
	{
	public:

		virtual void calculate(Mesh *mesh, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix, int id) = 0;

	};
}

#endif