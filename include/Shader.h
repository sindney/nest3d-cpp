#ifndef N3D_SHADER_H
#define N3D_SHADER_H

#include <map>
#include <string>
#include <vector>

#ifndef GLEW_INCLUDED
#define GLEW_INCLUDED
#include "GL/glew.h"
#endif

namespace nest
{
	class ShaderPart
	{
	public:

		virtual ~ShaderPart() {};

		virtual void upload() = 0;

	};

	// single float

	class ShaderPart1f : public ShaderPart
	{
	public:

		GLint location;

		GLfloat *v0;

		ShaderPart1f(GLint location, GLfloat *v0) : location(location), v0(v0) {}

		void upload()
		{
			glUniform1f(location, *v0);
		}
	};

	class ShaderPart2f : public ShaderPart
	{
	public:

		GLint location;
		
		GLfloat *v0, *v1;

		ShaderPart2f(GLint location, GLfloat *v0, GLfloat *v1)
		 : location(location), v0(v0), v1(v1) {}

		void upload()
		{
			glUniform2f(location, *v0, *v1);
		}
	};

	class ShaderPart3f : public ShaderPart
	{
	public:

		GLint location;
		
		GLfloat *v0, *v1, *v2;

		ShaderPart3f(GLint location, GLfloat *v0, GLfloat *v1, GLfloat *v2)
		 : location(location), v0(v0), v1(v1), v2(v2) {}

		void upload()
		{
			glUniform3f(location, *v0, *v1, *v2);
		}
	};

	class ShaderPart4f : public ShaderPart
	{
	public:

		GLint location;
		
		GLfloat *v0, *v1, *v2, *v3;

		ShaderPart4f(GLint location, GLfloat *v0, GLfloat *v1, GLfloat *v2, GLfloat *v3)
		 : location(location), v0(v0), v1(v1), v2(v2), v3(v3) {}

		void upload()
		{
			glUniform4f(location, *v0, *v1, *v2, *v3);
		}
	};

	// single int

	class ShaderPart1i : public ShaderPart
	{
	public:

		GLint location;
		
		GLint *v0;

		ShaderPart1i(GLint location, GLint *v0) : location(location), v0(v0) {}

		void upload()
		{
			glUniform1i(location, *v0);
		}
	};

	class ShaderPart2i : public ShaderPart
	{
	public:

		GLint location;
		
		GLint *v0, *v1;

		ShaderPart2i(GLint location, GLint *v0, GLint *v1)
		 : location(location), v0(v0), v1(v1) {}

		void upload()
		{
			glUniform2i(location, *v0, *v1);
		}
	};

	class ShaderPart3i : public ShaderPart
	{
	public:

		GLint location;
		
		GLint *v0, *v1, *v2;

		ShaderPart3i(GLint location, GLint *v0, GLint *v1, GLint *v2)
		 : location(location), v0(v0), v1(v1), v2(v2) {}

		void upload()
		{
			glUniform3i(location, *v0, *v1, *v2);
		}
	};

	class ShaderPart4i : public ShaderPart
	{
	public:

		GLint location;
		
		GLint *v0, *v1, *v2, *v3;

		ShaderPart4i(GLint location, GLint *v0, GLint *v1, GLint *v2, GLint *v3)
		 : location(location), v0(v0), v1(v1), v2(v2), v3(v3) {}

		void upload()
		{
			glUniform4i(location, *v0, *v1, *v2, *v3);
		}
	};

	// single unsigned int

	class ShaderPart1ui : public ShaderPart
	{
	public:

		GLint location;
		
		GLuint *v0;

		ShaderPart1ui(GLint location, GLuint *v0) : location(location), v0(v0) {}

		void upload()
		{
			glUniform1ui(location, *v0);
		}
	};

	class ShaderPart2ui : public ShaderPart
	{
	public:

		GLint location;
		
		GLuint *v0, *v1;

		ShaderPart2ui(GLint location, GLuint *v0, GLuint *v1)
		 : location(location), v0(v0), v1(v1) {}

		void upload()
		{
			glUniform2ui(location, *v0, *v1);
		}
	};

	class ShaderPart3ui : public ShaderPart
	{
	public:

		GLint location;
		
		GLuint *v0, *v1, *v2;

		ShaderPart3ui(GLint location, GLuint *v0, GLuint *v1, GLuint *v2)
		 : location(location), v0(v0), v1(v1), v2(v2) {}

		void upload()
		{
			glUniform3ui(location, *v0, *v1, *v2);
		}
	};

	class ShaderPart4ui : public ShaderPart
	{
	public:

		GLint location;
		
		GLuint *v0, *v1, *v2, *v3;

		ShaderPart4ui(GLint location, GLuint *v0, GLuint *v1, GLuint *v2, GLuint *v3)
		 : location(location), v0(v0), v1(v1), v2(v2), v3(v3) {}

		void upload()
		{
			glUniform4ui(location, *v0, *v1, *v2, *v3);
		}
	};

	// float array

	class ShaderPartfv : public ShaderPart
	{
	public:

		GLint location;
		
		unsigned int per;

		GLfloat *value;

		GLsizei count;

		ShaderPartfv(GLint location, unsigned int per, GLsizei count, GLfloat *value)
		 : location(location), per(per), count(count), value(value) {}

		void upload()
		{
			switch(per)
			{
				case 1:
					glUniform1fv(location, count, value);
					break;
				case 2:
					glUniform2fv(location, count, value);
					break;
				case 3:
					glUniform3fv(location, count, value);
					break;
				case 4:
					glUniform4fv(location, count, value);
					break;
			}
		}
	};

	// int array

	class ShaderPartiv : public ShaderPart
	{
	public:

		GLint location;
		
		unsigned int per;

		GLint *value;

		GLsizei count;

		ShaderPartiv(GLint location, unsigned int per, GLsizei count, GLint *value)
		 : location(location), per(per), count(count), value(value) {}

		void upload()
		{
			switch(per)
			{
				case 1:
					glUniform1iv(location, count, value);
					break;
				case 2:
					glUniform2iv(location, count, value);
					break;
				case 3:
					glUniform3iv(location, count, value);
					break;
				case 4:
					glUniform4iv(location, count, value);
					break;
			}
		}
	};

	// unsigned int array
	
	class ShaderPartuiv : public ShaderPart
	{
	public:

		GLint location;
		
		unsigned int per;

		GLuint *value;

		GLsizei count;

		ShaderPartuiv(GLint location, unsigned int per, GLsizei count, GLuint *value)
		 : location(location), per(per), count(count), value(value) {}

		void upload()
		{
			switch(per)
			{
				case 1:
					glUniform1uiv(location, count, value);
					break;
				case 2:
					glUniform2uiv(location, count, value);
					break;
				case 3:
					glUniform3uiv(location, count, value);
					break;
				case 4:
					glUniform4uiv(location, count, value);
					break;
			}
		}
	};

	// matrix

	class ShaderPartMatrix : public ShaderPart
	{
	public:

		GLint location;
		
		unsigned int w, h;

		GLfloat *value;

		GLsizei count;

		GLboolean transpose;

		ShaderPartMatrix(GLint location, unsigned int w, unsigned int h, GLsizei count, GLboolean transpose, GLfloat *value)
		 : location(location), w(w), h(h), count(count), transpose(transpose), value(value) {}

		void upload()
		{
			switch(w)
			{
				case 2:
					switch(h)
					{
						case 2:
							glUniformMatrix2fv(location, count, transpose, value);
							break;
						case 3:
							glUniformMatrix2x3fv(location, count, transpose, value);
							break;
						case 4:
							glUniformMatrix2x4fv(location, count, transpose, value);
							break;
					}
					break;
				case 3:
					switch(h)
					{
						case 2:
							glUniformMatrix3x2fv(location, count, transpose, value);
							break;
						case 3:
							glUniformMatrix3fv(location, count, transpose, value);
							break;
						case 4:
							glUniformMatrix3x4fv(location, count, transpose, value);
							break;
					}
					break;
				case 4:
					switch(h)
					{
						case 2:
							glUniformMatrix4x2fv(location, count, transpose, value);
							break;
						case 3:
							glUniformMatrix4x3fv(location, count, transpose, value);
							break;
						case 4:
							glUniformMatrix4fv(location, count, transpose, value);
							break;
					}
					break;
			}
		}
	};

	/**
	 *	See glBindTexture(target, texture).
	 */
	typedef struct
	{
		GLuint texture;
		
		GLenum target;

		/**
		 *	Delocation flag. True for auto delete.
		 */
		bool flag;

	} TextureInfo;

	class Shader
	{
	public:

		GLuint program, vertexShader, fragmentShader;

		/**
		 *	VertexArrayBuffer object. Use Geometry.bindVBOtoVAO() to bind vbos to it.
		 *	<p>You'd call createVAO() first.</p>
		 *	
		 *	@see Geometry
		 */
		GLuint vao;

		std::map<std::string, TextureInfo> textureMap;

		std::vector<ShaderPart*> parts;

		Shader() : program(0), vertexShader(0), fragmentShader(0) {}

		~Shader();

		void createVAO()
		{
			if(vao != 0)
				glDeleteVertexArrays(1, &vao);
			glGenVertexArrays(1, &vao);
		}

		/**
		 *	Bind texture to this shader. So we can use in in glsl.
		 *	See glBindTexture(target, texture).
		 *	
		 *	@flag Delocation flag. True for auto delete.
		 *	
		 *	@return Returns false when there's a name conflict.
		 */
		bool bindTexture(std::string name, GLuint texture, GLenum target, bool flag = true);

		bool bindTexture(std::string name, TextureInfo tInfo);

		bool unbindTexture(std::string name, TextureInfo *tInfo = NULL);

		static const GLchar VERTEX_POSITION[];

		static const GLchar VERTEX_UV[];

		static const GLchar VERTEX_NORMAL[];

		static const GLchar VERTEX_TANGENT[];

		static const GLchar VERTEX_INDICES[];

		static const GLchar VERTEX_WEIGHTS[];

		static const GLchar SKELETON[];

		static const GLchar COMBINED_MATRIX[];

		static const GLchar PROJECTION_MATRIX[];

		static const GLchar VIEW_MATRIX[];

		static const GLchar INVERT_VIEW_MATRIX[];

		static const GLchar WORLD_MATRIX[];

		static const GLchar INVERT_WORLD_MATRIX[];

		/**
		 *	@param params see Geometry.configure(int)
		 */
		static void configure(Shader *shader, int params, const char *vertex, const char *fragment);
	};
}

#endif