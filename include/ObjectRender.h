#ifndef N3D_OBJECTRENDER_H
#define N3D_OBJECTRENDER_H

#include <vector>

#include "Camera3d.h"
#include "Mesh.h"

namespace nest
{
	class ObjectRender
	{
	public:

		int numMeshes, numTris, numVts;

		/**
		 *	Test the visibility of all root's child and call their render function.
		 *	
		 *	@param camera View camera.
		 *	@param id Identifier to id renders' behavior for different RenderTargets.
		 *	@param result0 Stores those meshes with alphaTest off and passed specific culling function.
		 *	@param result1 Stores those meshes with alphaTest on and passed specific culling function.
		 *	@param result2 Stores those meshes who didn't pass the culling function.
		 *	
		 *	@see MeshRender
		 *	@see RenderTarget
		 */
		virtual void draw(Camera3d *camera, int id, std::vector<Mesh*> *result0, std::vector<Mesh*> *result1, std::vector<Mesh*> *result2) = 0;

	};
}

#endif