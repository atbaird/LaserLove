#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Renderer/Renderer.hpp"

//---------------------------------------------------------------------------------
//Constructors
const std::string Material::s_shaderModelBindName = "gModel";
const std::string Material::s_shaderViewBindName = "gView";
const std::string Material::s_shaderProjectionBindName = "gProj";

Material::Material()
	: meshShaderProg(ShaderProgram::GetDefaultShaderProgram())
{
}
Material::Material(ShaderProgram* shaderProg, std::vector<const Texture*>& texturesToUse, std::vector<unsigned int>& texBinds, const std::vector<std::string>& texBindNames)
	: meshTextures(texturesToUse),
	meshShaderProg(shaderProg),
	textureBinds(texBinds),
	textureNames(texBindNames),
	m_alwaysUseDefaultShader(false)
{
}
Material::Material(const Material& other)
	: meshTextures(other.meshTextures),
	meshShaderProg(other.meshShaderProg),
	textureBinds(other.textureBinds),
	cubeMapBinds(other.cubeMapBinds),
	meshCubeMaps(other.meshCubeMaps),
	cubeMapNames(other.cubeMapNames),
	m_alwaysUseDefaultShader(other.m_alwaysUseDefaultShader)
{

}
Material::~Material()
{
}

//---------------------------------------------------------------------------------
//Setters
void Material::SetProgram(ShaderProgram* shaderProg)
{
	meshShaderProg = nullptr;
	meshShaderProg = shaderProg;
}

void Material::SetTextures(std::vector<const Texture*>& texturesToUse)
{
	meshTextures = texturesToUse;
}
void Material::SetCubeMap(std::vector<const CubeMap*>& CubeMapsToUse)
{
	meshCubeMaps = CubeMapsToUse;
}
const bool Material::SetTexture(const Texture* texToUse, const unsigned int& textureBind, const std::string& name, const unsigned int& index)
{
	if (index >= meshTextures.size())
	{
		return false;
	}
	meshTextures[index] = nullptr;
	meshTextures[index] = texToUse;
	textureBinds[index] = textureBind;
	textureNames[index] = name;
	return true;
}

void Material::SetOrAddTexture(const Texture* texToUse, const unsigned int& textureBind, const std::string& name, const unsigned int& index)
{
	if (SetTexture(texToUse, textureBind, name, index) == false)
	{
		AddTexture(texToUse, textureBind, name);
	}
}

void Material::SetCubeMap(const CubeMap* cubeToUse, const unsigned int& cubeMapBind, const std::string& name, const unsigned int& index)
{
	if (index >= meshCubeMaps.size())
	{
		return;
	}
	meshCubeMaps[index] = nullptr;
	meshCubeMaps[index] = cubeToUse;
	cubeMapBinds[index] = cubeMapBind;
	cubeMapNames[index] = name;
}
void Material::AddTexture(const Texture* texToUse, const unsigned int& textureBind, const std::string& textureBindName)
{
	meshTextures.push_back(texToUse);
	textureBinds.push_back(textureBind);
	textureNames.push_back(textureBindName);
}
void Material::AddCubeMap(const CubeMap* cubeToUse, const unsigned int& cubeBind, const std::string& textureBindName)
{
	meshCubeMaps.push_back(cubeToUse);
	cubeMapBinds.push_back(cubeBind);
	cubeMapNames.push_back(textureBindName);
}
void Material::SetTextureBindLocations(std::vector<unsigned int>& texBinds)
{
	textureBinds = texBinds;
}
void Material::SetCubeMapBindLocations(std::vector<unsigned int>& cubeBinds)
{
	cubeMapBinds = cubeBinds; 
}
void Material::SetMatrices(const Matrix44& model, const Matrix44& view, const Matrix44& projection) const
{
	if (meshShaderProg == nullptr)
	{
		return;
	}
	meshShaderProg->SetProgramValueMatrix4by4(s_shaderModelBindName, &model, true);
	meshShaderProg->SetProgramValueMatrix4by4(s_shaderViewBindName, &view, true);
	meshShaderProg->SetProgramValueMatrix4by4(s_shaderProjectionBindName, &projection, true);
}

void Material::SetModelMatrix(const Matrix44& model) const
{
	if (meshShaderProg == nullptr)
	{
		return;
	}
	meshShaderProg->SetProgramValueMatrix4by4(s_shaderModelBindName, &model, true);
}

void Material::SetAlwaysUseDefaultShader(const bool& alwaysUseDefShader)
{
	m_alwaysUseDefaultShader = alwaysUseDefShader;
}


//----------------------------------------------------------------------------------------
//getters
ShaderProgram* Material::GetProgram() const 
{ 
	if (m_alwaysUseDefaultShader == true)
	{
		return ShaderProgram::GetDefaultShaderProgram();
	}
	return meshShaderProg; 
}
const std::vector<const Texture*>* Material::GetTextures() const
{ 
	return &meshTextures; 
}
const std::vector<const CubeMap*>* Material::GetCubeMaps() const
{ 
	return &meshCubeMaps;
}
const std::vector<unsigned int>* Material::GetBindLocations() const
{ 
	return &textureBinds; 
}
const std::vector<unsigned int>* Material::GetCubeMapBindLocations() const
{ 
	return &cubeMapBinds; 
}
const std::vector<std::string>* Material::GetTextureBindNames() const
{ 
	return &textureNames; 
}
const std::vector<std::string>* Material::GetCubeMapBindNames() const
{ 
	return &cubeMapNames; 
}
const bool Material::GetAlwaysUseDefaultShader() const
{
	return m_alwaysUseDefaultShader;
}
Material* Material::MakeCopy(const Material* mat)
{
	if (mat == nullptr)
	{
		return nullptr;
	}
	Material* copy = new Material();
	copy->m_alwaysUseDefaultShader = mat->m_alwaysUseDefaultShader;
	copy->textureBinds = mat->textureBinds;
	copy->cubeMapBinds = mat->cubeMapBinds;
	copy->meshTextures = mat->meshTextures;
	copy->meshCubeMaps = mat->meshCubeMaps;
	copy->textureNames = mat->textureNames;
	copy->cubeMapNames = mat->cubeMapNames;
	copy->meshShaderProg = mat->meshShaderProg;
	return copy;
}

//----------------------------------------------------------------------------------------
//Operators

void Material::ActiveTextures(const unsigned int& samplerID) const
{
	for (size_t index = 0; index < textureBinds.size() && index < meshTextures.size(); index++)
	{
		if (meshTextures.at(index) != nullptr)
		{
			const int bindLoc = (const int)(textureBinds.at(index));
			meshShaderProg->SetProgramValueInt(textureNames.at(index), &bindLoc);
			g_Renderer->ActivateTexture(textureBinds.at(index), meshTextures.at(index)->m_openglTextureID, samplerID);
		}
	}
	for (size_t index = 0; index < cubeMapBinds.size() && index < meshCubeMaps.size(); index++)
	{
		if (meshCubeMaps.at(index) != nullptr)
		{
			const int bindLoc = (const int)(cubeMapBinds.at(index));
			meshShaderProg->SetProgramValueInt(cubeMapNames.at(index), &bindLoc);
			g_Renderer->ActivateTexture(cubeMapBinds[index], meshCubeMaps[index]->getTextureObj(), samplerID, CUBE_MAP_TYPE);
		}
	}
}
void Material::DeactivateTextures() const
{
	for (size_t index = 0; index < textureBinds.size() && index < meshTextures.size(); index++)
	{
		g_Renderer->UnbindTexture(textureBinds.at(index));
	}
	for (size_t index = 0; index < cubeMapBinds.size() && index < meshCubeMaps.size(); index++)
	{
		g_Renderer->UnbindTexture(cubeMapBinds.at(index), CUBE_MAP_TYPE);
	}
}
void Material::DeleteProgram()
{
	if (meshShaderProg != nullptr)
	{
		delete meshShaderProg;
	}
}