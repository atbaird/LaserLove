#pragma once

#ifndef ENEMYTEMPLATE_HPP
#define ENEMYTEMPLATE_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include "Game/Bullets/BulletConfiguration.hpp"
#include <vector>
#include <string>

class Ship;

class EnemyTemplate
{
private:
	std::string m_name;
	int m_health;
	const SpriteResource* m_resource;
	Vector2 m_scale;
	Vector2 m_collisionScale;
	float m_speed;
	int m_points;
	std::vector<const BulletConfiguration*> m_bulletConfigurations;
public:
	//Constructors
	EnemyTemplate();
	EnemyTemplate(const std::string& name, const int& health, const std::vector<const BulletConfiguration*>& configs,
		const Vector2& scale = Vector2::vec2_ones, const Vector2& collisionScale = Vector2::vec2_ones, const SpriteResource* resource = nullptr);

	//Setters
	void SetName(const std::string& name);
	void SetHealth(const int& health);
	void AddBulletConfig(const BulletConfiguration* config);
	void SetBulletConfigs(const std::vector<const BulletConfiguration*> configs);
	void SetScale(const Vector2& scale);
	void SetCollisionScale(const Vector2& collisionScale);
	void SetSpriteResource(const SpriteResource* resource);
	void SetSpeed(const float& speed);
	void SetPointsWorth(const int& points);

	//Getters
	const std::string GetName() const;
	const int GetHealth() const;
	const std::vector<const BulletConfiguration*> GetBulletConfigurations() const;
	const Vector2 GetScale() const;
	const Vector2 GetCollisionScale() const;
	const SpriteResource* GetSpriteResource() const;
	const float GetSpeed() const;
	const int GetPointsWorth() const;

	//Spawners
	Ship* SpawnShipNoController(const Vector2& position, const float& rotation) const;
	Ship* SpawnShipWithPlayerController(const Vector2& position, const float& rotation) const;
	Ship* SpawnShipWithNPCController(const Vector2& position, const float& rotation) const;
};
#endif