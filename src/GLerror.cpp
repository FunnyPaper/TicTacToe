#include "GLerror.h"
#include <iostream>

#include <GLFW\glfw3.h>

void GL_EmptyErrorStack()
{
	// Jeœli jest otwarte okno kotekstowe to z³ap i przepuœæ wszystkie b³êdy
	if(glfwGetCurrentContext())
	while (glGetError() != GL_NO_ERROR);
}

bool GL_ErroCallback(const char* functionName, const char* file, int line)
{
	// £ap b³êdy ze stosu b³êdów OpenGL
	// W rezultacie weŸ pierwszy ze stosu i zwróæ false (bierze tylko jeden b³¹d)
	while (GLenum error = glGetError())
	{
		std::cout << "OPENGL_ERROR: " << "(" << error << ")\nfunction: ("
			<< functionName << ")\nfile: (" << file << ")\nline: (" << line << ")\n-----------------------" << std::endl;
		return false;
	}
	return true;
}