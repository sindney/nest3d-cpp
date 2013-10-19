#ifndef N3D_CONTAINERRENDER_H
#define N3D_CONTAINERRENDER_H

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
		 *	@param passed0 Stores those meshes with alphaTest off and passed specific culling function.
		 *	@param passed1 Stores those meshes with alphaTest on and passed specific culling function.
		 *	@param rejected Stores those meshes who didn't pass the culling function.
		 */
		void calculate(int id, std::vector<Mesh*> *passed0, std::vector<Mesh*> *passed1, std::vector<Mesh*> *rejected);

	};
}

#endif