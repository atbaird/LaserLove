#pragma once

#ifndef BULLETCONFIGURATION_HPP
#define BULLETCONFIGURATION_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include <vector>

class SpriteAnimationSequence;
class Bullet;
class Ship;

struct BulletPosAngSpeed
{
	BulletPosAngSpeed(const Vector2& pos = Vector2::vec2_zeros, const float& ang = 0.f, const float& sp = 1.f, const float& angularVel = 0.f);

	Vector2 relPos;
	Vector2 collisionScale;
	size_t numTimesExplode;
	float angle;
	float speed;
	float angularVelocity;
	float imageaddedrotation;
	float additiveAngularVelocity;
	float secondsAfterBornToDie;
};

class BulletConfiguration
{
private:
	std::vector<BulletPosAngSpeed> m_bulletRels;
	const SpriteResource* m_resource;
	Vector2 m_collisionScale;
	SpriteAnimationSequence* m_sequence;
	bool m_usesResource;
	float m_time;

	static std::map<std::string, BulletConfiguration*>* s_configurations;
	static const std::string s_bulletConfigurationRootFolder;
	static const char* s_baseBulletConfigurationsNodeName;
	static const char* s_bulletConfigurationNodeName;
public:
	//Constructors
	BulletConfiguration(const SpriteResource* resource = nullptr);
	BulletConfiguration(const SpriteResource* resource, const std::vector<BulletPosAngSpeed>& positions);
	BulletConfiguration(SpriteAnimationSequence* sequence, const float& time = 0.f);
	BulletConfiguration(SpriteAnimationSequence* sequence, const std::vector<BulletPosAngSpeed>& positions, const float& time = 0.f);
	virtual ~BulletConfiguration();

	//Setters
	void SetTime(const float& time);
	void SetResource(const SpriteResource* resource);
	void SetSequence(SpriteAnimationSequence* sequence);
	void AddBulletPosition(const BulletPosAngSpeed& pos);
	void SetBulletPositions(const std::vector<BulletPosAngSpeed>& positions);
	void AppendBulletPosition(const std::vector<BulletPosAngSpeed>& positions);
	void SetCollisionScale(const Vector2& colScale);

	//Spawner
	const std::vector<Bullet*> SpawnBullets(const Ship* instigator, const Vector2& currentShipSpeed = Vector2::vec2_zeros) const;

	//Static Methods
	static const bool RegisterBulletConfiguration(const std::string& name, BulletConfiguration* configuration);
	static const BulletConfiguration* GetBulletConfiguration(const std::string& name);
	static void ClearAllBulletConfigurations();
	static void LoadAllBulletConfigurations();
};
#endif