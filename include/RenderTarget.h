#ifndef N3D_RENDERTARGET_H
#define N3D_RENDERTARGET_H

#include <string>
#include <vector>

#include "GL/glew.h"

namespace nest
{
	/**
	 *	When we draw a RenderTarget, we pass it's flag to MeshRender's calculate function to have different render behavior.
	 *	@see MeshRender
	 */
	class RenderTarget
	{
	public:

		GLuint frameBuffer;

		std::vector<GLuint> buffers, textures;

		GLint x, y;

		GLsizei width, height;

		std::string flag;

		RenderTarget(std::string flag, GLint x, GLint y, GLint width, GLint height, GLuint frameBuffer = 0)
		{
			this->flag = flag;
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