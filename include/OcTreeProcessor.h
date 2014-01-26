#ifndef N3D_OCTREEPROCESSOR_H
#define N3D_OCTREEPROCESSOR_H

#include "ObjectProcessor.h"

namespace nest
{
	class OcTree;
	
	class MeshRender;

	/**
	 *	A processor that processes meshes in octree.
	 */
	class OcTreeProcessor : public ObjectProcessor 
	{
	public:

		/**
		 *	You take care of this pointer's deletion.
		 */
		OcTree *tree;

		/**
		 *	You take care of this pointer's deletion.
		 */
		MeshRender *render;

		OcTreeProcessor(OcTree *tree, MeshRender *render)
		 : tree(tree), render(render) {}

		virtual ~OcTreeProcessor()
		{
			tree = NULL;
			render = NULL;
		}

		virtual void calculate(CameraNode *camera, bool flag, std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2);
		
	};
}

#endif