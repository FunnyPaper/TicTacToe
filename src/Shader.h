#pragma once

#include <string>
#include <glm\gtc\type_ptr.hpp>

class Shader
{
	// Klasa odpowiedzialna za komunikacj� z workflowem graficznym
private:
	// Zmienne do przechowywania lokalizacji program�w oraz ich kodu �r�d�owego
	std::string m_VertexFilepath;
	std::string m_FragmentFilepath;
	std::string m_VertexShaderSrc;
	std::string m_FragmentShaderSrc;
	// Unikatowe ID Shadera
	unsigned int m_RendererID;

public:
	// Konstruktor Shadera
	Shader(const std::string& VertexShaderFilepath, const std::string& FragmentShaderFilepath);
	// Destruktor Shadera
	~Shader();
	// Metody odpowiedzialne za aktywacj�/ deaktywacj� shadera
	void Bind() const;
	void Unbind() const;
	// Metody odpowiedzialne za komunikacj� mi�dzy u�ytkownikiem, a utworzonym shaderem (nie mamy do niego bezpo�redniego dost�pu)
	void SetUniform2f(const std::string& uniformName, float x, float y);
	void SetUniform1f(const std::string& uniformName, float x);
	void SetUniform1i(const std::string& unifromName, int x);
	void SetUniform3f(const std::string& uniformName, float x, float y, float z);
	void SetUniformMat3f(const std::string& uniformName, const float* matrix);
	void SetUniformMat4f(const std::string& uniformName, const float* matrix);
	void SetUniformVec3f(const std::string& uniformName, const float* vec);
	// Metody zwracaj�ce prywatne zmienne klasy
	inline std::string GetVertexFilepath() const { return m_VertexFilepath; }
	inline std::string GetFragmentFilepath() const { return m_FragmentFilepath; }
	inline std::string GetVertexShaderSrc() const { return m_VertexShaderSrc; }
	inline std::string GetFragmentShaderSrc() const { return m_FragmentShaderSrc; }
	inline unsigned int GetShaderID() const { return m_RendererID; }

private:
	// Za�aduj program graficzny z pliku
	std::string _LoadShaderFromFile(const std::string& ShaderFilepath);
	// Skompiluj shader
	unsigned int _CompileShader(const std::string& ShaderSrc, unsigned int ShaderType);
	// Stw�rz program shadera
	unsigned int _CreateShaderProgram(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc);
};

