#include "Engine/2D/SpriteRenderer.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/2D/Sprite.hpp"
#include "Engine/2D/ParticleSystem2D.hpp"
#include "Engine/Math/ExtendedMath.hpp"


LayerInformation::LayerInformation(const size_t& id, const AABB2& boundingVolume, const Vector2& size, bool useCustomCamPos, Vector2 camPos, float camYaw)
	: m_layerID(id),
	m_enabled(true),
	m_boundingVolume(boundingVolume),
	m_size(size),
	m_useCustomCameraPositionAndOrientation(useCustomCamPos),
	m_customCameraPosition(camPos),
	m_CustomCameraYaw(camYaw)
{

}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Console Command Specific
void SpriteRenderer::ConsoleToggleLayerOnOff(Command& com)
{
	if (g_SpriteRenderer == nullptr)
	{
		g_Console->ConsolePrint("No global sprite renderer object exists!");
		return;
	}
	std::string layer = com.get_string_by_index(0);
	if (SimpleStrCmp(layer, "") == true)
	{
		g_Console->ConsolePrint("Can not toggle; no layer id defined!");
		return;
	}
	int layerIDI = atoi(layer.c_str());
	if (layerIDI < 0)
	{
		g_Console->ConsolePrint("Can not toggle; layer id is less than 0!");
		return;
	}

	size_t layerID = (size_t)layerIDI;
	g_SpriteRenderer->ToggleLayerOnOff(layerID, true);
}

void SpriteRenderer::PrintTotalNumberOfActiveParticles(Command&)
{
	if (g_SpriteRenderer != nullptr)
	{
		g_Console->ConsolePrint("Total number of Active Particles: " + std::to_string(g_SpriteRenderer->GetTotalNumberOfActiveParticles()));
	}
	else
	{
		g_Console->ConsolePrintf(WARNING, "Cant print number of active particles; g_SpriteRenderer is null!");
	}
}


//Console Held Command Specific
void SpriteRenderer::HelpToggleLayerOnOff(Command&)
{
	g_Console->ConsolePrint("toggleSpriteLayer takes one unsigned integer value.");
	g_Console->ConsolePrint("It then searches g_spriteRenderer\'s layers for one that matches that ID");
	g_Console->ConsolePrint("and toggles it on or off.");
}

void SpriteRenderer::HelpPrintTotalNumberOfActiveParticles(Command&)
{
	g_Console->ConsolePrint("PrintTotalNumberOfActiveParticles2D checks with");
	g_Console->ConsolePrint("g_SpriteRenderer to find out how many particles are");
	g_Console->ConsolePrint("Currently Active on screen via it\'s currently registered");
	g_Console->ConsolePrint("ParticleSystems.");
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors

SpriteRenderer* SpriteRenderer::g_SpriteRenderer = nullptr;

SpriteRenderer::SpriteRenderer(const bool& enableEffectRendering, const size_t& numOfLayers)
	: current_surface(nullptr),
	effect_surface(nullptr),
	m_material(nullptr),
	m_mesh(nullptr),
	m_axisLinesMesh(nullptr),
	m_axisLinesMaterial(nullptr),
	m_clearColor(0.f, 0.f, 0.f, 1.f),
	m_enableEffectRendering(enableEffectRendering),
	m_nearView(-1.f, -1.f, -1.f),
	m_farView(1.f, 1.f, 1.f),
	m_cameraPosition(0.f, 0.f, 0.f),
	m_cameraYaw(0.f),
	m_debugLineThickness(0.1f),
	m_virtualScreen(1.f, 1.f),
	m_virtualSize(1.f),
	m_enableDebugLineRendering(false),
	m_totalNumberOfActiveParticles(0)
{
	static RegisterCommandHelper _PrintTotalNumberOfActiveParticles2D = RegisterCommandHelper("PrintTotalNumberOfActiveParticles2D", PrintTotalNumberOfActiveParticles, HelpPrintTotalNumberOfActiveParticles);
	static RegisterCommandHelper _toggleLayerByID = RegisterCommandHelper("toggleSpriteLayer", ConsoleToggleLayerOnOff, HelpToggleLayerOnOff);
	m_layerInformations.resize(numOfLayers);
	m_SpritesToRenderByLayer.resize(numOfLayers);
	m_particleSystemsByLayer.resize(numOfLayers);
	SetUpFramebuffers();
	m_axisLinesMesh = new Mesh();
	m_axisLinesMaterial = new Material();
	UpdateAxisLines();
}
SpriteRenderer::~SpriteRenderer()
{
	if (m_axisLinesMaterial != nullptr)
	{
		delete m_axisLinesMaterial;
		m_axisLinesMaterial = nullptr;
	}
	if (m_axisLinesMesh != nullptr)
	{
		delete m_axisLinesMesh;
		m_axisLinesMesh = nullptr;
	}
	if (current_surface != nullptr)
	{
		FramebufferDelete(current_surface);
		current_surface = nullptr;
	}
	if(effect_surface != nullptr)
	{
		FramebufferDelete(effect_surface);
		effect_surface = nullptr;
	}
	if (m_material != nullptr)
	{
		delete m_material;
		m_material = nullptr;
	}
	if (m_mesh != nullptr)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
}

void SpriteRenderer::SetUpFramebuffers()
{
	Texture** colors2 = new Texture*[1];
	Texture** colors = new Texture*[1];
	colors2[0] = new Texture(1600, 900, eTextureFormat_RGBA8);
	colors[0] = new Texture(1600, 900, eTextureFormat_RGBA8);
	Texture* stencil = new Texture(1600, 900, eTextureFormat_D24S8);
	Texture* stencil2 = new Texture(1600, 900, eTextureFormat_D24S8);

	m_material = new Material();
	m_mesh = new Mesh();
	std::vector<Vertex_PCTT> quad;
	quad.push_back(Vertex_PCTT(Vector3(-1.f, 1.f, 0.f), Rgba(1.f, 0.f, 1.f, 1.f), Vector2(0.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //TL
	quad.push_back(Vertex_PCTT(Vector3(-1.f, -1.f, 0.f), Rgba(0.f, 1.f, 1.f, 1.f), Vector2(0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //BL
	quad.push_back(Vertex_PCTT(Vector3(1.f, -1.f, 0.f), Rgba(1.f, 1.f, 0.f, 1.f), Vector2(1.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //BR
	quad.push_back(Vertex_PCTT(Vector3(1.f, 1.f, 0.f), Rgba(1.f, 1.f, 1.f, 1.f), Vector2(1.f, 0.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f))); //TR
	m_mesh->setVBO(quad);

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	m_mesh->setIBO(indices);

	unsigned int eh = 0;
	g_Renderer->GenerateVAO(eh);
	m_mesh->setVAOID(eh);
	current_surface = FramebufferCreate(1, colors, stencil);
	effect_surface = FramebufferCreate(1, colors2, stencil2);
	colorBind = 0;
	depthBind = 1;
	m_material->AddTexture(current_surface->get_color_texture(0), colorBind, "gTexDiffuse");
	m_material->AddTexture(current_surface->get_depth_stencil_target(), depthBind, "gTexDepth");

	unsigned int sampler = g_Renderer->CreateSampler(NEAREST_DISTANCE_TYPE, NEAREST_DISTANCE_TYPE, REPEAT_DISTANCE_TYPE, REPEAT_DISTANCE_TYPE);
	m_mesh->SetSamplerID(sampler);
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Sprite Registration.
void SpriteRenderer::RegisterSprite(Sprite* sprite)
{
	if (sprite == nullptr)
	{
		return;
	}
	size_t layer = sprite->GetLayerID();
	if (layer >= m_layerInformations.size())
	{
		m_layerInformations.resize(layer + 1);
		m_SpritesToRenderByLayer.resize(layer + 1);
		m_particleSystemsByLayer.resize(layer + 1);
	}

	for (size_t i = 0; i < m_SpritesToRenderByLayer.at(layer).size(); i++)
	{
		if (m_SpritesToRenderByLayer.at(layer).at(i) == sprite)
		{
			return;
		}
	}
	m_SpritesToRenderByLayer.at(layer).push_back(sprite);
}
void SpriteRenderer::UnregisterSprite(Sprite* sprite)
{
	if (sprite == nullptr)
	{
		return;
	}
	size_t layer = sprite->GetLayerID();
	if (layer >= m_layerInformations.size())
	{
		return;
	}

	for (size_t i = 0; i < m_SpritesToRenderByLayer.at(layer).size(); i++)
	{
		if (m_SpritesToRenderByLayer.at(layer).at(i) == sprite)
		{
			m_SpritesToRenderByLayer.at(layer).erase(m_SpritesToRenderByLayer.at(layer).begin() + i);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Particle System Registration
void SpriteRenderer::RegisterParticleSystem(ParticleSystem2D* particleSystem)
{
	if (particleSystem == nullptr)
	{
		return;
	}
	size_t layer = particleSystem->GetLayer();
	if (layer >= m_layerInformations.size())
	{
		m_layerInformations.resize(layer + 1);
		m_SpritesToRenderByLayer.resize(layer + 1);
		m_particleSystemsByLayer.resize(layer + 1);
	}

	for (size_t i = 0; i < m_particleSystemsByLayer.at(layer).size(); i++)
	{
		if (m_particleSystemsByLayer.at(layer).at(i) == particleSystem)
		{
			return;
		}
	}
	m_particleSystemsByLayer.at(layer).push_back(particleSystem);
}

void SpriteRenderer::UnregisterParticleSystem(ParticleSystem2D* particleSystem)
{
	if (particleSystem == nullptr)
	{
		return;
	}
	size_t layer = particleSystem->GetLayer();
	if (layer >= m_layerInformations.size())
	{
		m_layerInformations.resize(layer + 1);
		m_SpritesToRenderByLayer.resize(layer + 1);
		m_particleSystemsByLayer.resize(layer + 1);
	}
	for (size_t i = 0; i < m_particleSystemsByLayer.at(layer).size(); i++)
	{
		if (m_particleSystemsByLayer.at(layer).at(i) == particleSystem)
		{
			m_particleSystemsByLayer.at(layer).erase(m_particleSystemsByLayer.at(layer).begin() + i);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Setters and Adders
void SpriteRenderer::CreateEmptyLayer(const size_t& layerID)
{
	if (DoesLayerExist(layerID) == true)
	{
		std::string alreadyExistsWarning = "layer " + std::to_string(layerID) + " already exists!.\n";
		DebuggerPrintf(alreadyExistsWarning.c_str());
		return;
	}
	LayerInformation newLayer = LayerInformation(layerID);
	m_layerInformations.push_back(newLayer);
	m_SpritesToRenderByLayer.resize(m_layerInformations.size());
	m_particleSystemsByLayer.resize(m_layerInformations.size());
}
void SpriteRenderer::AddLayerEffect(const size_t& layerID)
{
	if (DoesLayerExist(layerID) == false)
	{
		CreateEmptyLayer(layerID);
	}
}

void SpriteRenderer::SetUseCustomCameraPositionForLayer(const size_t& layerID, bool useCustomCamPos, Vector2 camPos, float CamYaw)
{
	if (m_layerInformations.size() <= layerID)
	{
		m_layerInformations.resize((layerID + 1) * 2);
		m_SpritesToRenderByLayer.resize((layerID + 1) * 2);
		m_particleSystemsByLayer.resize((layerID + 1) * 2);
	}

	m_layerInformations.at(layerID).m_useCustomCameraPositionAndOrientation = useCustomCamPos;
	m_layerInformations.at(layerID).m_customCameraPosition = camPos;
	m_layerInformations.at(layerID).m_CustomCameraYaw = CamYaw;

}

void SpriteRenderer::SetClearScreenColor(const Vector3& color)
{
	SetClearScreenColor(Rgba(color.x, color.y, color.z, 1.f));
}
void SpriteRenderer::SetClearScreenColor(const Rgba& color)
{
	m_clearColor = color;
}

void SpriteRenderer::ToggleLayerOnOff(const size_t& layerID, const bool& printToConsole)
{
	for (size_t i = 0; i < m_layerInformations.size(); i++)
	{
		if (m_layerInformations.at(i).m_layerID == layerID)
		{
			m_layerInformations.at(i).m_enabled = !m_layerInformations.at(i).m_enabled;
			if (g_Console != nullptr && printToConsole == true)
			{
				if (m_layerInformations.at(i).m_enabled == true)
				{
					g_Console->ConsolePrint("SpriteLayer " + std::to_string(layerID) + " was toggled on.");
				}
				else
				{
					g_Console->ConsolePrint("SpriteLayer " + std::to_string(layerID) + " was toggled off.");
				}
			}
			return;
		}
	}
	if (g_Console != nullptr && printToConsole == true)
	{
		g_Console->ConsolePrint("Can not toggle; layer does not exist");
	}

}
void SpriteRenderer::EnableLayerOnOff(const size_t& layerID, const bool& enable)
{
	if (m_layerInformations.size() <= layerID)
	{
		m_layerInformations.resize((layerID + 1) * 2);
		m_SpritesToRenderByLayer.resize((layerID + 1) * 2);
		m_particleSystemsByLayer.resize((layerID + 1) * 2);
	}
	m_layerInformations.at(layerID).m_enabled = enable;
}

void SpriteRenderer::SetFrameBufferShader(ShaderProgram* prog)
{
	if (prog != nullptr)
	{
		prog->SetProgramValueInt("gTexDiffuse", &colorBind);
		prog->SetProgramValueInt("gTexDepth", &depthBind);
	}
	m_material->SetProgram(prog);
}
void SpriteRenderer::SetCameraPosition(const Vector2& camPos)
{
	m_cameraPosition = Vector3(camPos);
}
void SpriteRenderer::SetCameraRotation(const float& rotation)
{
	m_cameraYaw = rotation;
}
void SpriteRenderer::SetDebugAxisLinesShader(ShaderProgram* prog)
{
	m_axisLinesMaterial->SetProgram(prog);
}
void SpriteRenderer::SetTileCountIntendedForScreen(const Vector2& ortho)
{
	m_virtualScreen = ortho;
}
void SpriteRenderer::SetVirtualSize(const float& size)
{
	const float aspectRatio = GetAspectRatio();
	m_virtualSize = size;
	if (aspectRatio > 1.f)
	{
		m_virtualScreen.x = size * aspectRatio;
		m_virtualScreen.y = size;
	}
	else
	{
		m_virtualScreen.x = size;
		m_virtualScreen.y = size / aspectRatio;
	}
}
void SpriteRenderer::EnableDebugLines(const bool& enableDebugLines)
{
	m_enableDebugLineRendering = enableDebugLines;
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Getters
const bool SpriteRenderer::DoesLayerExist(const size_t& id) const
{

	for (size_t i = 0; i < m_layerInformations.size(); i++)
	{
		if (m_layerInformations.at(i).m_layerID == id)
		{
			return true;
		}
	}

	return false;
}
const Matrix44 SpriteRenderer::GetProjection() const
{
	return m_Projection;
}
const Matrix44 SpriteRenderer::GetView() const
{
	return m_View;
}
const Vector2 SpriteRenderer::GetVirtualScreen() const
{
	return m_virtualScreen;
}
const Vector3 SpriteRenderer::GetCameraPosition() const
{
	return m_cameraPosition;
}
const float SpriteRenderer::GetVirtualSize() const
{
	return m_virtualSize;
}
const float SpriteRenderer::GetDebugLineThickness() const
{
	return m_debugLineThickness;
}
const float SpriteRenderer::GetCameraRotation() const
{
	return m_cameraYaw;
}
const Rgba SpriteRenderer::GetScreenClearColor() const
{
	return m_clearColor;
}
const size_t SpriteRenderer::GetTotalNumberOfActiveParticles() const
{
	return m_totalNumberOfActiveParticles;
}


//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Updates
void SpriteRenderer::Update(const float& deltaSeconds)
{
	Calc2DViewMath();

	size_t numOfLayers = m_layerInformations.size();
	m_totalNumberOfActiveParticles = 0;
	for (size_t layerIdx = 0; layerIdx < numOfLayers; layerIdx++)
	{
		Vector2 camXY;
		if (m_layerInformations.at(layerIdx).m_useCustomCameraPositionAndOrientation)
		{
			m_layerInformations.at(layerIdx).m_layerView.MatrixMakeIdentity();
			Matrix44::MakeEulerRotation(m_layerInformations.at(layerIdx).m_layerView, DegreesToRadians(0.f), DegreesToRadians(-0.f),
				DegreesToRadians(m_layerInformations.at(layerIdx).m_CustomCameraYaw),
				Vector3(m_layerInformations.at(layerIdx).m_customCameraPosition.x, m_layerInformations.at(layerIdx).m_customCameraPosition.y, 0.f)); //
			Matrix44::MatrixInvertOrthogonal(m_layerInformations.at(layerIdx).m_layerView);

			camXY = m_layerInformations.at(layerIdx).m_customCameraPosition;
		}
		else
		{
			camXY = m_cameraPosition.GetXY();
		}
		Vector2 screenMaxs = camXY + (m_virtualScreen / 2.f);
		Vector2 screenMins = camXY - (m_virtualScreen / 2.f);
		for (size_t spriteIdx = 0; spriteIdx < m_SpritesToRenderByLayer.at(layerIdx).size(); spriteIdx++)
		{
			Sprite* sprite = m_SpritesToRenderByLayer.at(layerIdx).at(spriteIdx);
			if (sprite != nullptr)
			{
				Vector2 spriteCenter = sprite->GetPosition();
				Vector2 scale = sprite->GetTrueScale();
				Vector2 halfScale = scale / 2.f;
				Vector2 maxs = spriteCenter + halfScale;
				Vector2 mins = spriteCenter - halfScale;

				//screenbigger test
				if (((spriteCenter.y >= screenMins.y && spriteCenter.y <= screenMaxs.y)
					|| (maxs.y >= screenMins.y && maxs.y <= screenMaxs.y)
					|| (mins.y >= screenMins.y && mins.y <= screenMaxs.y))
					&& (spriteCenter.x >= screenMins.x && spriteCenter.x <= screenMaxs.x)
					|| (maxs.x >= screenMins.x && maxs.x <= screenMaxs.x)
					|| (mins.x >= screenMins.x && mins.x <= screenMaxs.x)
						)
				{
					sprite->SetOnScreen(true);
				}
				//screensmaller test
				else if (((screenMins.y <= maxs.y && screenMins.y >= mins.y)
					|| (screenMaxs.y <= maxs.y && screenMaxs.y >= mins.y)
					|| (camXY.y <= maxs.y && camXY.y >= mins.y))
					&& ((screenMins.x <= maxs.x && screenMins.x >= mins.x)
						|| (screenMaxs.x <= maxs.x && screenMaxs.x >= mins.x)
						|| (camXY.x <= maxs.x && camXY.x >= mins.x)
						))
				{
					sprite->SetOnScreen(true);
				}
				else
				{
					sprite->SetOnScreen(false);
				}
			}
		}
		for (size_t ParticleSystemIdx = 0; ParticleSystemIdx < m_particleSystemsByLayer.at(layerIdx).size(); ParticleSystemIdx++)
		{
			ParticleSystem2D* system = m_particleSystemsByLayer.at(layerIdx).at(ParticleSystemIdx);
			if (system != nullptr)
			{
				if (system->GetDestroyMe() == true)
				{
					delete system;
					system = nullptr;
					ParticleSystemIdx--;
					continue;
				}
				system->Update(deltaSeconds);
				m_totalNumberOfActiveParticles += system->GetTotalNumberOfParticlesInSystem();
			}
		}
	}
	ParticleSystemDefinition2D::s_ClearedParticleSystemDefinitions = false;

	//Matrix44::MatrixMakePerspective(m_Projection, 70.f, aspect, 0.1f, 1000.f); //3d
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//Renders
void SpriteRenderer::RenderFrame()
{
	g_Renderer->DepthTestingTrueFalse(false);
	//g_Renderer->FramebufferBind(current_surface);
	g_Renderer->clearScreen(m_clearColor, false);
	SetUp2DViewRender();
	
	bool lastLayerUsedCustom = false;
	ShaderProgram::SetProjectionAndViewAll(m_Projection, m_View);
	//ShaderProgram::SetProjectionAll(m_Projection);

	if (m_enableDebugLineRendering)
	{
		m_axisLinesMesh->DrawWith(m_axisLinesMaterial);
	}

	size_t numOfLayers = m_layerInformations.size();
	for (size_t layerIdx = 0; layerIdx < numOfLayers; layerIdx++)
	{
		if (m_layerInformations.at(layerIdx).m_enabled == true)
		{
			if (m_layerInformations.at(layerIdx).m_useCustomCameraPositionAndOrientation != lastLayerUsedCustom)
			{
				lastLayerUsedCustom = m_layerInformations.at(layerIdx).m_useCustomCameraPositionAndOrientation;
				if (lastLayerUsedCustom)
				{
					ShaderProgram::SetViewAll(m_layerInformations.at(layerIdx).m_layerView);
				}
				else
				{
					ShaderProgram::SetViewAll(m_View);
				}
			}
			RenderLayer(layerIdx);
		}
	}

	//g_Renderer->FramebufferBind(nullptr);
	//g_Renderer->RenderPostProcess(current_surface, m_material, m_mesh);
	/*
	//undo depth testing/z-test.
	Bind(current_surface);
	Clear();
	foreach(Layer)
	{
		RenderLayer(); //just get the base image without any of the layer effects.
		foreach(effect)
		{
			BindFBO(effect_surface);
			effect->SetTexture("gColor", current_surface->get_color_texture(0));
			RenderFullScreenQuad();
			Swap(effect, current);
		}
	}
	*/
}

void SpriteRenderer::RenderLayer(const size_t& layerID)
{
	for (size_t i = 0; i < m_SpritesToRenderByLayer.at(layerID).size(); i++)
	{
		Sprite* sprite = m_SpritesToRenderByLayer.at(layerID).at(i);
		if (sprite == nullptr)
		{
			continue;
		}

		sprite->Render();
	}
	for (size_t i = 0; i < m_particleSystemsByLayer.at(layerID).size(); i++)
	{
		ParticleSystem2D* system = m_particleSystemsByLayer.at(layerID).at(i);
		if (system == nullptr)
		{
			continue;
		}
		system->Render();
	}
}

//--------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------
//static methods
void SpriteRenderer::SetScreenResolution(const Vector2& screenResolution)
{
	SpriteResource::SetScreenResolution(screenResolution);
}
void SpriteRenderer::SetImportSize(const float& importSize)
{
	SpriteResource::SetImportSize(importSize);
}
void SpriteRenderer::SetScreenSize(const float& screenSize)
{
	SpriteResource::SetScreenSize(screenSize);
}
const Vector2 SpriteRenderer::GetScreenResolution()
{
	return SpriteResource::GetScreenResolution();
}
const float SpriteRenderer::GetImportSize()
{
	return SpriteResource::GetImportSize();
}
const float SpriteRenderer::GetScreenSize()
{
	return SpriteResource::GetScreenSize();
}
const float SpriteRenderer::GetInverseAspectRatio()
{
	return SpriteResource::GetInverseAspectRatio();
}
const float SpriteRenderer::GetAspectRatio()
{
	return SpriteResource::GetAspectRatio();
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//private methods

void SpriteRenderer::Calc2DViewMath()
{
	m_Projection.MatrixMakeIdentity();
	m_View.MatrixMakeIdentity();

	Matrix44::MakeEulerRotation(m_View, DegreesToRadians(0.f), DegreesToRadians(-0.f),
		DegreesToRadians(m_cameraYaw), Vector3(m_cameraPosition.x, m_cameraPosition.y, m_cameraPosition.z)); //
	Matrix44::MatrixInvertOrthogonal(m_View);
	//Matrix44::MatrixMakeOrthogonal(m_Projection, m_nearView, m_farView);
	Matrix44::MatrixMakeOrthogonal(m_Projection, m_virtualScreen, m_nearView.z, m_farView.z);
}
void SpriteRenderer::SetUp2DViewRender() const
{
	g_Renderer->SetOrtho(Vector2::vec2_zeros, GetScreenResolution().y, m_nearView.y, m_farView.y);
}
void SpriteRenderer::UpdateAxisLines()
{
	if (m_axisLines.size() != 8)
	{
		m_axisLines.clear();
		m_axisLines.resize(8);
	}
	if (m_axisLineIndices.size() != 12)
	{
		m_axisLineIndices.clear();
		m_axisLineIndices.resize(12);
	}

	const Vector2 pos = SpriteResource::ConvertVirtualCoordsToScreenCords(Vector2(1.f, 1.f), GetScreenResolution(), Vector2(1.f, 1.f));
	const Vector2 size = SpriteResource::ConvertVirtualCoordsToScreenCords(Vector2(m_debugLineThickness, m_debugLineThickness), GetScreenResolution(), Vector2(1.f, 1.f));

	//X axis
	m_axisLines[0] = SpriteVert(Vector2(-size.x, -size.y), Rgba::s_Red, Vector2::vec2_zeros);
	m_axisLines[1] = SpriteVert(Vector2(pos.x, -size.y), Rgba::s_Red, Vector2::vec2_zeros);
	m_axisLines[2] = SpriteVert(Vector2(pos.x, size.y), Rgba::s_Red, Vector2::vec2_zeros);
	m_axisLines[3] = SpriteVert(Vector2(-size.x, size.y), Rgba::s_Red, Vector2::vec2_zeros);

	//Y axis
	m_axisLines[4] = SpriteVert(Vector2(-size.x, -size.y), Rgba::s_Green, Vector2::vec2_zeros);
	m_axisLines[5] = SpriteVert(Vector2(-size.x, pos.y), Rgba::s_Green, Vector2::vec2_zeros);
	m_axisLines[6] = SpriteVert(Vector2(size.x, pos.y), Rgba::s_Green, Vector2::vec2_zeros);
	m_axisLines[7] = SpriteVert(Vector2(size.x, -size.y), Rgba::s_Green, Vector2::vec2_zeros);

	m_axisLineIndices[0] = 0;
	m_axisLineIndices[1] = 1;
	m_axisLineIndices[2] = 3;
	m_axisLineIndices[3] = 1;
	m_axisLineIndices[4] = 3;
	m_axisLineIndices[5] = 2;

	m_axisLineIndices[6] = 4;
	m_axisLineIndices[7] = 5;
	m_axisLineIndices[8] = 7;
	m_axisLineIndices[9] = 5;
	m_axisLineIndices[10] = 7;
	m_axisLineIndices[11] = 6;

	m_axisLinesMesh->setVBO(m_axisLines);
	m_axisLinesMesh->setIBO(m_axisLineIndices);
}
void SpriteRenderer::VerifySpriteNotCoveredByAnotherSpriteInAboveLayers(Sprite* sprite, const size_t& layedID)
{
	sprite;
	layedID;
}