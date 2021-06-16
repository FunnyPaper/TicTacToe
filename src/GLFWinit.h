#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

class GLFWinit
{
	// Klasa odpowiedzialna za wi�zanie OpenGL oraz tworzenie okna kontekstowego
public:
	// Konstruktor
	GLFWinit(unsigned int width, unsigned int height, const char* window_title = "Test", 
		unsigned int major_version = 3, unsigned int minor_version = 3, 
		unsigned int profile = GLFW_OPENGL_CORE_PROFILE, bool is_resizeable = true);
	// Destruktor
	~GLFWinit();
	// Zwr�� wska�nik na okno kontekstowe
	GLFWwindow* GetCurrentWindow() { return m_Window; }

private:
	// Ta klasa powinna m�c stworzy� tylko obiekty statyczne
	void* operator new(size_t) = delete;
	void* operator new[](size_t) = delete;
	// Nie jest mo�liwe stworzenie kopii obiektu tej klasy
	GLFWinit(GLFWinit&) = delete;
	GLFWinit(const GLFWinit&) = delete;
	GLFWinit(GLFWinit&&) = delete;
	GLFWinit& operator=(const GLFWinit& other) = delete;
	// UWAGA -> mo�liwe jest w dalszym ci�gu stworzenie statycznego okna kontekstowego z tymi samymi parametrami

private:
	// Wska�nik na okno kontekstowe
	GLFWwindow* m_Window{};
};

