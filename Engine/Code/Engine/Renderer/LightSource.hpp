#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Renderer/Shaders/ShaderProgram.hpp"
#include "Engine/Math/Vertices/Vertex_PCT.hpp"
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"

#ifndef LIGHTSOURCE_HPP
#define LIGHTSOURCE_HPP

enum LightSourceType
{
	INVALID_LIGHT_SOURCE_TYPE = -1,
	CUSTOM_LIGHT_TYPE,
	GLOBAL_POINT_LIGHT,
	LOCAL_POINT_LIGHT,
	LOCAL_SPOTLIGHT,
	GLOBAL_SPOTLIGHT,
	GLOBAL_DIRECT_LIGHT,
	NUMBER_OF_LIGHT_SOURCE_TYPES
};

class LightSource
{
private:
	//Common LightSource bind names
	static const std::string s_gLightPosition;
	static const std::string s_gLightDirection;
	static const std::string s_gLightColor;
	static const std::string s_gLightDirectionFactor;
	static const std::string s_gNearDistance;
	static const std::string s_gFarDistance;
	static const std::string s_gNearFactor;
	static const std::string s_gFarFactor;
	static const std::string s_gInnerAngle;
	static const std::string s_gOuterAngle;
	static const std::string s_gInnerFactor;
	static const std::string s_gOuterFactor;

public:
	//Constructors
	LightSource();
	LightSource(const Vector3& position, const Vector3& forwardDir, Vector3& lightColor, const float& dirFactor, const float& nearDistance, const float& farDistance,
		const float& nearFactor, const float& farFactor, const float& innerAng, const float& outerAng, const float& innerFact, const float& outerFact,
		LightSourceType lightType = CUSTOM_LIGHT_TYPE); //Global Point Light Definition.
	LightSource(const LightSource& other);
	~LightSource();


	//Setters
	void SetPosition(const Vector3& position) { m_position = position; };
	void SetDirection(const Vector3& direction) { m_direction = direction; };
	void SetLightColor(const Vector3& lightColor) { m_lightColor = lightColor; };
	void SetDirectionFactor(const float& directionFactor) { m_lightDirectionPower = directionFactor; };
	void SetNearDistance(const float& nearDistance) { m_nearDist = nearDistance; };
	void SetFarDistance(const float& farDistance) { m_farDist = farDistance; };
	void SetNearFactor(const float& nearFactor) { m_nearLightPower = nearFactor; };
	void SetFarFactor(const float& farFactor) { m_farLightPower = farFactor; };
	void SetInnerAngle(const float& innerAngle) { m_innerAngle = innerAngle; };
	void SetOuterAngle(const float& outerAngle) { m_outerAngle = outerAngle; };
	void SetInnerFactor(const float& innerFactor) { m_innerLightPower = innerFactor; };
	void SetOuterFactor(const float& outerFactor) { m_outerLightPower = outerFactor; };
	void SetLightSourceType(const LightSourceType& type) { m_type = type; };

	//Getters
	const Vector3 GetPosition() const {	return m_position; };
	const Vector3 GetDirection() const { return m_direction; };
	const Vector3 GetLightColor() const { return m_lightColor; };
	const float GetLightDirectionPower() const { return m_lightDirectionPower; };
	const float GetNearDistance() const { return m_nearDist; };
	const float GetFarDistance() const { return m_farDist; };
	const float GetNearLightPower() const { return m_nearLightPower; };
	const float GetFarLightPower() const { return m_farLightPower; };
	const float GetInnerAngle() const { return m_innerAngle; };
	const float GetOuterAngle() const { return m_outerAngle; };
	const float GetInnerFactor() const { return m_innerLightPower; };
	const float GetOuterFactor() const { return m_outerLightPower; };
	const LightSourceType GetLightSourceType() const { return m_type; };

	//Render
	void Update();
	void RenderAsPoint() const;

	//Shader
	static const void FindAllCommonBindsOfLightsForShaderProgram(ShaderProgram*& inShaderProg);
	const void LoadIntoShaderAsSingleLight(const ShaderProgram* inShaderProg) const;
	static const void LoadMultipleLightsIntoShader(ShaderProgram*& shaderProg, const std::vector<LightSource*>& lightSources);

	//Light Creators
	static LightSource* CreateGlobalPointLight(const Vector3& position, const Rgba& color, const float& radius);
	static LightSource* CreateLocalPointLight(const Vector3& position, const Rgba& color, const float& innerRadius, const float& outerRadius);
	static LightSource* CreateLocalSpotlight(const Vector3& position, const Vector3& forwardDirection, const Rgba& color, const float& nearRadius, const float& farRadius,
		const float& farFactor, const float& nearFactor, const float& innerAngle, const float& outerAngle, const float& innerAngleFactor, const float& outerAngleFactor);
	static LightSource* CreateGlobalSpotlight(const Vector3& position, const Vector3& forwardDirection, const Rgba& color, const float& radius,
		const float& innerAngle, const float& outerAngle);
	static LightSource* CreateDirectionalLight(const Vector3& position, const Vector3& direction, const float& dist, const Rgba& color);

	static void ClearAllLightSourceDataFromStaticVectors();
private:
	//variables
	LightSourceType m_type;
	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_lightColor;
	float m_lightDirectionPower;
	float m_nearDist;
	float m_farDist;
	float m_nearLightPower;
	float m_farLightPower;
	float m_innerAngle;
	float m_outerAngle;
	float m_innerLightPower;
	float m_outerLightPower;
	std::vector<Vertex_PCT> vertice;


	static std::vector<Vector3, TUntrackedAllocator<Vector3>> positions;
	static std::vector<Vector3, TUntrackedAllocator<Vector3>> directions;
	static std::vector<Vector3, TUntrackedAllocator<Vector3>> lightColors;
	static std::vector<float, TUntrackedAllocator<float>> directionPowers;
	static std::vector<float, TUntrackedAllocator<float>> nearDistances;
	static std::vector<float, TUntrackedAllocator<float>> farDistances;
	static std::vector<float, TUntrackedAllocator<float>> nearPowers;
	static std::vector<float, TUntrackedAllocator<float>> farPowers;
	static std::vector<float, TUntrackedAllocator<float>> innerAngles;
	static std::vector<float, TUntrackedAllocator<float>> outerAngles;
	static std::vector<float, TUntrackedAllocator<float>> innerPowers;
	static std::vector<float, TUntrackedAllocator<float>> outerPowers;
};
#endif

/*
// Arrays are referenced by "gLightPosition[0]", not "gLightPosition"
uniform vec3 gLightPosition[16];
uniform vec3 gLightDirection[16];
uniform vec3 gLightColor[16];
uniform float gLightDirectionFactor[16];

// Attenutation Values
uniform float gNearDistance[16];
uniform float gFarDistance[16];
uniform float gNearFactor[16];
uniform float gFarFactor[16];

uniform float gInnerAngle[16];
uniform float gOuterAngle[16];
uniform float gInnerFactor[16];
uniform float gOuterFactor[16];
*/