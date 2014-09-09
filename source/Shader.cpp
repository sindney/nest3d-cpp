#include <iterator>

#include "Geometry.h"
#include "Shader.h"

namespace nest
{
	using namespace std;
	
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

		if(vao != 0) glDeleteVertexArrays(1, &vao);

		map<string, TextureInfo>::iterator it;
		TextureInfo tInfo;
		for(it = textureMap.begin(); it != textureMap.end(); ++it)
		{
			tInfo = it->second;
			if(tInfo.flag) glDeleteTextures(1, &tInfo.texture);
		}

		ShaderPart *part;
		while(parts.size() > 0)
		{
			part = parts.back();
			parts.pop_back();
			delete part;
		}
	}

	void Shader::create(int params, const char *vertex, const char *fragment)
	{
		if(program != 0)
		{
			glDetachShader(program, vertexShader);
			glDetachShader(program, fragmentShader);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			glDeleteProgram(program);
		}
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex, NULL);
		glCompileShader(vertexShader);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragment, NULL);
		glCompileShader(fragmentShader);
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		int offset = 0;
		if(params & GEOM_VERTEX) glBindAttribLocation(program, offset++, Shader::VERTEX_POSITION);
		if(params & GEOM_UV) glBindAttribLocation(program, offset++, Shader::VERTEX_UV);
		if(params & GEOM_NORMAL) glBindAttribLocation(program, offset++, Shader::VERTEX_NORMAL);
		if(params & GEOM_TANGENT) glBindAttribLocation(program, offset++, Shader::VERTEX_TANGENT);
		if(params & GEOM_INDICES) glBindAttribLocation(program, offset++, Shader::VERTEX_INDICES);
		if(params & GEOM_WEIGHTS) glBindAttribLocation(program, offset++, Shader::VERTEX_WEIGHTS);
		glLinkProgram(program);
	}

	bool Shader::bindTexture(string name, GLuint texture, GLenum target, bool flag)
	{
		TextureInfo tInfo = {texture, target, flag};
		return bindTexture(name, tInfo);
	}

	bool Shader::bindTexture(std::string name, TextureInfo tInfo)
	{
		pair<map<string, TextureInfo>::iterator, bool> it;
		it = textureMap.insert(map<string, TextureInfo>::value_type(name, tInfo));
		return it.second;
	}

	bool Shader::unbindTexture(string name, TextureInfo *tInfo)
	{
		map<string, TextureInfo>::iterator it;
		it = textureMap.find(name);
		if(it != textureMap.end())
		{
			if(tInfo) *tInfo = it->second;
			textureMap.erase(it);
			return true;
		}
		return false;
	}

	const GLchar Shader::VERTEX_POSITION[] = "vertex_position";

	const GLchar Shader::VERTEX_UV[] = "vertex_uv";

	const GLchar Shader::VERTEX_NORMAL[] = "vertex_normal";

	const GLchar Shader::VERTEX_TANGENT[] = "vertex_tangent";

	const GLchar Shader::VERTEX_INDICES[] = "vertex_indices";

	const GLchar Shader::VERTEX_WEIGHTS[] = "vertex_weights";

	const GLchar Shader::SKELETON[] = "skeleton";

	const GLchar Shader::COMBINED_MATRIX[] = "combined_matrix";

	const GLchar Shader::PROJECTION_MATRIX[] = "projection_matrix";

	const GLchar Shader::VIEW_MATRIX[] = "view_matrix";

	const GLchar Shader::INVERT_VIEW_MATRIX[] = "invert_view_matrix";

	const GLchar Shader::WORLD_MATRIX[] = "world_matrix";

	const GLchar Shader::INVERT_WORLD_MATRIX[] = "invert_world_matrix";
}