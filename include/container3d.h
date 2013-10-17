#ifndef N3D_CONTAINER3D_H
#define N3D_CONTAINER3D_H

#include <vector>

#include "Object3d.h"

namespace nest
{
	class Container3d : public Object3d
	{
	public:

		std::vector<Object3d*> objects;

		bool castShadows;

		bool visible;

		Container3d() : castShadows(false), visible(true) {}

		~Container3d();

		void addChild(Object3d *object);

		/**
		 *	Remove child from partition tree first, then from it's container.
		 */
		void removeChild(Object3d *object);

		void recompose();
	};
}

#endif