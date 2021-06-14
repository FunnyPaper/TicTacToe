#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

class GLFWinit
{
public:
	GLFWinit(unsigned int width, unsigned int height, const char* window_title = "Test", unsigned int major_version = 3, unsigned int minor_version = 3, unsigned int profile = GLFW_OPENGL_CORE_PROFILE, bool is_resizeable = true);
	
	~GLFWinit();

	GLFWwindow* GetCurrentWindow() { return m_Window; }
private:
	// Ja chce tylko jeden obiekt tej klasy dlatego usuwamy inne opcje tworzenia UwU
	void* operator new(size_t) = delete;
	void* operator new[](size_t) = delete;
	GLFWinit(GLFWinit&) = delete;
	GLFWinit(const GLFWinit&) = delete;
	GLFWinit(GLFWinit&&) = delete;
private:
	GLFWwindow* m_Window{};
};

