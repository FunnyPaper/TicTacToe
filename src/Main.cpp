#include <iostream>
#include <glad/glad.h> 
#include <GLFW\glfw3.h>
#include <SOIL2.h>

#include <glm\glm.hpp>
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "GLFWinit.h"
#include "TicTacToe\Board.h"

const static unsigned int WIDTH = CAMERA_DEFAULT::WIDTH;
const static unsigned int HEIGHT = CAMERA_DEFAULT::HEIGHT;
//Komponenty kamery
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

float lastFrame = 0.0f;
float deltaTime = 0.0f;

void processInput(GLFWwindow* window, float& transparent);

int main(int argc, char** argv)
{
	//Inicjalizacja okna (GLFW + GLAD)
	GLFWinit WindowContext(WIDTH, HEIGHT, "Tic Tac Toe", 3, 3, GLFW_OPENGL_CORE_PROFILE, 0);
	
	GLFWwindow* window = WindowContext.GetCurrentWindow();
	//Zmieniaj rozmiar viewportu w zale¿noœci od aktualnych wymiarów okna
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		});

	//Stworz shadery
	Shader shaders[] = {
		{"res/shaders/vs.shader", "res/shaders/fs.shader"}
	};
	//Przypisz sloty do tekstur
	shaders[0].SetUniform1i("diffuse_texture0", 0);
	shaders[0].SetUniform1i("specular_texture1", 1);
	//Stworz materialy -> do przerobki
	Material blank(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32); blank.SendToShader(shaders[0]);
	//Aktywuj tekstury -> przy renderowaniu dwoch obiektow o roznych teksturach to powinno dziac sie w glownej petli
	//Ustawienia dla zabawy
	float transparent = 0.0f;
	PLAYERS first, second;
	
	// Jesli ktos zechce ustawic sobie poziom trudnosci w konsoli
	if (argc > 1)
	{
		if (std::string(argv[1]) == "COMPUTER") first = PLAYERS::COMPUTER;
		else if (std::string(argv[1]) == "HUMAN") first = PLAYERS::HUMAN;
		if (std::string(argv[2]) == "COMPUTER") second = PLAYERS::COMPUTER;
		else if (std::string(argv[2]) == "HUMAN") second = PLAYERS::HUMAN;
	}
	else
	{
		first = PLAYERS::HUMAN;
		second = PLAYERS::COMPUTER;
	}

	Board TicTacToe(window, first, second);

	//Tutaj dzieje siê renderowanie co klatkê
	glfwSwapInterval(4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(window))
	{
		//Licz deltaTime
		float currentFrame = glfwGetTime(); deltaTime = currentFrame - lastFrame; lastFrame = currentFrame;
		//Wyczyœæ ekran
		Renderer::ClearScreen(0.1f, 0.1f, 0.1f);
		//Zaktualizuj Mat4 -> view and projection matrix + view_position 
		camera.SendToShader(shaders[0]);

		TicTacToe.Play(shaders[0]);
		//Jeœli wciœniêty zostanie klawisz esc wy³¹cz okno
		processInput(window, transparent);
		//Zmieñ buffery do renderu oraz pilnuj eventów
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	return 0;
}

void processInput(GLFWwindow* window, float& transparent)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << "TRYB_RYSOWANIA::GL_LINE" << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		std::cout << "TRYB_RYSOWANIA::GL_POINT" << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::cout << "TRYB_RYSOWANIA::GL_FILL" << std::endl;
	}
	else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (transparent < 1.0f) transparent += 0.01f;
	}
	else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		if (transparent > 0.0f) transparent -= 0.01f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.UpdateMovement(CAMERA_MOVEMENT::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.UpdateMovement(CAMERA_MOVEMENT::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.UpdateMovement(CAMERA_MOVEMENT::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.UpdateMovement(CAMERA_MOVEMENT::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.UpdateMovement(CAMERA_MOVEMENT::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.UpdateMovement(CAMERA_MOVEMENT::DOWN, deltaTime);
	
}