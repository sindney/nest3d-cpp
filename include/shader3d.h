#ifndef SHADER3D_H
#define SHADER3D_H

namespace nest
{
	class shader3d
	{
	public:

		static const GLchar SHADER_VERTEX_POSITION[];

		static const GLchar SHADER_VERTEX_UV[];

		static const GLchar SHADER_VERTEX_NORMAL[];

		static const GLchar SHADER_VERTEX_TANGENT[];

		static const GLchar SHADER_FRAGMENT_COLOR[];

		static const GLchar SHADER_PROJECTION_MATRIX[];

		static const GLchar SHADER_INVERT_VIEW_MATRIX[];

		static const GLchar SHADER_WORLD_MATRIX[];

		GLuint program, vertexShader, fragmentShader;

		shader3d() : program(0), vertexShader(0), fragmentShader(0) {}

		~shader3d();

		static void setupShader(shader3d &shader, const char *vertex, const char *fragment);
	};
}

#endif