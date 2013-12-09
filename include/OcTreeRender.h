#ifndef N3D_OCTREERENDER_H
#define N3D_OCTREERENDER_H

#include "OcTree.h"
#include "ObjectRender.h"

namespace nest
{
	/**
	 *	An ObjectRender that renders objects stored in octree.
	 */
	class OcTreeRender : public ObjectRender
	{
	public:

		OcTree *tree;

		/**
		 *	@param tree Target octree.
		 */
		OcTreeRender(OcTree *tree, Camera3d *camera)
		 : tree(tree), ObjectRender(camera) {}

		~OcTreeRender()
		{
			if(tree != NULL) delete tree;
		}

		void calculate(int id, std::vector<Mesh*> *result0, std::vector<Mesh*> *result1, std::vector<Mesh*> *result2);

	};
}

#endif