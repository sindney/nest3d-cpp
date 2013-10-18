#ifndef N3D_CONTAINERRENDER_H
#define N3D_CONTAINERRENDER_H

#include <vector>

#include "Container3d.h"
#include "ObjectRender.h"

namespace nest
{
	/**
	 *	A ObjectRender that renders container3d tree.
	 */
	class ContainerRender : public ObjectRender
	{
	public:

		Container3d *root;

		/**
		 *	@param root Target container3d.
		 *	@param draw Draw function for those meshes who has no draw function.
		 *	@param camera View camera which stores view & projection matrices.
		 */
		ContainerRender(Container3d *root, ORDraw *draw, Camera3d *camera)
		 : root(root), ObjectRender(draw, camera) {}

		~ContainerRender()
		{
			if(root != NULL) delete root;
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