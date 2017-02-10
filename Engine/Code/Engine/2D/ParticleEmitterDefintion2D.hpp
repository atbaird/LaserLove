#pragma once

#ifndef PARTICLEEMITTERDEFINITION2D_HPP
#define PARTICLEEMITTERDEFINITION2D_HPP
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/2D/Particle2D.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include <map>



class ParticleEmitterDefinition2D
{
private:
	std::string m_name;
	const SpriteResource* resource;
	size_t start_spawn_count;
	float particle_spawn_rate_per_second;
	float life_time;
	float secondsBeforeStopSpawningParticles;
	float secondsBeforeAutoDestroyThis;
	Rgba tint;
	AABB2 start_velocity_MinsMaxs;
	AABB2 start_acceleration_MinsMaxs;
	Vector2 scale;
	Vector2 relativePosition;
	float radius;
	bool enableColorTransition;
	Rgba m_startColor;
	Rgba m_endColor;
protected:
public:
	//Constructors
	ParticleEmitterDefinition2D(XMLNode node);
	ParticleEmitterDefinition2D(std::string name = "", const Vector2& relPos = Vector2::vec2_zeros, size_t start_spawn_cnt = 0, float spawn_rate_per_second = 1.f,
		float life_time_seconds = 1.f, const Rgba& color = Rgba::s_White, AABB2 start_velocity = AABB2::ZERO_TO_ONE, AABB2 start_acceleration = AABB2::ZERO_TO_ONE,
		const Vector2& scal = Vector2::vec2_ones, float radiu = 0.f, bool enableColorTrans = false, const Rgba& startColor = Rgba::s_White, const Rgba& endColor = Rgba::s_White);
	ParticleEmitterDefinition2D(const SpriteResource* res, std::string name = "", const Vector2& relPos = Vector2::vec2_zeros, size_t start_spawn_cnt = 0, float spawn_rate_per_second = 1.f,
		float life_time_seconds = 1.f, const Rgba& color = Rgba::s_White, AABB2 start_velocity = AABB2::ZERO_TO_ONE, AABB2 start_acceleration = AABB2::ZERO_TO_ONE,
		const Vector2& scal = Vector2::vec2_ones, float radiu = 0.f, bool enableColorTrans = false, const Rgba& startColor = Rgba::s_White, const Rgba& endColor = Rgba::s_White);
	virtual ~ParticleEmitterDefinition2D();

	//Operations
	Particle2D SpawnParticle(const Vector2& emitterCenter);
	ParticleEmitterDefinition2D* Clone();

	//Setters
	void SetSpriteResource(const SpriteResource* res);
	void SetStartSpawnCount(size_t start_spawn_cnt);
	void SetParticleSpawnRate(float spawn_rate_per_second);
	void SetLifeTime(float life_time_seconds);
	void SetTint(const Rgba& color);
	void SetStartVelocity(const AABB2& start_velocity);
	void SetStartAcceleration(const AABB2& start_acceleration);
	void SetScale(const Vector2& scal);
	void SetSpawnRadius(float radiu);
	void SetSecondsBeforeStopSpawningParticles(float seconds);
	void SetSecondsBeforeAutoDestroyEmitter(float seconds);
	void SetLocalPosition(const Vector2& pos);

	//Getters
	const SpriteResource* GetSpriteResource() const;
	const size_t GetStartSpawnCount() const;
	const float GetParticleSpawnRatePerSecond() const;
	const float GetLifeTime() const;
	const Rgba GetTint() const;
	const AABB2 GetStartVelocity() const;
	const AABB2 GetStartAcceleration() const;
	const Vector2 GetScale() const;
	const float GetSpawnRadius() const;
	const std::string GetName() const;
	const float GetSecondsBeforeStopSpawningParticles() const;
	const float GetSecondsbeforeAutoDestroyEmitter() const;
	const Vector2 GetLocalPosition() const;
};
#endif