#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Renderer/Renderer.hpp"


Shader::Shader(const std::string& fileLocation)
{
	LoadShader(fileLocation);
}

Shader::~Shader()
{
	DeleteShader();
}

void Shader::LoadShader(const std::string& fileLocation)
{
	vertOrFragID = g_Renderer->CompileShader(fileLocation, loadedSuccessfully);
}

void Shader::DeleteShader()
{
	g_Renderer->DeleteShader(vertOrFragID);
	vertOrFragID = 0;
}