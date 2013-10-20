#ifndef N3D_OCTREERENDER_H
#define N3D_OCTREERENDER_H

#include "OcTree.h"
#include "ObjectRender.h"

namespace nest
{
	/**
	 *	A ObjectRender that renders objects stored in octree.
	 */
	class OcTreeRender : public ObjectRender
	{
	public:

		OcTree *tree;

		/**
		 *	@param tree Target octree.
		 *	@param draw Draw function for those meshes who has no draw function.
		 *	@param camera View camera which stores view & projection matrices.
		 */
		OcTreeRender(OcTree *tree, ORDraw *draw, Camera3d *camera)
		 : tree(tree), ObjectRender(draw, camera) {}

		~OcTreeRender()
		{
			if(tree != NULL) delete tree;
		}

		/**
		 *	Test the visibility of all root's child and call their draw function.
		 *	
		 *	@param result0 Stores those meshes with alphaTest off and passed specific culling function.
		 *	@param result1 Stores those meshes with alphaTest on and passed specific culling function.
		 *	@param result2 Stores those meshes who didn't pass the culling function.
		 */
		void calculate(std::vector<Mesh*> *result0, std::vector<Mesh*> *result1, std::vector<Mesh*> *result2);

	};
}

#endif