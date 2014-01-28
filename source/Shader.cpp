#include "Shader.h"

namespace nest
{
	Shader::~Shader()
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
		ShaderPart *part;
		while(parts.size() != 0)
		{
			part = parts.back();
			parts.pop_back();
			delete part;
		}
	}

	const GLchar Shader::VERTEX_POSITION[] = "vertex_position";

	const GLchar Shader::VERTEX_UV[] = "vertex_uv";

	const GLchar Shader::VERTEX_NORMAL[] = "vertex_normal";

	const GLchar Shader::VERTEX_TANGENT[] = "vertex_tangent";

	const GLchar Shader::VERTEX_INDICES[] = "vertex_indices";

	const GLchar Shader::VERTEX_WEIGHTS[] = "vertex_weights";

	const GLchar Shader::FRAGMENT_COLOR[] = "fragment_color";

	const GLchar Shader::SKELETON[] = "skeleton";

	const GLchar Shader::PROJECTION_MATRIX[] = "projection_matrix";

	const GLchar Shader::VIEW_MATRIX[] = "view_matrix";

	const GLchar Shader::INVERT_VIEW_MATRIX[] = "invert_view_matrix";

	const GLchar Shader::WORLD_MATRIX[] = "world_matrix";

	const GLchar Shader::INVERT_WORLD_MATRIX[] = "invert_world_matrix";

	const GLchar Shader::TEXTURE_DIFFUSE[] = "texture_diffuse";

	const GLchar Shader::TEXTURE_SPECULAR[] = "texture_specular";

	const GLchar Shader::TEXTURE_NORMAL[] = "texture_normal";

	void Shader::configure(Shader *shader, const char *vertex, const char *fragment)
	{
		if(shader->program != 0)
		{
			glDetachShader(shader->program, shader->vertexShader);
			glDetachShader(shader->program, shader->fragmentShader);
			glDeleteShader(shader->vertexShader);
			glDeleteShader(shader->fragmentShader);
			glDeleteShader(shader->program);
		}
		shader->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader->vertexShader, 1, &vertex, NULL);
		glCompileShader(shader->vertexShader);
		shader->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader->fragmentShader, 1, &fragment, NULL);
		glCompileShader(shader->fragmentShader);
		shader->program = glCreateProgram();
		glAttachShader(shader->program, shader->vertexShader);
		glAttachShader(shader->program, shader->fragmentShader);
		glBindFragDataLocation(shader->program, 0, Shader::FRAGMENT_COLOR);
		glLinkProgram(shader->program);
	}
}