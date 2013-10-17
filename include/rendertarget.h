#ifndef N3D_RENDERTARGET_H
#define N3D_RENDERTARGET_H

#include <vector>

#include "GL/glew.h"

namespace nest
{
	class RenderTarget
	{
	public:

		GLuint frameBuffer;

		std::vector<GLuint> buffers;

		std::vector<GLuint> textures;

		GLint x;

		GLint y;

		GLsizei width;

		GLsizei height;

		RenderTarget(GLint x, GLint y, GLint width, GLint height, GLuint frameBuffer = 0)
		{
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->frameBuffer = frameBuffer;
		}

		~RenderTarget()
		{
			glDeleteTextures(textures.size(), &textures[0]);
			glDeleteRenderbuffers(buffers.size(), &buffers[0]);
			glDeleteFramebuffers(1, &frameBuffer);
			textures.clear();
			buffers.clear();
		}
	};
}

#endif