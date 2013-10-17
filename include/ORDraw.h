#ifndef N3D_ORDRAW_H
#define N3D_ORDRAW_H

#include "Mesh.h"

namespace nest
{
	class ORDraw
	{
	public:

		virtual void calculate(Mesh *mesh, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix) = 0;

	};
}

#endif