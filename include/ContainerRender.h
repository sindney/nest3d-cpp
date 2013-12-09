#ifndef N3D_CONTAINERRENDER_H
#define N3D_CONTAINERRENDER_H

#include "Container3d.h"
#include "ObjectRender.h"

namespace nest
{
	/**
	 *	An ObjectRender that renders container3d tree.
	 */
	class ContainerRender : public ObjectRender
	{
	public:

		Container3d *root;

		/**
		 *	@param root Target container3d.
		 */
		ContainerRender(Container3d *root, Camera3d *camera)
		 : root(root), ObjectRender(camera) {}

		~ContainerRender()
		{
			if(root != NULL) delete root;
		}

		void calculate(int id, std::vector<Mesh*> *result0, std::vector<Mesh*> *result1, std::vector<Mesh*> *result2);

	};
}

#endif