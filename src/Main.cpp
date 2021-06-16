#include <iostream>
// Przetwarzanie danych na grafik�
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
// Inicjowanie okienka kontekstowego
#include "GLFWinit.h"
// K�ko i krzy�yk
#include "TicTacToe\Board.h"

int main(int argc, char** argv)
{
	//Inicjalizacja okna kontekstowego (GLFW + GLAD)
	GLFWinit WindowContext(
		CAMERA_DEFAULT::WIDTH, CAMERA_DEFAULT::HEIGHT, 
		"Tic Tac Toe", 3, 3, GLFW_OPENGL_CORE_PROFILE, 0);

	// Zwr�� wska�nik na okno kontekstowe
	GLFWwindow* window = WindowContext.GetCurrentWindow();

	// Zmieniaj rozmiar viewportu w zale�no�ci od aktualnych wymiar�w okna
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});

	// Plansza do gry w k�ko i krzy�yk
	// Przechwytuje okno kontekstowe i dw�ch graczy
	Board TicTacToe(window);

	// Rozpocznij Gr�
	TicTacToe.Play();
	return 0;
}