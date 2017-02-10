#include "Game/BackgroundScroller/BackgroundScroller.hpp"
#include "Game/SpriteLoader/CommonVariables.hpp"
#include "Engine/Core/JobSystem/JobSystem.hpp"

void UpdateBackgroundScroller(void* data)
{
	if (data == nullptr)
	{
		return;
	}
	BackgroundScrollerPointsToUpdate* ptr = (BackgroundScrollerPointsToUpdate*)data;
	ptr->m_ptr->UpdateTiles(ptr->m_startTile, ptr->m_endTile, ptr->time);
	
}

BackgroundScrollerPointsToUpdate::BackgroundScrollerPointsToUpdate(BackgroundScroller* ptr,
	size_t startTile, size_t endTile)
	: m_ptr(ptr),
	m_startTile(startTile),
	m_endTile(endTile)
{
}

size_t BackgroundScroller::maxTilesPerJobData = 2;

//Constructors
BackgroundScroller::BackgroundScroller(const SpriteResource* tileResource, const Vector2& centerPos, const size_t& numOfCols,
	const Vector2& directionToScroll, const Vector2& directionForNewCol,
	const float& speedToScroll, const Vector2& pointToWrapTo, const Vector2& parametersBeforeWrap,
	const size_t& numTilesToSpawnPerColumn, const Vector2& scale, const Vector2& virtSizeToClip)
	: m_tileResource(tileResource),
	m_scrollingSpeed(speedToScroll),
	m_scrollingDirection(directionToScroll),
	m_pointToWrapTo(pointToWrapTo),
	m_parametersBeforeWrap(parametersBeforeWrap),
	m_directionForNewColumns(directionForNewCol),
	m_centerPos(centerPos),
	m_numOfCols(numOfCols),
	m_scale(scale),
	m_virtSizeToClip(virtSizeToClip)
{
	m_scrollingDirection.normalize();

	Vector2 virtualScale = tileResource->GetVirtualSpaceSize();
	Vector2 realSize = SpriteResource::ConvertVirtualCoordsToScreenCords(virtualScale, tileResource->GetScreenResolution(), m_scale);

	Vector2 diff = m_pointToWrapTo - m_parametersBeforeWrap;
	Vector2 distance;
	distance.x = diff.x / (float)numOfCols;
	distance.y = diff.y / (float)numTilesToSpawnPerColumn;

	Vector2 currentPosition = m_parametersBeforeWrap;

	for (size_t j = 0; j < numOfCols; j++)
	{
		for (size_t i = 0; i < numTilesToSpawnPerColumn; i++)
		{

			Sprite* newSprite = new Sprite(tileResource);
			newSprite->SetPosition(currentPosition);
			newSprite->SetScale(m_scale);
			newSprite->ChangeLayers(CommonVariables::s_backgroundLayer);
			m_tiles.push_back(newSprite);
			currentPosition += directionToScroll * distance.y;
		}
		currentPosition += directionForNewCol * distance.x;
		currentPosition.y = m_parametersBeforeWrap.y;
	}

	size_t numTiles = m_tiles.size();
	for (size_t i = 0; i < numTiles; i = i + maxTilesPerJobData)
	{
		size_t max = i + maxTilesPerJobData;
		if (max > numTiles)
		{
			max = numTiles;
		}
		BackgroundScrollerPointsToUpdate* data = new BackgroundScrollerPointsToUpdate(this, i, max);
		jobData.push_back(data);
	}
}
BackgroundScroller::~BackgroundScroller()
{
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		if (m_tiles.at(i) != nullptr)
		{
			delete m_tiles[i];
		}
	}
	m_tiles.clear();
	ClearJobData();
}

//Update
void BackgroundScroller::Update(const float& deltaSeconds)
{
	const Vector2 amountToMove = m_scrollingDirection * (deltaSeconds * m_scrollingSpeed);
	const float negX = m_scrollingDirection.x / abs(m_scrollingDirection.x);
	const float negY = m_scrollingDirection.y / abs(m_scrollingDirection.y);
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		Sprite* tile = m_tiles.at(i);
		if (tile != nullptr)
		{
			Vector2 pos = tile->GetPosition();
			pos += amountToMove;

			if (m_scrollingDirection.x != 0.f 
				&& ((negX < 0.f && pos.x <= m_parametersBeforeWrap.x)
				|| (negX > 0.f && pos.x >= m_parametersBeforeWrap.x)))
			{
				float diff = m_parametersBeforeWrap.x - pos.x;
				if (pos.x <= m_parametersBeforeWrap.x)
				{
					pos.x = m_pointToWrapTo.x - diff;
				}
				else
				{
					pos.x = m_pointToWrapTo.x + diff;
				}
			}
			if (m_scrollingDirection.y != 0.f
				&& ((negY < 0.f && pos.y <= m_parametersBeforeWrap.y)
				|| (negY > 0.f && pos.y >= m_parametersBeforeWrap.y)))
			{
				float diff = m_parametersBeforeWrap.y - pos.y;
				if (pos.y <= m_parametersBeforeWrap.y)
				{
					pos.y = m_pointToWrapTo.y - diff;
				}
				else
				{
					pos.y = m_pointToWrapTo.y + diff;
				}
			}

			tile->SetPosition(pos);
		}
	}
}

void BackgroundScroller::RecalcJobData()
{
	ClearJobData();
	size_t numTiles = m_tiles.size();
	for (size_t i = 0; i < numTiles; i = i + maxTilesPerJobData)
	{
		size_t max = i + maxTilesPerJobData;
		if (max > numTiles)
		{
			max = numTiles;
		}
		BackgroundScrollerPointsToUpdate* data = new BackgroundScrollerPointsToUpdate(this, i, max);
		jobData.push_back(data);
	}
}

void BackgroundScroller::ClearJobData()
{
	for (size_t i = 0; i < jobData.size(); i++)
	{
		if (jobData.at(i) != nullptr)
		{
			delete jobData[i];
			jobData[i] = nullptr;
		}
	}
	jobData.clear();
}

void BackgroundScroller::RegisterJobs(const float& deltaSeconds)
{
	for (size_t i = 0; i < jobData.size(); i++)
	{
		if (jobData.at(i) != nullptr)
		{
			jobData.at(i)->time = deltaSeconds;
			JobSystem::CreateAndAddJob(0, UpdateBackgroundScroller, jobData.at(i));
		}
	}
}

void BackgroundScroller::UpdateTiles(size_t start, size_t stop, float timeSeconds)
{
	const Vector2 amountToMove = m_scrollingDirection * (timeSeconds * m_scrollingSpeed);
	const float negX = m_scrollingDirection.x / abs(m_scrollingDirection.x);
	const float negY = m_scrollingDirection.y / abs(m_scrollingDirection.y);
	for (size_t i = start; i < stop; i++)
	{
		Sprite* tile = m_tiles.at(i);
		if (tile != nullptr)
		{
			Vector2 pos = tile->GetPosition();
			pos += amountToMove;

			if (m_scrollingDirection.x != 0.f
				&& ((negX < 0.f && pos.x <= m_parametersBeforeWrap.x)
					|| (negX > 0.f && pos.x >= m_parametersBeforeWrap.x)))
			{
				pos.x = m_pointToWrapTo.x;
			}
			if (m_scrollingDirection.y != 0.f
				&& ((negY < 0.f && pos.y <= m_parametersBeforeWrap.y)
					|| (negY > 0.f && pos.y >= m_parametersBeforeWrap.y)))
			{
				pos.y = m_pointToWrapTo.y;
			}

			tile->SetPosition(pos);
		}
	}
}

//Setters
void BackgroundScroller::SetScrollingSpeed(const float& scrollingSpeed)
{
	m_scrollingSpeed = scrollingSpeed;
}
void BackgroundScroller::SetScrollingDirection(const Vector2& direction)
{
	m_scrollingDirection = direction;
	m_scrollingDirection.normalize();
}
void BackgroundScroller::ChangeTileResource(const SpriteResource* tileResource)
{
	m_tileResource = nullptr;
	m_tileResource = tileResource;
	for (size_t i = 0; i < m_tiles.size(); i++)
	{
		if(m_tiles.at(i) != nullptr)
		{
			m_tiles.at(i)->ChangeSpriteResource(tileResource);
		}
	}
}

//Getters
const float BackgroundScroller::GetScrollingSpeed() const
{
	return m_scrollingSpeed;
}
const Vector2 BackgroundScroller::GetScrollingDirection() const
{
	return m_scrollingDirection;
}