#include "Engine/Renderer/LightSource.hpp"
#include "Engine/Renderer/Renderer.hpp"


//Common LightSource bind names
const std::string LightSource::s_gLightPosition = "gLightPosition[0]";
const std::string LightSource::s_gLightDirection = "gLightDirection[0]";
const std::string LightSource::s_gLightColor = "gLightColor[0]";
const std::string LightSource::s_gLightDirectionFactor = "gLightDirectionFactor[0]";
const std::string LightSource::s_gNearDistance = "gNearDistance[0]";
const std::string LightSource::s_gFarDistance = "gFarDistance[0]";
const std::string LightSource::s_gNearFactor = "gNearFactor[0]";
const std::string LightSource::s_gFarFactor = "gFarFactor[0]";
const std::string LightSource::s_gInnerAngle = "gInnerAngle[0]";
const std::string LightSource::s_gOuterAngle = "gOuterAngle[0]";
const std::string LightSource::s_gInnerFactor = "gInnerFactor[0]";
const std::string LightSource::s_gOuterFactor = "gOuterFactor[0]";

std::vector<Vector3, TUntrackedAllocator<Vector3>> LightSource::positions;
std::vector<Vector3, TUntrackedAllocator<Vector3>> LightSource::directions;
std::vector<Vector3, TUntrackedAllocator<Vector3>> LightSource::lightColors;
std::vector<float, TUntrackedAllocator<float>> LightSource::directionPowers;
std::vector<float, TUntrackedAllocator<float>> LightSource::nearDistances;
std::vector<float, TUntrackedAllocator<float>> LightSource::farDistances;
std::vector<float, TUntrackedAllocator<float>> LightSource::nearPowers;
std::vector<float, TUntrackedAllocator<float>> LightSource::farPowers;
std::vector<float, TUntrackedAllocator<float>> LightSource::innerAngles;
std::vector<float, TUntrackedAllocator<float>> LightSource::outerAngles;
std::vector<float, TUntrackedAllocator<float>> LightSource::innerPowers;
std::vector<float, TUntrackedAllocator<float>> LightSource::outerPowers;

//---------------------------------------------------------------------------------------------
//Constructors
LightSource::LightSource()
	: m_type(INVALID_LIGHT_SOURCE_TYPE),
	m_position(0.f, 0.f, 0.f),
	m_direction(1.f, 0.f, 0.f),
	m_lightColor(0.f,0.f,0.f),
	m_lightDirectionPower(0.f),
	m_nearDist(0.f),
	m_farDist(0.f),
	m_nearLightPower(0.f),
	m_farLightPower(0.f),
	m_innerAngle(0.f),
	m_outerAngle(0.f),
	m_innerLightPower(0.f),
	m_outerLightPower(0.f)
{
}
LightSource::LightSource(const Vector3& position, const Vector3& forwardDir, Vector3& lightColor, const float& dirFactor, const float& nearDistance, const float& farDistance,
	const float& nearFactor, const float& farFactor, const float& innerAng, const float& outerAng, const float& innerFact, const float& outerFact,
	LightSourceType lightType)
	: m_type(lightType),
	m_position(position),
	m_direction(forwardDir),
	m_lightColor(lightColor),
	m_lightDirectionPower(dirFactor),
	m_nearDist(nearDistance),
	m_farDist(farDistance),
	m_nearLightPower(nearFactor),
	m_farLightPower(farFactor),
	m_innerAngle(innerAng),
	m_outerAngle(outerAng),
	m_innerLightPower(innerFact),
	m_outerLightPower(outerFact)
{
}
LightSource::LightSource(const LightSource& other)
	: m_type(other.m_type),
	m_position(other.m_position),
	m_direction(other.m_direction),
	m_lightColor(other.m_lightColor),
	m_lightDirectionPower(other.m_lightDirectionPower),
	m_nearDist(other.m_nearDist),
	m_farDist(other.m_farDist),
	m_nearLightPower(other.m_nearLightPower),
	m_farLightPower(other.m_farLightPower),
	m_innerAngle(other.m_innerAngle),
	m_outerAngle(other.m_outerAngle),
	m_innerLightPower(other.m_innerLightPower),
	m_outerLightPower(other.m_outerLightPower)
{
}
LightSource::~LightSource()
{
	vertice.clear();
}

//----------------------------------------------------------------------------------------------
//Render
void LightSource::Update()
{
	if (vertice.size() == 0)
	{
		vertice.push_back(Vertex_PCT(m_position, Rgba(m_lightColor.x, m_lightColor.y, m_lightColor.z, 1.f), Vector2(0.f, 0.f)));
	}
	else
	{
		vertice[0].m_pos = m_position;
		vertice[0].m_color.Red = m_lightColor.x;
		vertice[0].m_color.Green = m_lightColor.y;
		vertice[0].m_color.Blue = m_lightColor.z;
	}
}
void LightSource::RenderAsPoint() const
{

	g_Renderer->setPointSize(10.f);
	g_Renderer->DrawVertexArray(vertice, PRIMITIVE_POINTS, nullptr);
}

//-----------------------------------------------------------------------------------------------
//Shader
const void LightSource::FindAllCommonBindsOfLightsForShaderProgram(ShaderProgram*& inShaderProg)
{
	if (inShaderProg == nullptr)
	{
		return;
	}
	inShaderProg->FindAndStoreBind("gLightPosition");
	inShaderProg->FindAndStoreBind("gLightDirection");
	inShaderProg->FindAndStoreBind("gLightColor");
	inShaderProg->FindAndStoreBind("gLightDirectionFactor");
	inShaderProg->FindAndStoreBind("gNearDistance");
	inShaderProg->FindAndStoreBind("gFarDistance");
	inShaderProg->FindAndStoreBind("gInnerAngle");
	inShaderProg->FindAndStoreBind("gOuterAngle");
	inShaderProg->FindAndStoreBind("gInnerFactor");
	inShaderProg->FindAndStoreBind("gOuterFactor");
}
const void LightSource::LoadIntoShaderAsSingleLight(const ShaderProgram* inShaderProg) const
{
	if (inShaderProg == nullptr)
	{
		return;
	}
	inShaderProg->SetProgramValueVector3("gLightPosition", &m_position);
	inShaderProg->SetProgramValueVector3("gLightDirection", &m_direction);
	inShaderProg->SetProgramValueVector3("gLightColor", &m_lightColor);
	inShaderProg->SetProgramValueFloat("gLightDirectionFactor", &m_lightDirectionPower);
	inShaderProg->SetProgramValueFloat("gNearDistance", &m_nearDist);
	inShaderProg->SetProgramValueFloat("gFarDistance", &m_farDist);
	inShaderProg->SetProgramValueFloat("gInnerAngle", &m_innerAngle);
	inShaderProg->SetProgramValueFloat("gOuterAngle", &m_outerAngle);
	inShaderProg->SetProgramValueFloat("gInnerFactor", &m_innerLightPower);
	inShaderProg->SetProgramValueFloat("gOuterFactor", &m_outerLightPower);
}
const void LightSource::LoadMultipleLightsIntoShader(ShaderProgram*& shaderProg, const std::vector<LightSource*>& lightSources)
{
	if (shaderProg == nullptr)
	{
		return;
	}
	size_t numberOfElements = lightSources.size();
	size_t trueNumElements = numberOfElements;
	if (positions.size() != numberOfElements)
	{
		positions.clear();
		directions.clear();
		lightColors.clear();
		directionPowers.clear();
		nearDistances.clear();
		farDistances.clear();
		nearPowers.clear();
		farPowers.clear();
		innerAngles.clear();
		outerAngles.clear();
		innerPowers.clear();
		outerPowers.clear();

		for (size_t index = 0; index < numberOfElements; index++)
		{
			LightSource* curLight = lightSources.at(index);
			if (curLight != nullptr)
			{
				positions.push_back(curLight->GetPosition());
				directions.push_back(curLight->GetDirection());
				lightColors.push_back(curLight->GetLightColor());
				directionPowers.push_back(curLight->GetLightDirectionPower());
				nearDistances.push_back(curLight->GetNearDistance());
				farDistances.push_back(curLight->GetFarDistance());
				nearPowers.push_back(curLight->GetNearLightPower());
				farPowers.push_back(curLight->GetFarLightPower());
				innerAngles.push_back(curLight->GetInnerAngle());
				outerAngles.push_back(curLight->GetOuterAngle());
				innerPowers.push_back(curLight->GetInnerFactor());
				outerPowers.push_back(curLight->GetOuterFactor());
			}
			else
			{
				trueNumElements--;
			}
		}
	}
	else
	{
		for (size_t index = 0; index < numberOfElements; index++)
		{
			LightSource* curLight = lightSources.at(index);
			if (curLight != nullptr)
			{
				positions[index] = (curLight->GetPosition());
				directions[index] = (curLight->GetDirection());
				lightColors[index] = (curLight->GetLightColor());
				directionPowers[index] = (curLight->GetLightDirectionPower());
				nearDistances[index] = (curLight->GetNearDistance());
				farDistances[index] = (curLight->GetFarDistance());
				nearPowers[index] = (curLight->GetNearLightPower());
				farPowers[index] = (curLight->GetFarLightPower());
				innerAngles[index] = (curLight->GetInnerAngle());
				outerAngles[index] = (curLight->GetOuterAngle());
				innerPowers[index] = (curLight->GetInnerFactor());
				outerPowers[index] = (curLight->GetOuterFactor());
			}
			else
			{
				trueNumElements--;
			}
		}
	}

	shaderProg->SetProgramValueVector3(s_gLightPosition, &positions[0], trueNumElements);
	shaderProg->SetProgramValueVector3(s_gLightDirection, &directions[0], trueNumElements);
	shaderProg->SetProgramValueVector3(s_gLightColor, &lightColors[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gLightDirectionFactor, &directionPowers[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gNearDistance, &nearDistances[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gFarDistance, &farDistances[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gNearFactor, &nearPowers[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gFarFactor, &farPowers[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gInnerAngle, &innerAngles[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gOuterAngle, &outerAngles[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gInnerFactor, &innerPowers[0], trueNumElements);
	shaderProg->SetProgramValueFloat(s_gOuterFactor, &outerPowers[0], trueNumElements);
}
//---------------------------------------------------------------------------------------------
//Light Creators

LightSource* LightSource::CreateGlobalPointLight(const Vector3& position, const Rgba& colorAndAlphaIsBrightness, const float& radius)
{
	/*
	LightSource(const Vector3& position, 
	const Vector3& forwardDir, 
	Vector3& lightColor, 
	const float& dirFactor, 
	const float& nearDistance, 
	const float& farDistance,
	const float& nearFactor, 
	const float& farFactor, 
	const float& innerAng, 
	const float& outerAng, 
	const float& innerFact, 
	const float& outerFact,
	LightSourceType lightType)
	*/
	Vector3 rgb = colorAndAlphaIsBrightness.GetRGB();
	LightSource* newLight = new LightSource(position, //position
		Vector3(0.f, 0.f, 0.f), //Forward
		rgb, //color
		0.f, //DirPower
		radius - 1.f, //nearDist
		radius + 1.f, //farDist
		1.f, //nearPower
		1.f, //farPower
		1.f, //innerAng
		-1.f, //outerAng
		1.f, //innerPower
		1.f, //outerPower
		GLOBAL_POINT_LIGHT);
	return newLight;
}

LightSource* LightSource::CreateLocalPointLight(const Vector3& position, const Rgba& colorAndAlphaIsBrightness, const float& innerRadius, const float& outerRadius)
{
	Vector3 rgb = colorAndAlphaIsBrightness.GetRGB();
	LightSource* newLight = new LightSource(position, //position
		Vector3(0.f, 0.f, 0.f), //Forward
		rgb, //color
		0.f, //DirPower
		innerRadius, //nearDist
		outerRadius, //farDist
		1.f, //nearPower
		0.f, //farPower
		1.f, //innerAng
		-1.f, //outerAng
		1.f, //innerPower
		0.f, //outerPower
		LOCAL_POINT_LIGHT);
	return newLight;
}
LightSource* LightSource::CreateLocalSpotlight(const Vector3& position, const Vector3& forwardDirection, const Rgba& color, const float& nearRadius, const float& farRadius,
	const float& farFactor, const float& nearFactor, const float& innerAngle, const float& outerAngle, const float& innerAngleFactor, const float& outerAngleFactor)
{
	Vector3 rgb = color.GetRGB();
	LightSource* newLight = new LightSource(position,
		forwardDirection,
		rgb,
		1.f,
		nearRadius,
		farRadius,
		nearFactor,
		farFactor, 
		innerAngle,
		outerAngle, 
		innerAngleFactor,
		outerAngleFactor,
		LOCAL_SPOTLIGHT);
	return newLight;
}

LightSource* LightSource::CreateDirectionalLight(const Vector3& position, const Vector3& direction, const float& dist, const Rgba& color)
{
	Vector3 rgb = (color.GetRGB());
	LightSource* newLight = new LightSource(position,
		direction,
		rgb,
		1.f,
		0.f,
		dist,
		0.f,
		dist,
		1.f,
		-1.f,
		1.f,
		1.f,
		GLOBAL_DIRECT_LIGHT);
	return newLight;
}
LightSource* LightSource::CreateGlobalSpotlight(const Vector3& position, const Vector3& forwardDirection, const Rgba& color, const float& radius,
	const float& innerAngle, const float& outerAngle)
{
	Vector3 rgb = (color.GetRGB());
	LightSource* newLight = new LightSource(position,
		forwardDirection,
		rgb,
		1.f,
		radius,
		radius,
		1.f,
		1.f,
		innerAngle,
		outerAngle,
		1.f,
		1.f,
		GLOBAL_SPOTLIGHT);
	return newLight;
}


void LightSource::ClearAllLightSourceDataFromStaticVectors()
{
	positions.clear();
	directions.clear();
	lightColors.clear();
	directionPowers.clear();
	nearDistances.clear();
	farDistances.clear();
	nearPowers.clear();
	farPowers.clear();
	innerAngles.clear();
	outerAngles.clear();
	innerPowers.clear();
	outerPowers.clear();
}