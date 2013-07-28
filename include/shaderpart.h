#ifndef SHADERPART_H
#define SHADERPART_H

#include "GL/glew.h"

namespace nest
{

	class shaderpart
	{
	public:

		virtual ~shaderpart() {};

		virtual void upload() = 0;

	};

	// single float

	class shaderpart1f : public shaderpart
	{
	public:

		GLint location;

		GLfloat v0;

		shaderpart1f(GLint location, GLfloat v0) : location(location), v0(v0) {}

		void upload()
		{
			glUniform1f(location, v0);
		}
	};

	class shaderpart2f : public shaderpart
	{
	public:

		GLint location;
		
		GLfloat v0, v1, v2;

		shaderpart2f(GLint location, GLfloat v0, GLfloat v1) : location(location), v0(v0), v1(v1) {}

		void upload()
		{
			glUniform2f(location, v0, v1);
		}
	};

	class shaderpart3f : public shaderpart
	{
	public:

		GLint location;
		
		GLfloat v0, v1, v2;

		shaderpart3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) : location(location), v0(v0), v1(v1), v2(v2) {}

		void upload()
		{
			glUniform3f(location, v0, v1, v2);
		}
	};

	class shaderpart4f : public shaderpart
	{
	public:

		GLint location;
		
		GLfloat v0, v1, v2, v3;

		shaderpart4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) : location(location), v0(v0), v1(v1), v2(v2), v3(v3) {}

		void upload()
		{
			glUniform4f(location, v0, v1, v2, v3);
		}
	};

	// single int

	class shaderpart1i : public shaderpart
	{
	public:

		GLint location;
		
		GLint v0;

		shaderpart1i(GLint location, GLint v0) : location(location), v0(v0) {}

		void upload()
		{
			glUniform1i(location, v0);
		}
	};

	class shaderpart2i : public shaderpart
	{
	public:

		GLint location;
		
		GLint v0, v1, v2;

		shaderpart2i(GLint location, GLint v0, GLint v1) : location(location), v0(v0), v1(v1) {}

		void upload()
		{
			glUniform2i(location, v0, v1);
		}
	};

	class shaderpart3i : public shaderpart
	{
	public:

		GLint location;
		
		GLint v0, v1, v2;

		shaderpart3i(GLint location, GLint v0, GLint v1, GLint v2) : location(location), v0(v0), v1(v1), v2(v2) {}

		void upload()
		{
			glUniform3i(location, v0, v1, v2);
		}
	};

	class shaderpart4i : public shaderpart
	{
	public:

		GLint location;
		
		GLint v0, v1, v2, v3;

		shaderpart4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) : location(location), v0(v0), v1(v1), v2(v2), v3(v3) {}

		void upload()
		{
			glUniform4i(location, v0, v1, v2, v3);
		}
	};

	// single unsigned int

	class shaderpart1ui : public shaderpart
	{
	public:

		GLint location;
		
		GLuint v0;

		shaderpart1ui(GLint location, GLuint v0) : location(location), v0(v0) {}

		void upload()
		{
			glUniform1ui(location, v0);
		}
	};

	class shaderpart2ui : public shaderpart
	{
	public:

		GLint location;
		
		GLuint v0, v1, v2;

		shaderpart2ui(GLint location, GLuint v0, GLuint v1) : location(location), v0(v0), v1(v1) {}

		void upload()
		{
			glUniform2ui(location, v0, v1);
		}
	};

	class shaderpart3ui : public shaderpart
	{
	public:

		GLint location;
		
		GLuint v0, v1, v2;

		shaderpart3ui(GLint location, GLuint v0, GLuint v1, GLuint v2) : location(location), v0(v0), v1(v1), v2(v2) {}

		void upload()
		{
			glUniform3ui(location, v0, v1, v2);
		}
	};

	class shaderpart4ui : public shaderpart
	{
	public:

		GLint location;
		
		GLuint v0, v1, v2, v3;

		shaderpart4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3) : location(location), v0(v0), v1(v1), v2(v2), v3(v3) {}

		void upload()
		{
			glUniform4ui(location, v0, v1, v2, v3);
		}
	};

	// float array

	class shaderpartfv : public shaderpart
	{
	public:

		GLint location;
		
		unsigned int per;

		GLfloat *value;

		GLsizei count;

		shaderpartfv(GLint location, unsigned int per, GLsizei count, GLfloat *value) : location(location), per(per), count(count), value(value) {}

		~shaderpartfv()
		{
			if(value != NULL) delete [] value;
		}

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

	class shaderpartiv : public shaderpart
	{
	public:

		GLint location;
		
		unsigned int per;

		GLint *value;

		GLsizei count;

		shaderpartiv(GLint location, unsigned int per, GLsizei count, GLint *value) : location(location), per(per), count(count), value(value) {}

		~shaderpartiv()
		{
			if(value != NULL) delete [] value;
		}

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

	// unsigned array
	
	class shaderpartuiv : public shaderpart
	{
	public:

		GLint location;
		
		unsigned int per;

		GLuint *value;

		GLsizei count;

		shaderpartuiv(GLint location, unsigned int per, GLsizei count, GLuint *value) : location(location), per(per), count(count), value(value) {}

		~shaderpartuiv()
		{
			if(value != NULL) delete [] value;
		}

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

	class shaderpartmatrix : public shaderpart
	{
	public:

		GLint location;
		
		unsigned int w, h;

		GLfloat *value;

		GLsizei count;

		GLboolean transpose;

		shaderpartmatrix(GLint location, unsigned int w, unsigned int h, GLsizei count, GLboolean transpose, GLfloat *value)
		 : location(location), w(w), h(h), count(count), transpose(transpose), value(value) {}

		~shaderpartmatrix()
		{
			if(value != NULL) delete [] value;
		}

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
}

#endif