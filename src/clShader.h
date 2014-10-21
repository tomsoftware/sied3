#ifndef CLSHADER_H
#define CLSHADER_H


#define GLEW_STATIC
#include <GL/glew.h>

#include "clError.h"


class clShader
{

private:

	GLuint m_program;
	GLuint m_shader;
	clError m_error = clError("clShader");

public:
	clShader();
	~clShader();



	bool addFragmentShader(const char * ShaderCode);
	bool activate();
	void disable();
};


#endif