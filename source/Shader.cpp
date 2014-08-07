#include "Shader.h"
#include "Geometry.h"

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
			glDeleteProgram(program);
		}
		if(vao != 0)
			glDeleteVertexArrays(1, &vao);
		int size = textures.size();
		if(size > 0)
		{
			int i;
			for(i = 0; i < size; i++)
				if(textureParams[i])
					glDeleteTextures(1, &textures[i]);
		}
		ShaderPart *part;
		while(parts.size() != 0)
		{
			part = parts.back();
			parts.pop_back();
			delete part;
		}
	}

	void Shader::linkTexture(GLuint texture, string name, bool autoDelete)
	{
		textures.push_back(texture);
		textureNames.push_back(name);
		textureParams.push_back(autoDelete);
	}

	const GLchar Shader::VERTEX_POSITION[] = "vertex_position";

	const GLchar Shader::VERTEX_UV[] = "vertex_uv";

	const GLchar Shader::VERTEX_NORMAL[] = "vertex_normal";

	const GLchar Shader::VERTEX_TANGENT[] = "vertex_tangent";

	const GLchar Shader::VERTEX_INDICES[] = "vertex_indices";

	const GLchar Shader::VERTEX_WEIGHTS[] = "vertex_weights";

	const GLchar Shader::FRAGMENT_OUTPUT[] = "fragment_output";

	const GLchar Shader::SKELETON[] = "skeleton";

	const GLchar Shader::COMBINED_MATRIX[] = "combined_matrix";

	const GLchar Shader::PROJECTION_MATRIX[] = "projection_matrix";

	const GLchar Shader::VIEW_MATRIX[] = "view_matrix";

	const GLchar Shader::INVERT_VIEW_MATRIX[] = "invert_view_matrix";

	const GLchar Shader::WORLD_MATRIX[] = "world_matrix";

	const GLchar Shader::INVERT_WORLD_MATRIX[] = "invert_world_matrix";

	void Shader::configure(Shader *shader, int params, const char *vertex, const char *fragment)
	{
		if(shader->program != 0)
		{
			glDetachShader(shader->program, shader->vertexShader);
			glDetachShader(shader->program, shader->fragmentShader);
			glDeleteShader(shader->vertexShader);
			glDeleteShader(shader->fragmentShader);
			glDeleteProgram(shader->program);
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
		int count = 0;
		if(params & GEOM_VERTEX) glBindAttribLocation(shader->program, count++, Shader::VERTEX_POSITION);
		if(params & GEOM_UV) glBindAttribLocation(shader->program, count++, Shader::VERTEX_UV);
		if(params & GEOM_NORMAL) glBindAttribLocation(shader->program, count++, Shader::VERTEX_NORMAL);
		if(params & GEOM_TANGENT) glBindAttribLocation(shader->program, count++, Shader::VERTEX_TANGENT);
		if(params & GEOM_INDICES) glBindAttribLocation(shader->program, count++, Shader::VERTEX_INDICES);
		if(params & GEOM_WEIGHTS) glBindAttribLocation(shader->program, count++, Shader::VERTEX_WEIGHTS);
		glBindFragDataLocation(shader->program, 0, Shader::FRAGMENT_OUTPUT);
		glLinkProgram(shader->program);
	}
}