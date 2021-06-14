#include <iostream>
// Przetwarzanie danych na grafikê
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
// Inicjowanie okienka kontekstowego
#include "GLFWinit.h"
// Kó³ko i krzy¿yk
#include "TicTacToe\Board.h"

void processInput(GLFWwindow* window);

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

	// Zmienne od których zale¿y który gracz jest pierwszy
	PLAYERS first, second;
	
	// TYLKO W PRZYPADKU ODPALENIA BEZPOŒREDNIO Z KONSOLI!!!
	// Mo¿na wybraæ czy gra ma siê toczyæ pomiêdzy ludŸmi, komputerami czy te¿ cz³owiek vs komputer
	// KOLEJNOŒÆ ARGUMENTÓW KONSOLOWYCH MA ZNACZENIE!!!
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

	// Plansza do gry w kó³ko i krzy¿yk
	// Przechwytuje okno kontekstowe i dwóch graczy
	Board TicTacToe(window, first, second);

	// Czêstotliwoœæ odœwie¿ania ekanu
	glfwSwapInterval(4);

	// Tutaj dzieje siê renderowanie co klatkê
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rozpocznij Grê
		TicTacToe.Play();

		//Jeœli wciœniêty zostanie klawisz esc wy³¹cz okno
		processInput(window);

		//Zmieñ buffery do renderu oraz pilnuj eventów
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}

void processInput(GLFWwindow* window)
{
	// Co siê stanie po wœciœniêciu esc
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		// Ustaw by okno kontekstowe powinno siê zamkn¹æ
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}