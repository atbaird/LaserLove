#pragma once
#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/RendererEnums.hpp"
#include "Engine/Renderer/3DModelling/Skeleton.hpp"
#include <vector>
#include <map>

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

struct ShaderFileLocations
{
	ShaderFileLocations(const std::string& vertLoc = "", const std::string& fragLoc = "");
	ShaderFileLocations(const ShaderFileLocations& other);
	const bool operator<(const ShaderFileLocations& other) const;
	std::string vertShaderLocation;
	std::string fragShaderLocation;
};

class ShaderProgram;
#define STATIC // Do-nothing indicator that method/member is static in class definition
extern STATIC std::map<ShaderFileLocations, ShaderProgram*> s_shaderPrograms;

struct bindFoundOrNot
{
	bool bindFound;
	int bindLocation;
};

class ShaderProgram
{
protected:
	static ShaderProgram* s_defaultShaderProgram;
public:
	//Common ShaderBinds.
	static const std::string s_gCameraPosition;

	//Constructors
	ShaderProgram();
	ShaderProgram(const std::string& vertexShaderFileLocation, const std::string& fragmentShaderFileLocation);
	~ShaderProgram();

	//Default Shader Program
	static void SetDefaultShaderProgram(ShaderProgram* shader);
	static ShaderProgram* GetDefaultShaderProgram();

	//Create or get or find
	static ShaderProgram* CreateOrGetShaderProgram(const std::string& vertexShaderFileLocation,
		const std::string& fragmentShaderFileLocation);
	static ShaderProgram* FindShaderProgram(const std::string& vertexShaderFileLocation, 
		const std::string& fragmentShaderFileLocation);
	static void SetProjectionAndViewAll(const Matrix44& projection, const Matrix44& view);
	static void SetProjectionAll(const Matrix44& projection);
	static void SetViewAll(const Matrix44& view);

	//Use
	void SetUseThisProgram() const;
	void SetUseProgramNull() const;
	void SingleTextureDrawShaderProgram(const PrimitiveType& drawMode, const unsigned int& vaoID,
		const unsigned int& numOfIndicesToDraw,
		const unsigned int& textureIndex = NULL, const unsigned int& textureID = NULL, 
		const unsigned int& samplerID = NULL) const;

	//Getters
	const void FindAndStoreBind(const std::string& valueName);
	const unsigned int GetShaderProgramID();
	const bool GetLoadedSuccessfully();
	const ShaderFileLocations GetShaderFileLocations() const;
	
	//Setters
	const bool SetProgramValueMatrix4by4(const std::string& valueName, const Matrix44* mat, 
		const bool& transposeMatrix = true, const unsigned int& numOfElements = 1) const;
	const bool SetProgramValueRGBA(const std::string& valueName, const Rgba* color, 
		const unsigned int& numOfElements = 1) const;
	const bool SetProgramValueVector4(const std::string& valueName, const Vector4* vec4, 
		const unsigned int& numOfElements = 1) const;
	const bool SetProgramValueVector3(const std::string& valueName, const Vector3* vec3, 
		const unsigned int& numOfElements = 1) const;
	//const bool SetProgramValueEulerAngles(const std::string& valueName, const EulerAngles* euler, const unsigned int& numOfElements = 1) const;
	const bool SetProgramValueVector2(const std::string& valueName, const Vector2* vec2, 
		const unsigned int& numOfElements = 1) const;
	const bool SetProgramValueFloat(const std::string& valueName, const float* flo, 
		const unsigned int& numOfElements = 1) const;
	const bool SetProgramValueBool(const std::string& valueName, const bool* boo, 
		const unsigned int& numOfElements = 1) const;
	const bool SetProgramValueInt(const std::string& valueName, const int* integer,
		const unsigned int& numOfElements = 1) const;
	const bool SetSkeleton(const std::string& valueName, const Skeleton* skele, 
		const bool& debug = false) const;

	static void DeletAllShaderPrograms();

private:
	const void AddBindToShaderProgram(const std::string& valueName, const int& bindLocation);
	const bindFoundOrNot FindBind(const std::string& bindLocation) const;
	static const void InternalAddBind(ShaderProgram* prog, const std::string& bindName, const int& bindLoc);

	unsigned int m_shaderProgID;
	bool m_loadedSuccessfully;
	ShaderFileLocations fileLocations;
	std::map<std::string, int> m_bindLocations;

	static const unsigned int nullVal;
};
#endif