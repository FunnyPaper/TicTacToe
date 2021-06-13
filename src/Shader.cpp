#include "Shader.h"
#include "GLerror.h"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& VertexShaderFilepath, const std::string& FragmentShaderFilepath)
	: m_RendererID(0), m_VertexFilepath(VertexShaderFilepath), m_FragmentFilepath(FragmentShaderFilepath)
{
	m_VertexShaderSrc = _LoadShaderFromFile(m_VertexFilepath);
	m_FragmentShaderSrc = _LoadShaderFromFile(m_FragmentFilepath);
	m_RendererID = _CreateShaderProgram(m_VertexShaderSrc, m_FragmentShaderSrc);
}
Shader::Shader()
	: m_RendererID(0), m_VertexFilepath(""), m_FragmentFilepath("") {}
void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}
Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}
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
	std::string line;
	std::ifstream fromFile(ShaderFilepath);
	std::stringstream str;
	while (getline(fromFile, line))
	{
		str << line << "\n";
	}
	fromFile.close();
	return str.str();
}
unsigned int Shader::_CreateShaderProgram(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = _CompileShader(m_VertexShaderSrc.c_str(), GL_VERTEX_SHADER);
	unsigned int fs = _CompileShader(m_FragmentShaderSrc.c_str(), GL_FRAGMENT_SHADER);
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}
unsigned int Shader::_CompileShader(const std::string& ShaderSrc, unsigned int ShaderType)
{
	unsigned int ShaderID = glCreateShader(ShaderType);
	const char* str = ShaderSrc.c_str();
	GLCall(GLCall(glShaderSource(ShaderID, 1, &str, NULL)));
	GLCall(GLCall(glCompileShader(ShaderID)));

	int isProperCompiled;
	char errorMessage[512];
	GLCall(glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &isProperCompiled));
	if (!isProperCompiled)
	{
		GLCall(glGetShaderInfoLog(ShaderID, 512, NULL, errorMessage));
		std::cout << "ERROR::SHADER::" << (ShaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << errorMessage << std::endl;
		GLCall(glDeleteShader(ShaderID));
	}
	return ShaderID;
}