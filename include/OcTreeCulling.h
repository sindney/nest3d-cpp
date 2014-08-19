#ifndef N3D_OCTREECULLING_H
#define N3D_OCTREECULLING_H

#include <vector>

namespace nest
{
	class CameraNode;

	class MeshNode;

	class OcTree;

	class OcTreeCulling
	{
	public:

		/**
		 *	Test the visibility of octree's meshes and store result in vector.
		 *	
		 *	@param result0 Stores those meshes with alphaSort off and passed specific culling function.
		 *	@param result1 Stores those meshes with alphaSort on and passed specific culling function.
		 *	@param result2 Stores those meshes who didn't pass the culling function.
		 */
		static void classify(
			OcTree *tree, CameraNode *camera, 
			std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2
		);
		
	};
}

#endif