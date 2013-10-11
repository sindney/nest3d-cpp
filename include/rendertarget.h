#ifndef N3D_RENDERTARGET_H
#define N3D_RENDERTARGET_H

#include "GL/glew.h"

namespace nest
{
	class rendertarget
	{
	public:

		GLuint frameBuffer;

		GLuint *renderBuffers;

		GLuint renderBufferSize;

		GLuint *textures;

		GLuint textureSize;

		GLint x;

		GLint y;

		GLsizei width;

		GLsizei height;

		rendertarget(GLint x, GLint y, GLint width, GLint height, GLuint frameBuffer = 0, 
					GLuint *renderBuffers = NULL, GLuint renderBufferSize = 0, 
					GLuint *textures = NULL, GLuint textureSize = 0)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->frameBuffer = frameBuffer;
			this->renderBuffers = renderBuffers;
			this->renderBufferSize = renderBufferSize;
			this->textures = textures;
			this->textureSize = textureSize;
		}

		~rendertarget()
		{
			glDeleteTextures(textureSize, textures);
			glDeleteRenderbuffers(renderBufferSize, renderBuffers);
			glDeleteFramebuffers(1, &frameBuffer);
			if(textures != NULL) delete [] textures;
			if(renderBuffers != NULL) delete [] renderBuffers;
		}
	};
}

#endif