#ifndef CLERROR_H
#define CLERROR_H

#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>


class clError
{
private:
	const char * m_ClassName;

public:
	clError(const char * className);
	~clError();
	void AddError(const char * errorString, ...);
	void AddPlanText(const char * debugText);
	void AddDebug(const char * debugString, ...);
	bool checkForGlError(const char * debugString, ...);
	bool checkForGlShaderError(GLuint shaderProgram, const char * debugString, ...);
};

#endif