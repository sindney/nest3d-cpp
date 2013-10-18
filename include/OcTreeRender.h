#ifndef N3D_OCTREERENDER_H
#define N3D_OCTREERENDER_H

#include <vector>

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
		 *	@param id Identity int to pass to draw function.
		 */
		void calculate(int id);

	};
}

#endif