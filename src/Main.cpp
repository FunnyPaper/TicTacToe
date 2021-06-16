#include <iostream>
// Przetwarzanie danych na grafikê
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
// Inicjowanie okienka kontekstowego
#include "GLFWinit.h"
// Kó³ko i krzy¿yk
#include "TicTacToe\Board.h"

int main(int argc, char** argv)
{
	//Inicjalizacja okna kontekstowego (GLFW + GLAD)
	GLFWinit WindowContext(
		CAMERA_DEFAULT::WIDTH, CAMERA_DEFAULT::HEIGHT, 
		"Tic Tac Toe", 3, 3, GLFW_OPENGL_CORE_PROFILE, 0);

	// Zwróæ wskaŸnik na okno kontekstowe
	GLFWwindow* window = WindowContext.GetCurrentWindow();

	// Zmieniaj rozmiar viewportu w zale¿noœci od aktualnych wymiarów okna
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});

	// Plansza do gry w kó³ko i krzy¿yk
	// Przechwytuje okno kontekstowe i dwóch graczy
	Board TicTacToe(window);

	// Rozpocznij Grê
	TicTacToe.Play();
	return 0;
}