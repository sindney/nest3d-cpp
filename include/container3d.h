#ifndef CONTAINER3D_H
#define CONTAINER3D_H

#include <iterator>
#include <vector>

#include "mesh.h"
#include "object3d.h"

namespace nest
{
	using namespace std;

	class container3d : public object3d
	{
	public:

		vector<object3d*> objects;

		bool castShadows;

		bool visible;

		container3d() : castShadows(false), visible(true) {}

		/**
		 *	The delocate function will delete target container's all child objects.
		 *	You can call container.objects.clear() before delete to avoid that process from happening.
		 *	Don't forget to call object.recompose() and set it's parent to NULL to reset his transform matrixes.
		 */
		~container3d();

		void addChild(object3d *node);

		object3d* removeChild(object3d *node);

		object3d* removeChildAt(vector<object3d*>::iterator index);

		void recompose();
	};
}

#endif