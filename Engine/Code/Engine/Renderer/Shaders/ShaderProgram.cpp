#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/FileUtils.hpp"

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Static variables
const unsigned int ShaderProgram::nullVal = NULL;
STATIC std::map<ShaderFileLocations, ShaderProgram*> s_shaderPrograms = std::map<ShaderFileLocations, ShaderProgram*>();

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//ShaderFileLocations
ShaderFileLocations::ShaderFileLocations(const std::string& vertLoc, const std::string& fragLoc)
	: vertShaderLocation(vertLoc),
	fragShaderLocation(fragLoc)
{
}
ShaderFileLocations::ShaderFileLocations(const ShaderFileLocations& other)
	: vertShaderLocation(other.vertShaderLocation),
	fragShaderLocation(other.fragShaderLocation)
{
}
const bool ShaderFileLocations::operator<(const ShaderFileLocations& other) const
{
	if (fragShaderLocation >= other.fragShaderLocation || vertShaderLocation >= other.vertShaderLocation)
	{
		return false;
	}
	return true;
}



//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Constructors


const std::string ShaderProgram::s_gCameraPosition = "gCameraPosition";
ShaderProgram* ShaderProgram::s_defaultShaderProgram = nullptr;

ShaderProgram::ShaderProgram()
	: m_shaderProgID(0),
	m_loadedSuccessfully(false),
	fileLocations("", "")
{
}
ShaderProgram::ShaderProgram(const std::string& vertexShaderFileLocation, const std::string& fragmentShaderFileLocation)
	:m_shaderProgID(0),
	m_loadedSuccessfully(false),
	fileLocations(vertexShaderFileLocation, fragmentShaderFileLocation)
{
	Shader* fragmentShader = new Shader(fragmentShaderFileLocation);
	Shader* vertexShader = new Shader(vertexShaderFileLocation);
	if (fragmentShader->GetLoadedSuccessfully() == false)
	{
		delete fragmentShader;
		delete vertexShader;
		return;
	}
	if (vertexShader->GetLoadedSuccessfully() == false)
	{
		delete fragmentShader;
		delete vertexShader;
		return;
	}
	m_shaderProgID = g_Renderer->CreateAndLinkShaderProgram(vertexShader->GetShaderID(), fragmentShader->GetShaderID());

	m_loadedSuccessfully = true;
	delete fragmentShader;
	delete vertexShader;
}
ShaderProgram::~ShaderProgram()
{
	g_Renderer->DeleteShaderProgram(m_shaderProgID);
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Default Shader Program
void ShaderProgram::SetDefaultShaderProgram(ShaderProgram* shader)
{
	s_defaultShaderProgram = nullptr;
	s_defaultShaderProgram = shader;
}
ShaderProgram* ShaderProgram::GetDefaultShaderProgram()
{
	return s_defaultShaderProgram;
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Create or get or find
ShaderProgram* ShaderProgram::CreateOrGetShaderProgram(const std::string& vertexShaderFileLocation, const std::string& fragmentShaderFileLocation)
{
	ShaderFileLocations fileLocs = ShaderFileLocations(vertexShaderFileLocation, fragmentShaderFileLocation);
	std::map<ShaderFileLocations, ShaderProgram*>::iterator it = s_shaderPrograms.find(fileLocs);
	if (it != s_shaderPrograms.end())
	{
		return s_shaderPrograms.at(fileLocs);
	}
	//bool vertexShaderValid = FileUtils::VerifyFileExists(vertexShaderFileLocation);
	//bool fragmentShaderValid = FileUtils::VerifyFileExists(fragmentShaderFileLocation);
	//if (vertexShaderValid == false
	//	|| fragmentShaderValid == false)
	//{
	//	DebuggerPrintf("\nDid not create shader");
	//	if (vertexShaderValid == false)
	//	{
	//		DebuggerPrintf("\n%s was invalid.", vertexShaderFileLocation.c_str());
	//	}
	//	if (fragmentShaderValid == false)
	//	{
	//		DebuggerPrintf("\n%s was invalid.", fragmentShaderFileLocation.c_str());
	//	}
	//	DebuggerPrintf("\n");
	//	return nullptr;
	//}

	ShaderProgram* prog = new ShaderProgram(vertexShaderFileLocation, fragmentShaderFileLocation);
	if (prog->GetLoadedSuccessfully() == true)
	{
		s_shaderPrograms.insert(std::pair<ShaderFileLocations, ShaderProgram*>(fileLocs, prog));
		return prog;
	}
	delete prog;
	ERROR_RECOVERABLE("Failed to find or load shader program.");
	return nullptr;
}
ShaderProgram* ShaderProgram::FindShaderProgram(const std::string& vertexShaderFileLocation, const std::string& fragmentShaderFileLocation)
{
	ShaderFileLocations fileLocs = ShaderFileLocations(vertexShaderFileLocation, fragmentShaderFileLocation);
	std::map<ShaderFileLocations, ShaderProgram*>::iterator it = s_shaderPrograms.find(fileLocs);
	if (it != s_shaderPrograms.end())
	{
		return s_shaderPrograms.at(fileLocs);
	}
	return nullptr;
}
void ShaderProgram::SetProjectionAndViewAll(const Matrix44& projection, const Matrix44& view)
{
	for (std::map<ShaderFileLocations, ShaderProgram*>::iterator it = s_shaderPrograms.begin(); it != s_shaderPrograms.end(); it++)
	{
		if (it->second != nullptr)
		{
			it->second->SetProgramValueMatrix4by4("gProj", &projection);
			it->second->SetProgramValueMatrix4by4("gView", &view);
		}
	}
}
void ShaderProgram::SetProjectionAll(const Matrix44& projection)
{
	for (std::map<ShaderFileLocations, ShaderProgram*>::iterator it = s_shaderPrograms.begin(); it != s_shaderPrograms.end(); it++)
	{
		if (it->second != nullptr)
		{
			it->second->SetProgramValueMatrix4by4("gProj", &projection);
		}
	}
}
void ShaderProgram::SetViewAll(const Matrix44& view)
{
	for (std::map<ShaderFileLocations, ShaderProgram*>::iterator it = s_shaderPrograms.begin(); it != s_shaderPrograms.end(); it++)
	{
		if (it->second != nullptr)
		{
			it->second->SetProgramValueMatrix4by4("gView", &view);
		}
	}
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Use
void ShaderProgram::SetUseThisProgram() const
{
	g_Renderer->SetUseShaderProgram(m_shaderProgID);
}

void ShaderProgram::SetUseProgramNull() const
{
	g_Renderer->SetUseShaderProgram(nullVal);
}
void ShaderProgram::SingleTextureDrawShaderProgram(const PrimitiveType& drawMode, const unsigned int& vaoID, const unsigned int& numOfIndicesToDraw, const unsigned int& textureIndex, const unsigned int& textureID, const unsigned int& samplerID) const
{
	drawMode;
	g_Renderer->ActivateTexture(textureIndex, textureID, samplerID);

	g_Renderer->ActivateVAO(vaoID);
	g_Renderer->SetUseShaderProgram(m_shaderProgID);
	g_Renderer->DrawElements(PRIMITIVE_TRIANGLES, SHORT_, numOfIndicesToDraw, 0);

	g_Renderer->SetUseShaderProgram(NULL);
	g_Renderer->ActivateVAO(NULL);

	g_Renderer->UnbindTexture(textureIndex);
	g_Renderer->BindSampler(NULL);
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Getters
const void ShaderProgram::FindAndStoreBind(const std::string& valueName)
{
	SetUseThisProgram();
	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	SetUseProgramNull();
	if (loc >= 0)
	{
		AddBindToShaderProgram(valueName, loc);
	}
}
const unsigned int ShaderProgram::GetShaderProgramID()
{ 
	return m_shaderProgID;
}
const bool ShaderProgram::GetLoadedSuccessfully()
{ 
	return m_loadedSuccessfully; 
}
const ShaderFileLocations ShaderProgram::GetShaderFileLocations() const
{
	return fileLocations;
}


//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Setters
const bool ShaderProgram::SetProgramValueMatrix4by4(const std::string& valueName, const Matrix44* mat, const bool& transposeMatrix, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariablesLocToValue4By4Matrix(foundBind.bindLocation, mat, transposeMatrix, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariablesLocToValue4By4Matrix(loc, mat, transposeMatrix, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();

	return false;
}
const bool ShaderProgram::SetProgramValueRGBA(const std::string& valueName, const Rgba* color, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariableLocToValueFourFloatPointer(foundBind.bindLocation, (const Vector4*)color, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariableLocToValueFourFloatPointer(loc, (const Vector4*)color, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();
	return false;
}

const bool ShaderProgram::SetProgramValueVector4(const std::string& valueName, const Vector4* vec4, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariableLocToValueFourFloatPointer(foundBind.bindLocation, vec4, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariableLocToValueFourFloatPointer(loc, vec4, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();
	return false;
}
const bool ShaderProgram::SetProgramValueVector3(const std::string& valueName, const Vector3* vec3, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariableLocToValueThreeFloatPointer(foundBind.bindLocation, vec3, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariableLocToValueThreeFloatPointer(loc, vec3, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();
	return false;
}
//const bool ShaderProgram::SetProgramValueEulerAngles(const std::string& valueName, const EulerAngles* euler, const unsigned int& numOfElements) const
//{
//	SetUseThisProgram();
//	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
//	if (loc >= 0 && numOfElements > 0)
//	{
//		g_Renderer->SetVariableLocToValueThreeFloatPointer(loc, (const float*)euler, numOfElements);
//		SetUseProgramNull();
//		return true;
//	}
//	SetUseProgramNull();
//	return false;
//}
const bool ShaderProgram::SetProgramValueVector2(const std::string& valueName, const Vector2* vec2, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariableLocToValueTwoFloatPointer(foundBind.bindLocation, vec2, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariableLocToValueTwoFloatPointer(loc, vec2, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();
	return false;
}
const bool ShaderProgram::SetProgramValueFloat(const std::string& valueName, const float* flo, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariableLocToValueOneFloatPointer(foundBind.bindLocation, (const float*)flo, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariableLocToValueOneFloatPointer(loc, (const float*)flo, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();
	return false;
}

const bool ShaderProgram::SetProgramValueBool(const std::string& valueName, const bool* boo, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariableLocToValueOneIntPointer(foundBind.bindLocation, (const int*)boo, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariableLocToValueOneIntPointer(loc, (const int*)boo, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();
	return false;
}
const bool ShaderProgram::SetProgramValueInt(const std::string& valueName, const int* integer, const unsigned int& numOfElements) const
{
	SetUseThisProgram();
	bindFoundOrNot foundBind = FindBind(valueName);
	if (foundBind.bindFound == true)
	{
		g_Renderer->SetVariableLocToValueOneIntPointer(foundBind.bindLocation, (const int*)integer, numOfElements);
		SetUseProgramNull();
		return true;
	}

	int loc = g_Renderer->GetUniformLocationFromShaderProgramByName(m_shaderProgID, valueName);
	if (loc >= 0 && numOfElements > 0)
	{
		g_Renderer->SetVariableLocToValueOneIntPointer(loc, (const int*)integer, numOfElements);
		InternalAddBind((ShaderProgram*)this, valueName, loc);
		SetUseProgramNull();
		return true;
	}
	SetUseProgramNull();
	return false;
}
const bool ShaderProgram::SetSkeleton(const std::string& valueName, const Skeleton* skele, const bool& debug) const
{
	if (skele == nullptr || (skele != nullptr && skele->GetJointCount() > 200))
	{
		return false;
	}
	unsigned int numOfElements = skele->GetJointCount();
	std::vector<Matrix44> matrices = (skele->GetBoneSpaceTransforms());
	Matrix44* dat = matrices.data();

	if (debug == true)
	{
		Rgba pointColor = skele->GetPointColor();
		Rgba lineColor = skele->GetLineColor();
		SetProgramValueRGBA("gSkelePointColor", &pointColor, 1);
		SetProgramValueRGBA("gSkeleLineColor", &lineColor, 1);
	}

	return SetProgramValueMatrix4by4(valueName, dat, true, numOfElements);
}
void ShaderProgram::DeletAllShaderPrograms()
{
	for (std::map<ShaderFileLocations, ShaderProgram*>::iterator it = s_shaderPrograms.begin(); it != s_shaderPrograms.end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	s_shaderPrograms.clear();
	s_defaultShaderProgram = nullptr;
}


const void ShaderProgram::AddBindToShaderProgram(const std::string& valueName, const int& bindLocation)
{
	m_bindLocations.insert(std::pair<std::string, int>(valueName, bindLocation));
}

const bindFoundOrNot ShaderProgram::FindBind(const std::string& bindLocation) const
{
	std::map<std::string, int>::const_iterator it = m_bindLocations.find(bindLocation);
	bindFoundOrNot found;
	if (it == m_bindLocations.end())
	{
		found.bindFound = false;
		return found;
	}
	found.bindFound = true;
	found.bindLocation = it->second;
	return found;
}

const void ShaderProgram::InternalAddBind(ShaderProgram* prog, const std::string& bindName, const int& bindLoc)
{
	prog->AddBindToShaderProgram(bindName, bindLoc);
}