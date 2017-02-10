#pragma once

#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include "Engine/Renderer/FrameBuffer.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Matrix44.hpp"

class ParticleSystem2D;
class Sprite;

struct LayerInformation
{
	//Variables
	size_t m_layerID;
	AABB2 m_boundingVolume;
	Vector2 m_size;
	bool m_enabled;
	bool m_useCustomCameraPositionAndOrientation;
	Vector2 m_customCameraPosition;
	float m_CustomCameraYaw;
	Matrix44 m_layerView;


	//Constructors
	LayerInformation(const size_t& id = 0, const AABB2& boundingVolume = AABB2::ZERO_TO_ONE, const Vector2& size = Vector2::vec2_ones, bool useCustomCamPos = false, Vector2 camPos = Vector2(0.f, 0.f), float camYaw = 0.f);
};

class SpriteRenderer
{
private:
	FrameBuffer* current_surface;
	FrameBuffer* effect_surface;
	std::vector<LayerInformation> m_layerInformations;
	std::vector<std::vector<Sprite*>> m_SpritesToRenderByLayer;
	std::vector<std::vector<ParticleSystem2D*>> m_particleSystemsByLayer;
	std::vector<SpriteVert> m_axisLines;
	std::vector<unsigned int> m_axisLineIndices;

	Mesh* m_axisLinesMesh;
	Material* m_axisLinesMaterial;

	bool m_enableEffectRendering;
	bool m_enableDebugLineRendering;
	Matrix44 m_Projection;
	Matrix44 m_View;
	Vector2 m_virtualScreen;
	Vector3 m_nearView;
	Vector3 m_farView;
	Vector3 m_cameraPosition;
	size_t m_totalNumberOfActiveParticles;
	float m_virtualSize;
	float m_debugLineThickness;
	float m_cameraYaw;
	int colorBind;
	int depthBind;

	Material* m_material;
	Mesh* m_mesh;

	Rgba m_clearColor;
public:
	static SpriteRenderer* g_SpriteRenderer;

	//Console Command Specific
	static void ConsoleToggleLayerOnOff(Command&);
	static void PrintTotalNumberOfActiveParticles(Command&);

	//Console Held Command Specific
	static void HelpToggleLayerOnOff(Command&);
	static void HelpPrintTotalNumberOfActiveParticles(Command&);

	//Constructors
	SpriteRenderer(const bool& enableEffectRendering = false, const size_t& numOfLayers = 3);
	~SpriteRenderer();

private:
	void SetUpFramebuffers();

public:

	//Sprite Registration.
	void RegisterSprite(Sprite* sprite);
	void UnregisterSprite(Sprite* sprite);

	//Particle System Registration
	void RegisterParticleSystem(ParticleSystem2D* particleSystem);
	void UnregisterParticleSystem(ParticleSystem2D* particleSystem);

	//Setters and Adders
	void CreateEmptyLayer(const size_t& layerID);
	void AddLayerEffect(const size_t& layerID);
	void SetUseCustomCameraPositionForLayer(const size_t& layerID, bool useCustomCamPos, Vector2 camPos = Vector2::vec2_zeros, float CamYaw = 0.f);
	void SetClearScreenColor(const Vector3& color);
	void SetClearScreenColor(const Rgba& color);
	void ToggleLayerOnOff(const size_t& layerID, const bool& printToConsole = false);
	void EnableLayerOnOff(const size_t& layerID, const bool& enable = true);
	void SetFrameBufferShader(ShaderProgram* prog);
	void SetCameraPosition(const Vector2& camPos);
	void SetCameraRotation(const float& rotation);
	void SetDebugAxisLinesShader(ShaderProgram* prog);
	void SetTileCountIntendedForScreen(const Vector2& tileCountForScreen);
	void SetVirtualSize(const float& size);
	void EnableDebugLines(const bool& enableDebugLines);

	//Getters
	const bool DoesLayerExist(const size_t& id) const;
	const Matrix44 GetProjection() const;
	const Matrix44 GetView() const;
	const Vector2 GetVirtualScreen() const;
	const Vector3 GetCameraPosition() const;
	const float GetVirtualSize() const;
	const float GetDebugLineThickness() const;
	const float GetCameraRotation() const;
	const Rgba GetScreenClearColor() const;
	const size_t GetTotalNumberOfActiveParticles() const;

	//Updates
	void Update(const float& deltaSeconds);

	//Renders
	void RenderFrame();
	void RenderLayer(const size_t& layerID);

	//static methods
	static void SetScreenResolution(const Vector2& screenResolution);
	static void SetImportSize(const float& importSize);
	static void SetScreenSize(const float& screenSize);
	static const Vector2 GetScreenResolution();
	static const float GetImportSize();
	static const float GetScreenSize();
	static const float GetInverseAspectRatio();
	static const float GetAspectRatio();


private:
	//private methods
	void Calc2DViewMath();
	void SetUp2DViewRender() const;
	void UpdateAxisLines();
	void VerifySpriteNotCoveredByAnotherSpriteInAboveLayers(Sprite* sprite, const size_t& layedID);
};
#endif