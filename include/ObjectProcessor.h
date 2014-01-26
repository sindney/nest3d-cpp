#ifndef N3D_OBJECTPROCESSOR_H
#define N3D_OBJECTPROCESSOR_H

#include <vector>

#include "CameraNode.h"
#include "MeshNode.h"

namespace nest
{
	class ObjectProcessor
	{
	public:

		/**
		 *	Test the visibility of all root's child and call their render function if you need.
		 *	
		 *	@param camera View camera.
		 *	@param flag Indicates whether to draw meshes passed culling process.
		 *	@param result0 Stores those meshes with alphaTest off and passed specific culling function.
		 *	@param result1 Stores those meshes with alphaTest on and passed specific culling function.
		 *	@param result2 Stores those meshes who didn't pass the culling function.
		 *	
		 *	@see MeshRender
		 *	@see RenderTarget
		 */
		virtual void calculate(CameraNode *camera, bool flag, std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2) = 0;

	};
}

#endif