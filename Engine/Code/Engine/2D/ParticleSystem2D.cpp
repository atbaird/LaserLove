#include "Engine/2D/ParticleSystem2D.hpp"
#include "Engine/2D/SpriteRenderer.hpp"


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Constructors
ParticleSystem2D::ParticleSystem2D(const std::string& tag, size_t layer, const Vector2& center)
	: m_tag(tag),
	m_layer(layer),
	m_center(center),
	DestroyMeNow(false),
	m_enabled(true),
	m_visible(true),
	m_definition(nullptr),
	numberOfParticlesContained(0)
{
	if (SpriteRenderer::g_SpriteRenderer != nullptr)
	{
		SpriteRenderer::g_SpriteRenderer->RegisterParticleSystem(this);
	}
}

ParticleSystem2D::~ParticleSystem2D()
{
	if (SpriteRenderer::g_SpriteRenderer != nullptr)
	{
		SpriteRenderer::g_SpriteRenderer->UnregisterParticleSystem(this);
	}
	ClearParticleSystem();
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Update
void ParticleSystem2D::Update(const float& deltaSeconds)
{
	if (ParticleSystemDefinition2D::s_ClearedParticleSystemDefinitions)
	{
		m_definition = nullptr;
	}
	if (m_enabled == false)
	{
		return;
	}
	numberOfParticlesContained = 0;
	bool stillAtLeastOneActiveEmitter = false;
	for (size_t i = 0; i < emitters.size(); i++)
	{
		if (emitters[i] != nullptr)
		{
			if (emitters[i]->GetDestroyMe() == true)
			{
				delete emitters[i];
				emitters.erase(emitters.begin() + i);
				i--;
				continue;
			}
			else
			{
				stillAtLeastOneActiveEmitter = true;
				emitters[i]->Update(deltaSeconds);
				numberOfParticlesContained += emitters[i]->GetTotalNumberOfParticles();
			}
		}
	}
	if (stillAtLeastOneActiveEmitter == false)
	{
		DestroyMeNow = true;
	}
	else
	{
		DestroyMeNow = false;
	}
}

void ParticleSystem2D::Render() const
{
	for (size_t i = 0; i < emitters.size(); i++)
	{
		ParticleEmitter2D* emitter = emitters[i];
		if (emitter != nullptr)
		{
			emitter->Render();
		}
	}
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Particle Emitters

void ParticleSystem2D::SetParticleSystemDefinition(const ParticleSystemDefinition2D* definition)
{
	m_definition = definition;
}

void ParticleSystem2D::ClearParticleSystem()
{
	for (size_t i = 0; i < emitters.size(); i++)
	{
		if (emitters.at(i) != nullptr)
		{
			delete emitters[i];
			emitters[i] = nullptr;
		}
	}
	emitters.clear();
}

void ParticleSystem2D::ResetFromParticleSystemDefinition()
{
	ClearParticleSystem();
	const std::vector<ParticleEmitterDefinition2D*>* particleEmitterDefinitions = m_definition->GetParticleEmitterDefinitionNames();
	for (size_t i = 0; i < particleEmitterDefinitions->size(); i++)
	{
		ParticleEmitterDefinition2D* def = particleEmitterDefinitions->at(i);
		if (def != nullptr)
		{
			ParticleEmitter2D* emitter = new ParticleEmitter2D(def->Clone());
			emitter->ResetParticleEmitter2D();
			emitters.push_back(emitter);
		}
	}
}

void ParticleSystem2D::AddParticleEmitter(ParticleEmitter2D* emitter)
{
	emitters.push_back(emitter);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Setters
void ParticleSystem2D::ChangeLayers(const size_t layer)
{
	if (layer == m_layer)
	{
		return;
	}

	if (SpriteRenderer::g_SpriteRenderer != nullptr && m_enabled == true)
	{
		SpriteRenderer::g_SpriteRenderer->UnregisterParticleSystem(this);
		m_layer = layer;
		SpriteRenderer::g_SpriteRenderer->RegisterParticleSystem(this);

	}
	else
	{
		m_layer = layer;
	}
}

void ParticleSystem2D::ChangeCenterLocation(const Vector2& center)
{
	m_center = center;
	for (size_t i = 0; i < emitters.size(); i++)
	{
		ParticleEmitter2D* emitter = emitters[i];
		if (emitter != nullptr)
		{
			emitter->SetCenterPosition(center);
		}
	}
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Getters
const std::string ParticleSystem2D::GetTag() const
{
	return m_tag;
}

const size_t ParticleSystem2D::GetLayer() const
{
	return m_layer;
}

const Vector2 ParticleSystem2D::GetCenter() const
{
	return m_center;
}

const bool ParticleSystem2D::GetDestroyMe() const
{
	return DestroyMeNow;
}

const bool ParticleSystem2D::GetEnabled() const
{
	return m_enabled;
}

const bool ParticleSystem2D::GetVisible() const 
{
	return m_visible;
}

const size_t ParticleSystem2D::GetTotalNumberOfParticlesInSystem() const
{
	return numberOfParticlesContained;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Static Methods
const bool ParticleSystem2D::ParticleSystemDestroy(ParticleSystem2D* system)
{
	if (system == nullptr)
	{
		return false;
	}
	delete system;
	return true;
}