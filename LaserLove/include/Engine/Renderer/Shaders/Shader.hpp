#pragma once
#include <string>

#ifndef SHADER_HPP
#define SHADER_HPP

class Shader
{
public:
	Shader(const std::string& fileLocation = "");
	~Shader();
	void LoadShader(const std::string& fileLocation);
	void DeleteShader();
	const unsigned int GetShaderID() const { return vertOrFragID; };
	const bool GetLoadedSuccessfully() const { return loadedSuccessfully; };
private:

	unsigned int vertOrFragID;
	bool loadedSuccessfully;
};
#endif // !SHADER_HPP
