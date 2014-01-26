#ifndef N3D_CONTAINERPROCESSOR_H
#define N3D_CONTAINERPROCESSOR_H

#include "ObjectProcessor.h"

namespace nest
{
	class ContainerNode;

	class MeshRender;

	/**
	 *	A processor that processes meshes in container3d.
	 */
	class ContainerProcessor : public ObjectProcessor 
	{
	public:

		/**
		 *	You take care of this pointer's deletion.
		 */
		ContainerNode *root;

		/**
		 *	You take care of this pointer's deletion.
		 */
		MeshRender *render;

		ContainerProcessor(ContainerNode *root, MeshRender *render)
		 : root(root), render(render) {}

		virtual ~ContainerProcessor()
		{
			root = NULL;
			render = NULL;
		}

		virtual void calculate(CameraNode *camera, bool flag, std::vector<MeshNode*> *result0, std::vector<MeshNode*> *result1, std::vector<MeshNode*> *result2);

	};
}

#endif