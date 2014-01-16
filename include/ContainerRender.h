#ifndef N3D_CONTAINERRENDER_H
#define N3D_CONTAINERRENDER_H

#include "ObjectRender.h"

namespace nest
{
	class Container3d;

	/**
	 *	A render that renders meshes in container3d.
	 */
	class ContainerRender : public ObjectRender 
	{
	public:

		/**
		 *	You take care of this pointer's deletion.
		 */
		Container3d *root;

		ContainerRender(Container3d *root) : root(root) {}

		~ContainerRender()
		{
			root = NULL;
		}

		void draw(Camera3d *camera, int id, std::vector<Mesh*> *result0, std::vector<Mesh*> *result1, std::vector<Mesh*> *result2);

	};
}

#endif