#include "clShader.h"


//-------------------------------------//
clShader::clShader()
{
	m_program = 0;
	m_shader = 0;
}


//-------------------------------------//
clShader::~clShader()
{
	if (m_program != 0)
	{
		glDeleteProgram(m_program);
	}
	
}




//-------------------------------------//
bool clShader::addFragmentShader(const char * ShaderCode)
{
	//- if no Shader Program is created yet: create one!
	if (m_program == 0) m_program = glCreateProgram();



	m_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_shader, 1, &ShaderCode, 0);

	glCompileShader(m_shader);
	m_error.checkForGlShaderError(m_shader, "glCompileShader(m_shader)");


	glAttachShader(m_program, m_shader);
	m_error.checkForGlError("glAttachShader(s_program, m_shader)");


	glLinkProgram(m_program);
	m_error.checkForGlShaderError(m_shader, "glAttachShader(m_shader_Program, m_pixle_shader)");


	return true;
}


//-------------------------------------//
bool clShader::activate()
{
	glUseProgram(m_program);
	return true;
}


//-------------------------------------//
void clShader::disable()
{
	glUseProgram(0);
}