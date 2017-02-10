#pragma once

#ifndef PARTICLE2D_HPP
#define PARTICLE2D_HPP
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/2D/SpriteResource.hpp"

class Particle2D
{
private:
public:
	static Particle2D s_empty;
	const SpriteResource* resource;
	Rgba tint;
	Rgba m_startColor;
	Rgba m_endColor;
	Rgba m_colorDiffBetweenEndAndStartColor;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 texCoords;
	float age;
	float max_age;
	bool destroyMe;
	bool ColorTransitionEnabled;
protected:
public:
	//Constructors
	Particle2D(const SpriteResource* res = nullptr, Rgba color = Rgba::s_White, Vector2 pos = Vector2::vec2_zeros,
		Vector2 vel = Vector2::vec2_zeros, Vector2 acc = Vector2::vec2_zeros, float max_age_seconds = 0.0f,
		bool enableColorOverTime = false, const Rgba& startColor = Rgba::s_White, const Rgba& endColor = Rgba::s_White);
	virtual ~Particle2D();

	//Updates and Render
	virtual void Update(const float& DeltaSeconds);
	virtual void Render() const;

	//Setters
	void SetSpriteResource(const SpriteResource* res);
	void SetTint(const Rgba& color);
	void SetPosition(const Vector2& pos);
	void SetVelocity(const Vector2& vel);
	void SetAcceleration(const Vector2& acc);
	void SetMaxAge(const float& max_age_seconds);
	void SetColorOverTimeEnabled(const bool& enabled);
	void SetStartColor(const Rgba& color);
	void SetEndColor(const Rgba& color);

	//Getters
	const SpriteResource* GetSpriteResource() const;
	const Rgba GetTint() const;
	const Vector2 GetPosition() const;
	const Vector2 GetVelocity() const;
	const Vector2 GetAcceleration() const;
	const float GetMaxAge() const;
	const float GetCurrentAge() const;
	const bool GetDestroyMe() const;
	const bool GetColorOverTimeEnabled() const;
	const Rgba GetStartColor() const;
	const Rgba GetEndColor() const;
};
#endif