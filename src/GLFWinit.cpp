#include "GLFWinit.h"
#include <iostream>

GLFWinit::GLFWinit(unsigned int width, unsigned int height, const char* window_title, unsigned int major_version, unsigned int minor_version, unsigned int profile, bool is_resizeable)
{
	// Konstruktor GLFWinit -> jest to najwazniejsza czesc tej aplikacji.
	// Bez tej klasy nie jest mozliwe uzycie OpenGL

	// Zainicjuj GLFW
	// glfwInit -> funkcja zewn�trzna "w��caj�ca" funkcje OpenGL
	// Bez tego nie mo�na korzysta� z funkcji GL
	if (!glfwInit())
	{
		std::cout << "Nie udalo sie zainicjowac GLFW!" << std::endl;
		throw -1;
	}
	// Parametry tworzonego okna
	// glfwWindowHint -> funkcja zewn�trzna odpowiedzialna za ustawienia okna
	// Jako pierwszy argument podajemy flag� (liczb�) -> wybieramy opcj�
	// Drugi argument to warto�� jak� przypisujemy do wybranej opcji
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, is_resizeable);
	// Tworzenie okna
	// glfwCreateWindow -> funkcja zewn�trzna odpowiedzialna za tworzenie okan kkontekstowego
	// Podajemy jako argumenty np szeroko��, wysoko�� i tytu� okna
	// Funkcja zwraca wska�nik na element typu glfwWindow
	m_Window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	// Je�li nie uda�o si� stworzy� okna wypisz komunikat
	if (m_Window == NULL)
	{
		std::cout << "Nie moge utworzyc ikna kontekstowego!" << std::endl;
		glfwTerminate(); // W takim wypadku u�ywamy zewn�trznej funkcji do wy��czenia glfw
		throw -1;
	}
	// Wybierz okno
	// glfwMakeContextCurrent przyjmuje wska�nik typu glfwWindow
	// Dajemy w ten spos�b sygna�, �e utworzone wcze�niej okno ma przej�� kontrol� nad zawarto�ci�
	glfwMakeContextCurrent(m_Window);
	// Tworzymy ��czenie
	// W normalnych okoliczno�ciach �eby m�c u�ywa� funkcji do kontaktu z kart� graficzn� nale�y porozumiewa� si� za pomoc� wska�nik�w funkcji
	// Mo�na je zdefiniowa� samemu jako makro jednak jest ich na tyle du�o, �e lepiej wykorzysta� do tego inn� bibliotek�
	// GLAD zawiera mo�liwo�� stworzenia takich wska�nik�w za pomoc� kt�rych mo�emy korzysta� z funkcji OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Nie udalo sie zainicjowac GLAD" << std::endl;
		throw -1;
	}
}

GLFWinit::~GLFWinit()
{
	// Destruktor GLFWinit

	// Automatycznie dealokuj smieci po destrukcji obiektu
	// glfwTerminate -> zewn�trzna funkcja do dealokacji tego co zosta�o stworzone przez glfw
	glfwTerminate();
}