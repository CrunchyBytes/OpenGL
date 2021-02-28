#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private: 
	std::string m_FilePath;
	unsigned int m_RendererID;
	// caching for uniforms
	/* Para no generar muchos uniforms a la vez, leemos una, y las ordena en un table. Al querer ocupar un uniform, si ya lo creó y leyó, sólov amos al lookup table en lugar de volverlo a asignar */
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	/* Constructor and Destructor */
	Shader(const std::string& filepath);
	~Shader();

	/* Bind and Unbind */
	void Bind() const; 
	void UnBind() const; 

	//Set uniforms
	void SetUniform1i(const std::string& name, int value); // e.g.
	void SetUniform1f(const std::string& name, float value); // e.g.
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	int GetUniformLocation(const std::string& name);
};