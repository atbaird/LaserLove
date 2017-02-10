#pragma once

#ifndef PARTICLESYSTEM2D_HPP
#define PARTICLESYSTEM2D_HPP
#include "Engine/2D/ParticleEmitter2D.hpp"
#include "Engine/2D/ParticleSystemDefinition2D.hpp"

class ParticleSystem2D
{
private:

	const ParticleSystemDefinition2D* m_definition;

	std::string m_tag;
	size_t m_layer;
	Vector2 m_center;
	std::vector<ParticleEmitter2D*> emitters;
	bool DestroyMeNow;
	bool m_enabled;
	bool m_visible;
	size_t numberOfParticlesContained;
protected:
public:
	//Constructors
	ParticleSystem2D(const std::string& tag, size_t layer, const Vector2& center);
	virtual ~ParticleSystem2D();

	//Update
	void Update(const float& deltaSeconds);
	void Render() const;

	//Particle Emitters
	void SetParticleSystemDefinition(const ParticleSystemDefinition2D* definition);
	void ClearParticleSystem();
	void ResetFromParticleSystemDefinition();
	void AddParticleEmitter(ParticleEmitter2D* emitter);

	//Setters
	void ChangeLayers(const size_t layer);
	void ChangeCenterLocation(const Vector2& center);

	//Getters
	const std::string GetTag() const;
	const size_t GetLayer() const;
	const Vector2 GetCenter() const;
	const bool GetDestroyMe() const;
	const bool GetEnabled() const;
	const bool GetVisible() const;
	const size_t GetTotalNumberOfParticlesInSystem() const;

	//Static Methods
	static const bool ParticleSystemDestroy(ParticleSystem2D* system);
};
#endif