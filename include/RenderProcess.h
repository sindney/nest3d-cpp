#ifndef N3D_RENDERPROCESS_H
#define N3D_RENDERPROCESS_H

#include "RenderTarget.h"

namespace nest
{
	/**
	 *	Extend this class to achieve a complete draw process.
	 *	
	 *	@see ObjectRender
	 *	@see ContainerRender
	 *	@see OcTreeRender
	 */
	class RenderProcess
	{
	public:

		RenderTarget *target;

		int numMeshes;

		int numTris;

		int numVts;

		RenderProcess(RenderTarget *target) : target(target), numMeshes(0), numTris(0), numVts(0) {}

		virtual ~RenderProcess()
		{
			if(target != NULL) delete target;
		}

		virtual void calculate() = 0;

	};
}

#endif