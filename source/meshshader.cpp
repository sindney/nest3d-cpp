#include "meshshader.h"

namespace nest
{
	meshshader::~meshshader()
	{
		if(program != 0)
		{
			glDetachShader(program, vertexShader);
			glDetachShader(program, fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteShader(program);
		}
		if(textures.size() > 0)
		{
			glDeleteTextures(textures.size(), &textures[0]);
			textures.clear();
		}
		shaderpart *part;
		while(parts.size() != 0)
		{
			part = parts.back();
			parts.pop_back();
			delete part;
		}
	}

	const GLchar meshshader::VERTEX_POSITION[] = "vertex_position";

	const GLchar meshshader::VERTEX_UV[] = "vertex_uv";

	const GLchar meshshader::VERTEX_NORMAL[] = "vertex_normal";

	const GLchar meshshader::VERTEX_TANGENT[] = "vertex_tangent";

	const GLchar meshshader::FRAGMENT_COLOR[] = "fragment_color";

	const GLchar meshshader::PROJECTION_MATRIX[] = "projection_matrix";

	const GLchar meshshader::INVERT_VIEW_MATRIX[] = "invert_view_matrix";

	const GLchar meshshader::WORLD_MATRIX[] = "world_matrix";

	const GLchar meshshader::TEXTURE_DIFFUSE[] = "texture_diffuse";

	const GLchar meshshader::TEXTURE_SPECULAR[] = "texture_specular";

	const GLchar meshshader::TEXTURE_NORMAL[] = "texture_normal";

	void meshshader::configure(meshshader &shader, const char *vertex, const char *fragment)
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
		glBindFragDataLocation(shader.program, 0, meshshader::FRAGMENT_COLOR);
		glLinkProgram(shader.program);
	}
}