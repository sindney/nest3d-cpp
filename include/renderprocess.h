#ifndef N3D_RENDERPROCESS_H
#define N3D_RENDERPROCESS_H

#include "rendertarget.h"

namespace nest
{
	class renderprocess
	{
	public:

		rendertarget *target;

		renderprocess(rendertarget *target) : target(target) {}

		virtual ~renderprocess()
		{
			if(target != NULL) delete target;
		}

		virtual void calculate() = 0;

	};
}

#endif