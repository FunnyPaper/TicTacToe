#include "GLFWinit.h"
#include <iostream>

GLFWinit::GLFWinit(unsigned int width, unsigned int height, const char* window_title, unsigned int major_version, unsigned int minor_version, unsigned int profile, bool is_resizeable)
{
	// Konstruktor GLFWinit -> jest to najwazniejsza czesc tej aplikacji.
	// Bez tej klasy nie jest mozliwe uzycie OpenGL

	// Zainicjuj GLFW
	// glfwInit -> funkcja zewnêtrzna "w³¹caj¹ca" funkcje OpenGL
	// Bez tego nie mo¿na korzystaæ z funkcji GL
	if (!glfwInit())
	{
		std::cout << "Nie udalo sie zainicjowac GLFW!" << std::endl;
		throw -1;
	}
	// Parametry tworzonego okna
	// glfwWindowHint -> funkcja zewnêtrzna odpowiedzialna za ustawienia okna
	// Jako pierwszy argument podajemy flagê (liczbê) -> wybieramy opcjê
	// Drugi argument to wartoœæ jak¹ przypisujemy do wybranej opcji
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, is_resizeable);
	// Tworzenie okna
	// glfwCreateWindow -> funkcja zewnêtrzna odpowiedzialna za tworzenie okan kkontekstowego
	// Podajemy jako argumenty np szerokoœæ, wysokoœæ i tytu³ okna
	// Funkcja zwraca wskaŸnik na element typu glfwWindow
	m_Window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	// Jeœli nie uda³o siê stworzyæ okna wypisz komunikat
	if (m_Window == NULL)
	{
		std::cout << "Nie moge utworzyc ikna kontekstowego!" << std::endl;
		glfwTerminate(); // W takim wypadku u¿ywamy zewnêtrznej funkcji do wy³¹czenia glfw
		throw -1;
	}
	// Wybierz okno
	// glfwMakeContextCurrent przyjmuje wskaŸnik typu glfwWindow
	// Dajemy w ten sposób sygna³, ¿e utworzone wczeœniej okno ma przej¹æ kontrolê nad zawartoœci¹
	glfwMakeContextCurrent(m_Window);
	// Tworzymy ³¹czenie
	// W normalnych okolicznoœciach ¿eby móc u¿ywaæ funkcji do kontaktu z kart¹ graficzn¹ nale¿y porozumiewaæ siê za pomoc¹ wskaŸników funkcji
	// Mo¿na je zdefiniowaæ samemu jako makro jednak jest ich na tyle du¿o, ¿e lepiej wykorzystaæ do tego inn¹ bibliotekê
	// GLAD zawiera mo¿liwoœæ stworzenia takich wskaŸników za pomoc¹ których mo¿emy korzystaæ z funkcji OpenGL
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
	// glfwTerminate -> zewnêtrzna funkcja do dealokacji tego co zosta³o stworzone przez glfw
	glfwTerminate();
}