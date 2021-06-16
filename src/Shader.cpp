#include "Shader.h"
#include "GLerror.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& VertexShaderFilepath, const std::string& FragmentShaderFilepath)
	: m_RendererID(0), m_VertexFilepath(VertexShaderFilepath), m_FragmentFilepath(FragmentShaderFilepath)
{
	// Konstruktor

	// Za³aduj program graficzny z pliku (vertex + fragment shader)
	m_VertexShaderSrc = _LoadShaderFromFile(m_VertexFilepath);
	m_FragmentShaderSrc = _LoadShaderFromFile(m_FragmentFilepath);
	// Stwórz program gaficzny
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
	// Usuñ shader od strony OpenGL
	GLCall(glDeleteProgram(m_RendererID));
}
// W tych metodach najpierw aktywuj shader, a nastêpnie przeka¿ argumenty do programu graficznego
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
	// Za³aduj plik .shader i zwróæ jego kod ¿ród³owy
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
	// Stwórz program graficzny
	unsigned int program = glCreateProgram();
	// Skompiluj czêœci shadera i uzyskaj ich ID
	unsigned int vs = _CompileShader(m_VertexShaderSrc.c_str(), GL_VERTEX_SHADER);
	unsigned int fs = _CompileShader(m_FragmentShaderSrc.c_str(), GL_FRAGMENT_SHADER);
	// Po³¹cz czêœci shadera za pomoc¹ ich ID
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	// Po³¹cz program graficzny z shaderami
	GLCall(glLinkProgram(program));
	// Utwórz z³¹czony shader
	GLCall(glValidateProgram(program));
	// Po tym etapie mo¿na usun¹æ wszystkie pozosta³e dane - nie s¹ ju¿ one potrzebne
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
	// Zwróæ ID po³¹czonego shadera
	return program;
}
unsigned int Shader::_CompileShader(const std::string& ShaderSrc, unsigned int ShaderType)
{
	// Stwórz czêœæ shadera w oparciu o jego typ (np Vertex, Fragment, Tessellation, Geometry, etc) podaj¹c nazwê zadeklarowan¹ przez OpenGL
	unsigned int ShaderID = glCreateShader(ShaderType);
	// OpenGl zosta³ napisany w oparciu o C wiêc du¿a czêœæ funkcji wykorzystuje ten standard
	// Dlatego te¿ wszystkie stringi w C++ musimy przekonwertowaæ na odpowiednik C
	const char* str = ShaderSrc.c_str();
	// Po³¹cz informacje o shaderze (typ i kod Ÿród³owy)
	GLCall(GLCall(glShaderSource(ShaderID, 1, &str, NULL)));
	// Skompiluj shader
	GLCall(GLCall(glCompileShader(ShaderID)));
	// SprawdŸ czy kompilacja przebieg³a poprawnie
	int isProperCompiled;
	char errorMessage[512];
	// Uzyskaj informacje o statusie kompilacji
	GLCall(glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &isProperCompiled));
	// Jeœli coœ siê nie powiod³o to uzyskaj informacjê zwrotn¹ i wypisz j¹ do konsoli
	if (!isProperCompiled)
	{
		GLCall(glGetShaderInfoLog(ShaderID, 512, NULL, errorMessage));
		std::cout << "ERROR::SHADER::" << (ShaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << errorMessage << std::endl;
		GLCall(glDeleteShader(ShaderID));
	}
	// Zwróæ ID shadera
	return ShaderID;
}