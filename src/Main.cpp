#include <iostream>
// Przetwarzanie danych na grafik�
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
// Inicjowanie okienka kontekstowego
#include "GLFWinit.h"
// K�ko i krzy�yk
#include "TicTacToe\Board.h"

void processInput(GLFWwindow* window);

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

	// Zmienne od kt�rych zale�y kt�ry gracz jest pierwszy
	PLAYERS first, second;
	
	// TYLKO W PRZYPADKU ODPALENIA BEZPO�REDNIO Z KONSOLI!!!
	// Mo�na wybra� czy gra ma si� toczy� pomi�dzy lud�mi, komputerami czy te� cz�owiek vs komputer
	// KOLEJNO�� ARGUMENT�W KONSOLOWYCH MA ZNACZENIE!!!
	if (argc > 1)
	{
		if (std::string(argv[1]) == "COMPUTER") first = PLAYERS::COMPUTER;
		else if (std::string(argv[1]) == "HUMAN") first = PLAYERS::HUMAN;
		else first = PLAYERS::HUMAN;

		if (std::string(argv[2]) == "COMPUTER") second = PLAYERS::COMPUTER;
		else if (std::string(argv[2]) == "HUMAN") second = PLAYERS::HUMAN;
		else second = PLAYERS::COMPUTER;
	}
	else
	{
		first = PLAYERS::HUMAN;
		second = PLAYERS::COMPUTER;
	}

	// Plansza do gry w k�ko i krzy�yk
	// Przechwytuje okno kontekstowe i dw�ch graczy
	Board TicTacToe(window, first, second);

	// Cz�stotliwo�� od�wie�ania ekanu
	glfwSwapInterval(4);

	// Tutaj dzieje si� renderowanie co klatk�
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rozpocznij Gr�
		TicTacToe.Play();

		//Je�li wci�ni�ty zostanie klawisz esc wy��cz okno
		processInput(window);

		//Zmie� buffery do renderu oraz pilnuj event�w
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void processInput(GLFWwindow* window)
{
	// Co si� stanie po w�ci�ni�ciu esc
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Ustaw by okno kontekstowe powinno si� zamkn��
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}