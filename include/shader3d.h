#ifndef N3D_SHADER3D_H
#define N3D_SHADER3D_H

#include <vector>

#include "GL/glew.h"

#include "shaderpart.h"

namespace nest
{
	class shader3d
	{
	public:

		GLuint program, vertexShader, fragmentShader;

		std::vector<GLuint> textures;

		std::vector<shaderpart*> parts;

		shader3d() : program(0), vertexShader(0), fragmentShader(0) {}

		~shader3d();

		static const GLchar SHADER_VERTEX_POSITION[];

		static const GLchar SHADER_VERTEX_UV[];

		static const GLchar SHADER_VERTEX_NORMAL[];

		static const GLchar SHADER_VERTEX_TANGENT[];

		static const GLchar SHADER_FRAGMENT_COLOR[];

		static const GLchar SHADER_PROJECTION_MATRIX[];

		static const GLchar SHADER_INVERT_VIEW_MATRIX[];

		static const GLchar SHADER_WORLD_MATRIX[];

		static const GLchar SHADER_TEXTURE_DIFFUSE[];

		static const GLchar SHADER_TEXTURE_SPECULAR[];

		static const GLchar SHADER_TEXTURE_NORMAL[];

		static void configure(shader3d &shader, const char *vertex, const char *fragment);
	};
}

#endif