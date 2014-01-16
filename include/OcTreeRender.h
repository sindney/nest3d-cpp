#ifndef N3D_OCTREERENDER_H
#define N3D_OCTREERENDER_H

#include "ObjectRender.h"

namespace nest
{
	class OcTree;
	
	/**
	 *	A render that renders meshes in octree.
	 */
	class OcTreeRender : public ObjectRender 
	{
	public:

		/**
		 *	You take care of this pointer's deletion.
		 */
		OcTree *tree;

		OcTreeRender(OcTree *tree) : tree(tree) {}

		~OcTreeRender()
		{
			tree = NULL;
		}

		void draw(Camera3d *camera, int id, std::vector<Mesh*> *result0, std::vector<Mesh*> *result1, std::vector<Mesh*> *result2);
		
	};
}

#endif