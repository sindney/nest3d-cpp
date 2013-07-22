#include "GL/glew.h"

#include "shader3d.h"

namespace nest
{
	shader3d::~shader3d()
	{
		if(program != 0)
		{
			glDetachShader(program, vertexShader);
			glDetachShader(program, fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteShader(program);
		}
	}

	const GLchar shader3d::SHADER_VERTEX_POSITION[] = "vertex_position";

	const GLchar shader3d::SHADER_VERTEX_UV[] = "vertex_uv";

	const GLchar shader3d::SHADER_VERTEX_NORMAL[] = "vertex_normal";

	const GLchar shader3d::SHADER_VERTEX_TANGENT[] = "vertex_tangent";

	const GLchar shader3d::SHADER_FRAGMENT_COLOR[] = "fragment_color";

	const GLchar shader3d::SHADER_PROJECTION_MATRIX[] = "projection_matrix";

	const GLchar shader3d::SHADER_INVERT_VIEW_MATRIX[] = "invert_view_matrix";

	const GLchar shader3d::SHADER_WORLD_MATRIX[] = "world_matrix";

	void shader3d::setupShader(shader3d &shader, const char *vertex, const char *fragment)
	{
		if(shader.program != 0)
		{
			glDetachShader(shader.program, shader.vertexShader);
			glDetachShader(shader.program, shader.fragmentShader);
			glDeleteShader(shader.vertexShader);
			glDeleteShader(shader.fragmentShader);
			glDeleteShader(shader.program);
		}
		shader.vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader.vertexShader, 1, &vertex, NULL);
		glCompileShader(shader.vertexShader);
		shader.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader.fragmentShader, 1, &fragment, NULL);
		glCompileShader(shader.fragmentShader);
		shader.program = glCreateProgram();
		glAttachShader(shader.program, shader.vertexShader);
		glAttachShader(shader.program, shader.fragmentShader);
		glBindFragDataLocation(shader.program, 0, shader3d::SHADER_FRAGMENT_COLOR);
		glLinkProgram(shader.program);
	}
}