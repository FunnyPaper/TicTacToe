#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

class GLFWinit
{
	// Klasa odpowiedzialna za wiπzanie OpenGL oraz tworzenie okna kontekstowego
public:
	// Konstruktor
	GLFWinit(unsigned int width, unsigned int height, const char* window_title = "Test", 
		unsigned int major_version = 3, unsigned int minor_version = 3, 
		unsigned int profile = GLFW_OPENGL_CORE_PROFILE, bool is_resizeable = true);
	// Destruktor
	~GLFWinit();
	// ZwrÛÊ wskaünik na okno kontekstowe
	GLFWwindow* GetCurrentWindow() { return m_Window; }

private:
	// Ta klasa powinna mÛc stworzyÊ tylko obiekty statyczne
	void* operator new(size_t) = delete;
	void* operator new[](size_t) = delete;
	// Nie jest moøliwe stworzenie kopii obiektu tej klasy
	GLFWinit(GLFWinit&) = delete;
	GLFWinit(const GLFWinit&) = delete;
	GLFWinit(GLFWinit&&) = delete;
	GLFWinit& operator=(const GLFWinit& other) = delete;
	// UWAGA -> moøliwe jest w dalszym ciπgu stworzenie statycznego okna kontekstowego z tymi samymi parametrami

private:
	// Wskaünik na okno kontekstowe
	GLFWwindow* m_Window{};
};

