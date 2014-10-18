#include "clError.h"
#include <stdarg.h> 

clError::clError(const char * className)
{
	m_ClassName = className;
}

//-------------------------------------//
void clError::AddError(const char * errorString, ...)
{
	va_list args;
	va_start(args, errorString);

	printf("!Error: [%s] ", m_ClassName);
	vprintf(errorString, args);
	printf("\n");

	va_end(args);
}


//-------------------------------------//
void clError::AddPlanText(const char * debugText)
{
	printf("%s", debugText);
	printf("\n");
}


//-------------------------------------//
void clError::AddDebug(const char * debugString, ...)
{
	va_list args;
	va_start(args, debugString);

	printf(" Debug: [%s] ", m_ClassName);
	vprintf(debugString, args);
	printf("\n");

	va_end(args);
}


//-------------------------------------//
clError::~clError()
{
}



//-------------------------------------//
bool clError::checkForGlError(const char * debugString, ...)
{

	int err = glGetError();

	if (err == GL_NO_ERROR) return false;

	va_list args;
	va_start(args, debugString);


	const char * GL_Error_text = "";
	switch (err)
	{
		case GL_INVALID_ENUM: GL_Error_text = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE: GL_Error_text = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: GL_Error_text = "GL_INVALID_OPERATION"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: GL_Error_text = "GL_INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY: GL_Error_text = "GL_OUT_OF_MEMORY"; break;
		case GL_STACK_UNDERFLOW: GL_Error_text = "GL_STACK_UNDERFLOW"; break;
		case GL_STACK_OVERFLOW: GL_Error_text = "GL_STACK_OVERFLOW"; break;
	}


	printf("!GlError: [%s] { %i [%s] }", m_ClassName, err, GL_Error_text);
	vprintf(debugString, args);
	printf("\n");

	va_end(args);


	return true;
}




//-------------------------------------//
bool clError::checkForGlShaderError(GLuint shaderProgram, const char * debugString, ...)
{
	static char GL_Error_text[1024];


	int  bufferLen = 0;
	glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &bufferLen);

	if (bufferLen <= 1) return false; //- no error found.
	
	va_list args;
	va_start(args, debugString);


	//- if data is to large... cut it
	if (bufferLen > sizeof(GL_Error_text)) bufferLen = sizeof(GL_Error_text);

	//- read log
	glGetShaderInfoLog(shaderProgram, sizeof(GL_Error_text), &bufferLen, GL_Error_text);

	//- toDo: clear log


	printf("!GlShaderError: [%s] ", m_ClassName);
	vprintf(debugString, args);
	printf("\n::%s\n", GL_Error_text);

	va_end(args);

	return true;
}