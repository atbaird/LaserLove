#pragma once

#ifndef SNOWMANAGER_HPP
#define SNOWMANAGE_HPP
#include "Engine/2D/Sprite.hpp"

class SnowManager
{
private:
	//variables
	std::vector<Sprite*> m_snowFlakes;
	const SpriteResource* m_snowFlakeResource;

	size_t m_maxNumSnowFlakes;
	float m_secondsBeforeSpawn;
	float m_secondsSinceLastSpawn;
	Vector2 m_mins;
	Vector2 m_maxs;
	float m_fallingSpeed;
public:
	//Constructors
	SnowManager(const SpriteResource* snowFlakeResource);
	SnowManager(const size_t& snowFlakeResourceID);
	~SnowManager();

	//Update
	void Update(const float& deltaSeconds);
	void ClearSnowFlakes();
	void SpawnSnowFlake();

	//Setters
	void SetMaxNumberOfSnowFlakes(const size_t& maxSnowFlakes);
	void SetSecondsBeforeSpawn(const float& seconds);
	void SetSnowSpawnHeight(const float& height);
	void SetSnowDespawnHeight(const float& height);
	void SetWidthSpawnRange(const float& min, const float& max);
	void SetFallingSpeed(const float& fallingSpeed);

	//Getters
	const size_t GetMaxNumSnowFlakes() const;
	const float GetSecondsBeforeSpawn() const;

};
#endif // !SNOWMANAGER_HPP