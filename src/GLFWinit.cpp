#include "GLFWinit.h"
#include <iostream>

GLFWinit::GLFWinit(unsigned int width, unsigned int height, const char* window_title, unsigned int major_version, unsigned int minor_version, unsigned int profile, bool is_resizeable)
{
	//Zainicjuj GLFW
	if (!glfwInit())
	{
		std::cout << "I couldn't initialize GLFW!" << std::endl;
		throw -1;
	}
	//Parametry nastêpnego okna
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, is_resizeable);
	//Tworzenie okna
	m_Window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	//Jeœli nie uda³o siê stworzyæ okna wypisz komunikat
	if (m_Window == NULL)
	{
		std::cout << "I couldn't create window!" << std::endl;
		glfwTerminate();
		throw -1;
	}
	//Wybierz okno
	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw -1;
	}
}

GLFWinit::~GLFWinit()
{
	//Automatycznie dealokuj smieci po destrukcj obiektu
	glfwTerminate();
}