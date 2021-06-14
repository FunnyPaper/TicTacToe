#include <iostream>
#include "GLerror.h"

#include <GLFW\glfw3.h>

void GL_EmptyErrorStack()
{
	if(glfwGetCurrentContext())
	while (glGetError() != GL_NO_ERROR);
}

bool GL_ErroCallback(const char* functionName, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "OPENGL_ERROR: " << "(" << error << ")\nfunction: ("
			<< functionName << ")\nfile: (" << file << ")\nline: (" << line << ")\n-----------------------" << std::endl;
		return false;
	}
	return true;
}