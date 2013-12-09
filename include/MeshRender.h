#ifndef N3D_MESHRENDER_H
#define N3D_MESHRENDER_H

#include <iterator>

#include "Mesh.h"

namespace nest
{
	class MeshRender
	{
	public:

		/**
		 *	@param id You may need different render behavior for different RenderTargets, this int identifies those RenderTargets for you.
		 *	@param mesh Mesh to draw.
		 *	@param invertViewMatrix Camera's invert view matrix.
		 *	@param projectionMatrix Camera's projection matrix.
		 */
		virtual void calculate(int id, Mesh *mesh, Matrix4 *invertViewMatrix, Matrix4 *projectionMatrix) = 0;

	};
}

#endif