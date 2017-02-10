#pragma once

#ifndef PARTICLEEMITTER2D_HPP
#define PARTICLEEMITTER2D_HPP
#include "Engine/2D/ParticleEmitterDefintion2D.hpp"
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"

class ParticleEmitter2D
{
private:
	ParticleEmitterDefinition2D* m_definition;
	Mesh* m_mesh;
	Material* m_material;

	//ParticleEmitter2D Variables
	Vector2 localPosition;
	Vector2 worldPosition;
	Vector2 m_centerPosition;
	std::vector<Particle2D> particles;
	float total_seconds_alive;
	float timeSinceLastSpawned;
	bool destroyMe;
	bool allowSpawningMoreParticles;
protected:
public:
	//Constructors
	ParticleEmitter2D(ParticleEmitterDefinition2D* def = nullptr, const Vector2& centerPosition = Vector2::vec2_zeros);
	virtual ~ParticleEmitter2D();

	//Updates and Render
	virtual void Update(const float& deltaSeconds);
	virtual void UpdateParticles(float deltaSeconds);
	virtual void UpdateMesh();
	virtual void Render() const;

	//operations
	void Spawn(float deltaSeconds);
	void RestartEmitter(); //this erases particles, and resets internal clock
	void ResetParticleEmitter2D(); //This just resets the internal clock

	//setters
	void SetLocalPosition(const Vector2& pos);
	void SetCenterPosition(const Vector2& pos);
	void UpdateWorldPosition();
	void SetParticleEmitterDefinition2D(ParticleEmitterDefinition2D* def);
	void MarkForDestroy();
	void StopSpawningMoreParticles();
	void StartSpawningMoreParticles();
	void SetMaterial(Material* mat);
	void SetShaderProgram(ShaderProgram* prog);

	//getters
	ParticleEmitterDefinition2D* GetEditableParticleEmitterDefinition2D() const;
	const ParticleEmitterDefinition2D* GetParticleEmitterDefinition2D() const;
	const bool GetDestroyMe() const;
	const bool GetAllowSpawningMoreParticles();
	const float GetTotalSecondsAlive() const;
	const size_t GetTotalNumberOfParticles() const;
	const Material* GetMaterial() const;
	const ShaderProgram* GetShaderProgram() const;
	Material* GetEditableMaterial() const;
	ShaderProgram* GetEditableShaderProgram() const;

};

#endif