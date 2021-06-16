#include "Shader.h"
#include "GLerror.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& VertexShaderFilepath, const std::string& FragmentShaderFilepath)
	: m_RendererID(0), m_VertexFilepath(VertexShaderFilepath), m_FragmentFilepath(FragmentShaderFilepath)
{
	// Konstruktor

	// Za�aduj program graficzny z pliku (vertex + fragment shader)
	m_VertexShaderSrc = _LoadShaderFromFile(m_VertexFilepath);
	m_FragmentShaderSrc = _LoadShaderFromFile(m_FragmentFilepath);
	// Stw�rz program gaficzny
	m_RendererID = _CreateShaderProgram(m_VertexShaderSrc, m_FragmentShaderSrc);
}
void Shader::Bind() const
{
	// Aktywuj shader
	GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const
{
	// Dezaktywuj shader
	GLCall(glUseProgram(0));
}
Shader::~Shader()
{
	// Usu� shader od strony OpenGL
	GLCall(glDeleteProgram(m_RendererID));
}
// W tych metodach najpierw aktywuj shader, a nast�pnie przeka� argumenty do programu graficznego
// uniformName to nazwy zmiennych zadeklarowanych w plikach .shader
void Shader::SetUniform2f(const std::string& uniformName, float x, float y)
{
	Bind();
	glUniform2f(glGetUniformLocation(m_RendererID, uniformName.c_str()), x, y);
}
void Shader::SetUniform3f(const std::string& uniformName, float x, float y, float z)
{
	Bind();
	glUniform3f(glGetUniformLocation(m_RendererID, uniformName.c_str()), x, y, z);
}
void Shader::SetUniform1f(const std::string& uniformName, float x)
{
	Bind();
	glUniform1f(glGetUniformLocation(m_RendererID, uniformName.c_str()), x);
}
void Shader::SetUniform1i(const std::string& uniformName, int x)
{
	Bind();
	glUniform1i(glGetUniformLocation(m_RendererID, uniformName.c_str()), x);
}
void Shader::SetUniformMat4f(const std::string& uniformName, const float* matrix)
{
	Bind();
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, uniformName.c_str()), 1, GL_FALSE, matrix);
}
void Shader::SetUniformMat3f(const std::string& uniformName, const float* matrix)
{
	Bind();
	glUniformMatrix3fv(glGetUniformLocation(m_RendererID, uniformName.c_str()), 1, GL_FALSE, matrix);
}
void Shader::SetUniformVec3f(const std::string& uniformName, const float* vec)
{
	Bind();
	glUniform3fv(glGetUniformLocation(m_RendererID, uniformName.c_str()), 1, vec);
}
std::string Shader::_LoadShaderFromFile(const std::string& ShaderFilepath)
{
	// Za�aduj plik .shader i zwr�� jego kod �r�d�owy
	std::string line;
	std::ifstream fromFile(ShaderFilepath);
	std::stringstream str;
	// Zczytuj dane z pliku
	while (getline(fromFile, line))
	{
		str << line << "\n";
	}
	fromFile.close();
	return str.str();
}
unsigned int Shader::_CreateShaderProgram(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc)
{
	// Stw�rz program graficzny
	unsigned int program = glCreateProgram();
	// Skompiluj cz�ci shadera i uzyskaj ich ID
	unsigned int vs = _CompileShader(m_VertexShaderSrc.c_str(), GL_VERTEX_SHADER);
	unsigned int fs = _CompileShader(m_FragmentShaderSrc.c_str(), GL_FRAGMENT_SHADER);
	// Po��cz cz�ci shadera za pomoc� ich ID
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	// Po��cz program graficzny z shaderami
	GLCall(glLinkProgram(program));
	// Utw�rz z��czony shader
	GLCall(glValidateProgram(program));
	// Po tym etapie mo�na usun�� wszystkie pozosta�e dane - nie s� ju� one potrzebne
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	// Zwr�� ID po��czonego shadera
	return program;
}
unsigned int Shader::_CompileShader(const std::string& ShaderSrc, unsigned int ShaderType)
{
	// Stw�rz cz�� shadera w oparciu o jego typ (np Vertex, Fragment, Tessellation, Geometry, etc) podaj�c nazw� zadeklarowan� przez OpenGL
	unsigned int ShaderID = glCreateShader(ShaderType);
	// OpenGl zosta� napisany w oparciu o C wi�c du�a cz�� funkcji wykorzystuje ten standard
	// Dlatego te� wszystkie stringi w C++ musimy przekonwertowa� na odpowiednik C
	const char* str = ShaderSrc.c_str();
	// Po��cz informacje o shaderze (typ i kod �r�d�owy)
	GLCall(GLCall(glShaderSource(ShaderID, 1, &str, NULL)));
	// Skompiluj shader
	GLCall(GLCall(glCompileShader(ShaderID)));
	// Sprawd� czy kompilacja przebieg�a poprawnie
	int isProperCompiled;
	char errorMessage[512];
	// Uzyskaj informacje o statusie kompilacji
	GLCall(glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &isProperCompiled));
	// Je�li co� si� nie powiod�o to uzyskaj informacj� zwrotn� i wypisz j� do konsoli
	if (!isProperCompiled)
	{
		GLCall(glGetShaderInfoLog(ShaderID, 512, NULL, errorMessage));
		std::cout << "ERROR::SHADER::" << (ShaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << errorMessage << std::endl;
		GLCall(glDeleteShader(ShaderID));
	}
	// Zwr�� ID shadera
	return ShaderID;
}