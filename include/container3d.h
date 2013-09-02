#ifndef N3D_CONTAINER3D_H
#define N3D_CONTAINER3D_H

#include <vector>

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

		~container3d();

		void addChild(object3d *object);

		/**
		 *	Remove child from partition tree first, then from it's container.
		 */
		void removeChild(object3d *object);

		void recompose();
	};
}

#endif