#ifndef CONTAINER3D_H
#define CONTAINER3D_H

#include <iterator>
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

		~container3d()
		{
			objects.clear();
		}

		void recompose()
		{
			object3d::recompose();
			vector<object3d*>::iterator i;
			for(i = objects.begin(); i != objects.end(); i++)
			{
				(*i)->recompose();
			}
		}
	};
}

#endif