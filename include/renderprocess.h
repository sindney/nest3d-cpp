#ifndef RENDERPROCESS_H
#define RENDERPROCESS_H

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