#pragma once
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/CubeMap.hpp"
#include "Engine/Math/Matrix44.hpp"

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

class Material
{
private:
	//Common static variables
	static const std::string s_shaderModelBindName;
	static const std::string s_shaderViewBindName;
	static const std::string s_shaderProjectionBindName;

private:
	//Variables
	bool m_alwaysUseDefaultShader;
	std::vector<unsigned int> textureBinds;
	std::vector<unsigned int> cubeMapBinds;
	std::vector<const Texture*> meshTextures;
	std::vector<const CubeMap*> meshCubeMaps;
	std::vector<std::string> textureNames;
	std::vector<std::string> cubeMapNames;
	ShaderProgram* meshShaderProg;
public:
	//Constructors
	Material();
	Material(ShaderProgram* shaderProg, std::vector<const Texture*>& texturesToUse, std::vector<unsigned int>& texBinds, const std::vector<std::string>& texBindNames);
	Material(const Material&);
	~Material();

	//Setters
	void SetProgram(ShaderProgram* shaderProg);
	void SetTextures(std::vector<const Texture*>& texturesToUse);
	void SetCubeMap(std::vector<const CubeMap*>& CubeMapsToUse);
	const bool SetTexture(const Texture* texToUse, const unsigned int& textureBind, const std::string& name, const unsigned int& index);
	void SetOrAddTexture(const Texture* texToUse, const unsigned int& textureBind, const std::string& name, const unsigned int& index);
	void SetCubeMap(const CubeMap* cubeToUse, const unsigned int& cubeMapBind, const std::string& name, const unsigned int& index);
	void AddTexture(const Texture* texToUse, const unsigned int& textureBind, const std::string& textureBindName = "");
	void AddCubeMap(const CubeMap* texToUse, const unsigned int& textureBind, const std::string& textureBindName = "");
	void SetTextureBindLocations(std::vector<unsigned int>& texBinds);
	void SetCubeMapBindLocations(std::vector<unsigned int>& cubeBinds);
	void SetMatrices(const Matrix44& model, const Matrix44& view, const Matrix44& projection) const;
	void SetModelMatrix(const Matrix44& model) const;
	void SetAlwaysUseDefaultShader(const bool& alwaysUseDefShader);

	//getters
	ShaderProgram* GetProgram() const;
	const std::vector<const Texture*>* GetTextures() const;
	const std::vector<const CubeMap*>* GetCubeMaps() const;
	const std::vector<unsigned int>* GetBindLocations() const;
	const std::vector<unsigned int>* GetCubeMapBindLocations() const;
	const std::vector<std::string>* GetTextureBindNames() const;
	const std::vector<std::string>* GetCubeMapBindNames() const;
	const bool GetAlwaysUseDefaultShader() const;
	static Material* MakeCopy(const Material* mat);

	//Operators
	void ActiveTextures(const unsigned int& samplerID) const;
	void DeactivateTextures() const;
	void DeleteProgram();

};
#endif