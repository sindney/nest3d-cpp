#ifndef RENDERPROCESS_H
#define RENDERPROCESS_H

#include "GL/glew.h"

namespace nest
{
	typedef struct 
	{
		GLuint frameBuffer;

		GLuint *renderBuffers;

		GLuint renderBufferSize;

		GLuint *textures;

		GLuint textureSize;

		GLint top;

		GLint left;

		GLint right;

		GLint bottom;

	} renderTarget;

	class renderprocess
	{
	public:

		renderTarget *target;

		virtual ~renderprocess() 
		{
			target = NULL;
		}

		virtual void calculate() = 0;

	};
}

#endif