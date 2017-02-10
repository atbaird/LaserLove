#include "Engine/2D/ParticleEmitter2D.hpp"


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Constructors
ParticleEmitter2D::ParticleEmitter2D(ParticleEmitterDefinition2D* def, const Vector2& centerPosition)
	: m_definition(def),
	total_seconds_alive(0.f),
	timeSinceLastSpawned(0.f),
	destroyMe(false),
	allowSpawningMoreParticles(true),
	localPosition(Vector2::vec2_zeros),
	worldPosition(Vector2::vec2_zeros),
	m_centerPosition(centerPosition),
	m_material(nullptr),
	m_mesh(nullptr)
{
	if (def != nullptr)
	{
		localPosition = def->GetLocalPosition();
	}
	UpdateWorldPosition();
}

ParticleEmitter2D::~ParticleEmitter2D()
{
	if (m_definition != nullptr)
	{
		delete m_definition;
		m_definition = nullptr;
	}
	particles.clear();
}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Updates and Render
void ParticleEmitter2D::Update(const float& deltaSeconds)
{
	total_seconds_alive += deltaSeconds;
	UpdateParticles(deltaSeconds);
	Spawn(deltaSeconds);

	if (m_definition != nullptr && m_definition->GetSecondsbeforeAutoDestroyEmitter() > 0 && total_seconds_alive >= m_definition->GetSecondsbeforeAutoDestroyEmitter())
	{
		destroyMe = true;
	}
}

void ParticleEmitter2D::UpdateMesh()
{
	if (m_mesh == nullptr)
	{
		return;
	}
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < particles.size(); i++)
	{
		indices.push_back(i);
	}

	m_mesh->setIBO(indices);
}

void ParticleEmitter2D::UpdateParticles(float deltaSeconds)
{
	for (size_t i = 0; i < particles.size(); i++)
	{
		if (particles[i].GetDestroyMe() == true)
		{
			particles.erase(particles.begin() + i);
			i--;
			continue;
		}
		particles[i].Update(deltaSeconds);
	}
	if (particles.size() <= 0 && allowSpawningMoreParticles == false)
	{
		destroyMe = true;
	}
}

void ParticleEmitter2D::Render() const
{
	if (m_mesh == nullptr || m_material == nullptr)
	{
		return;
	}
	m_mesh->setVBOWithNoInternalAlterations(particles);


	return;

	//
	//Save this problem for the job list?
	//
	//size_t index = 0;
	//size_t endIndex = 0;
	//while (index < particles.size())
	//{
	//	endIndex = index + 4999;
	//	if (endIndex > particles.size())
	//	{
	//		endIndex = particles.size();
	//	}
	//
	//
	//	index += 5000;
	//	//Dynamic Buffers
	//	//go from 0 to 4999 particles, draw; then go from 5000 to 9999 and draw; rinse, repeat
	//	//FillMesh(&particles[i], Min(5000, particles.size() - index));
	//	//DrawMesh();
	//}
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//operations
void ParticleEmitter2D::Spawn(float deltaSeconds)
{
	timeSinceLastSpawned += deltaSeconds;
	if (m_definition == nullptr || (m_definition != nullptr && m_definition->GetSecondsBeforeStopSpawningParticles() > 0.f && total_seconds_alive >= m_definition->GetSecondsBeforeStopSpawningParticles()))
	{
		allowSpawningMoreParticles = false;
	}
	if (allowSpawningMoreParticles == false)
	{
		return;
	}

	const float particlesPerSecond = m_definition->GetParticleSpawnRatePerSecond();

	while (particlesPerSecond > 0.f && timeSinceLastSpawned >= particlesPerSecond)
	{
		particles.push_back(m_definition->SpawnParticle(worldPosition));
		timeSinceLastSpawned -= particlesPerSecond;
	}
}

void ParticleEmitter2D::RestartEmitter() //this erases particles, and resets internal clock
{
	particles.clear();
	ResetParticleEmitter2D();
}

void ParticleEmitter2D::ResetParticleEmitter2D() //This just resets the internal clock
{
	total_seconds_alive = 0.f;
	timeSinceLastSpawned = 0.f;
	destroyMe = false;
	allowSpawningMoreParticles = true;
	if (m_definition == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < m_definition->GetStartSpawnCount(); i++)
	{
		particles.push_back(m_definition->SpawnParticle(worldPosition));
	}
}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//setters
void ParticleEmitter2D::SetLocalPosition(const Vector2& pos)
{
	localPosition = pos;
	UpdateWorldPosition();
}

void ParticleEmitter2D::SetCenterPosition(const Vector2& pos)
{
	m_centerPosition = pos;
	UpdateWorldPosition();
}

void ParticleEmitter2D::UpdateWorldPosition()
{
	worldPosition = localPosition + m_centerPosition;
}

void ParticleEmitter2D::SetParticleEmitterDefinition2D(ParticleEmitterDefinition2D* def)
{
	m_definition = nullptr;
	m_definition = def;
}

void ParticleEmitter2D::MarkForDestroy()
{
	destroyMe = true;
}

void ParticleEmitter2D::StopSpawningMoreParticles()
{
	allowSpawningMoreParticles = false;
}

void ParticleEmitter2D::StartSpawningMoreParticles()
{
	allowSpawningMoreParticles = true;
}

void ParticleEmitter2D::SetMaterial(Material* mat)
{
	mat;
	if (m_material != nullptr)
	{
		delete m_material;
	}
	mat->MakeCopy(m_material);
}

void ParticleEmitter2D::SetShaderProgram(ShaderProgram* prog)
{
	if (m_material == nullptr)
	{
		m_material = new Material();
	}
	m_material->SetProgram(prog);
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//getters
ParticleEmitterDefinition2D* ParticleEmitter2D::GetEditableParticleEmitterDefinition2D() const
{
	return m_definition;
}

const ParticleEmitterDefinition2D* ParticleEmitter2D::GetParticleEmitterDefinition2D() const
{
	return m_definition;
}

const bool ParticleEmitter2D::GetDestroyMe() const
{
	return destroyMe;
}

const bool ParticleEmitter2D::GetAllowSpawningMoreParticles()
{
	return allowSpawningMoreParticles;
}

const float ParticleEmitter2D::GetTotalSecondsAlive() const
{
	return total_seconds_alive;
}

const size_t ParticleEmitter2D::GetTotalNumberOfParticles() const
{
	return particles.size();
}

const Material* ParticleEmitter2D::GetMaterial() const
{
	return m_material;
}

const ShaderProgram* ParticleEmitter2D::GetShaderProgram() const
{
	if (m_material == nullptr)
	{
		return nullptr;
	}
	return m_material->GetProgram();
}

Material* ParticleEmitter2D::GetEditableMaterial() const
{
	return m_material;
}

ShaderProgram* ParticleEmitter2D::GetEditableShaderProgram() const
{
	if (m_material == nullptr)
	{
		return nullptr;
	}
	return m_material->GetProgram();
}