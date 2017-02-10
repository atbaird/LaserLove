#pragma once

#ifndef BACKGROUNDSCROLLER_HPP
#define BACKGROUNDSCROLLER_HPP
#include "Engine/2D/Sprite.hpp"

class BackgroundScroller;

struct BackgroundScrollerPointsToUpdate
{
	BackgroundScrollerPointsToUpdate(BackgroundScroller* ptr, size_t startTile, size_t endTile);
	BackgroundScroller* m_ptr;
	size_t m_startTile;
	size_t m_endTile;
	float time;
};

static void UpdateBackgroundScroller(void* data);

class BackgroundScroller
{
private:
	const SpriteResource* m_tileResource;
	float m_scrollingSpeed;
	Vector2 m_scrollingDirection;
	Vector2 m_directionForNewColumns;
	Vector2 m_centerPos;
	std::vector<Sprite*> m_tiles;
	Vector2 m_pointToWrapTo;
	Vector2 m_parametersBeforeWrap;
	Vector2 m_scale;
	Vector2 m_virtSizeToClip;
	size_t m_numOfCols;
	static size_t maxTilesPerJobData;
	std::vector<BackgroundScrollerPointsToUpdate*> jobData;
public:
	//Constructors
	BackgroundScroller(const SpriteResource* tileResource, const Vector2& centerPos = Vector2::vec2_zeros, const size_t& numOfCols = 1,
		const Vector2& directionToScroll = Vector2(0.f, -1.f), const Vector2& directionForNewCol = Vector2(1.f,0.f),
		const float& speedToScroll = 1.f, const Vector2& pointToWrapTo = Vector2(0.f, 5.f),
		const Vector2& parametersBeforeWrap = Vector2(0.f, -5.f),
		const size_t& numTilesToSpawnPerColumn = 3,
		const Vector2& scale = Vector2(7.f, 5.52f),
		const Vector2& virtSizeToClip = Vector2(0.0f, 0.0f));
	~BackgroundScroller();

	//Update
	void Update(const float& deltaSeconds);
	void RecalcJobData();
	void ClearJobData();
	void RegisterJobs(const float& deltaSeconds);
	void UpdateTiles(size_t start, size_t stop, float timeSeconds);

	//Setters
	void SetScrollingSpeed(const float& scrollingSpeed);
	void SetScrollingDirection(const Vector2& direction);
	void ChangeTileResource(const SpriteResource* tileResource);

	//Getters
	const float GetScrollingSpeed() const;
	const Vector2 GetScrollingDirection() const;
};
#endif