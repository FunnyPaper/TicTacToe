#include "GLerror.h"
#include <iostream>

#include <GLFW\glfw3.h>

void GL_EmptyErrorStack()
{
	// Je�li jest otwarte okno kotekstowe to z�ap i przepu�� wszystkie b��dy
	if(glfwGetCurrentContext())
	while (glGetError() != GL_NO_ERROR);
}

bool GL_ErroCallback(const char* functionName, const char* file, int line)
{
	// �ap b��dy ze stosu b��d�w OpenGL
	// W rezultacie we� pierwszy ze stosu i zwr�� false (bierze tylko jeden b��d)
	while (GLenum error = glGetError())
	{
		std::cout << "OPENGL_ERROR: " << "(" << error << ")\nfunction: ("
			<< functionName << ")\nfile: (" << file << ")\nline: (" << line << ")\n-----------------------" << std::endl;
		return false;
	}
	return true;
}