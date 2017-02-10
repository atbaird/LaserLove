#include "Game/Snow/SnowManager.hpp"
#include "Engine/2D/SpriteDatabase.hpp"


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
SnowManager::SnowManager(const SpriteResource* snowFlakeResource)
	: m_snowFlakeResource(snowFlakeResource),
	m_maxNumSnowFlakes(100),
	m_secondsBeforeSpawn(0.01f),
	m_maxs(1.f, 1.f),
	m_mins(-1.f, -1.f),
	m_secondsSinceLastSpawn(0.f),
	m_fallingSpeed(0.5f)
{

}
SnowManager::SnowManager(const size_t& snowFlakeResourceID)
	: m_snowFlakeResource(nullptr),
	m_maxNumSnowFlakes(100),
	m_secondsBeforeSpawn(0.01f),
	m_maxs(1.f, 1.f),
	m_mins(-1.f, -1.f),
	m_secondsSinceLastSpawn(0.f),
	m_fallingSpeed(0.5f)
{
	if (SpriteDatabase::g_SpriteDatabase != nullptr)
	{
		m_snowFlakeResource = SpriteDatabase::g_SpriteDatabase->GetSprite(snowFlakeResourceID);
	}
}
SnowManager::~SnowManager()
{
	ClearSnowFlakes();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Update
void SnowManager::Update(const float& deltaSeconds)
{
	m_secondsSinceLastSpawn += deltaSeconds;
	if (m_snowFlakes.size() < m_maxNumSnowFlakes && m_secondsSinceLastSpawn >= m_secondsBeforeSpawn)
	{
		m_secondsSinceLastSpawn = 0.f;
		SpawnSnowFlake();
	}

	for (size_t i = 0; i < m_snowFlakes.size(); i++)
	{
		if (m_snowFlakes.at(i) != nullptr)
		{
			Sprite* snowFlake = m_snowFlakes.at(i);
			snowFlake->Update(0.0f);
			Vector2 pos = snowFlake->GetPosition() - Vector2(0.f, deltaSeconds * m_fallingSpeed);
			snowFlake->SetPosition(pos);
			if (pos.y < m_mins.y)
			{
				delete snowFlake;
				snowFlake = nullptr;
				m_snowFlakes.erase(m_snowFlakes.begin() + i);
				i--;
			}
		}
	}
}

void SnowManager::ClearSnowFlakes()
{
	for (size_t i = 0; i < m_snowFlakes.size(); i++)
	{
		if (m_snowFlakes.at(i) != nullptr)
		{
			delete m_snowFlakes[i];
			m_snowFlakes[i] = nullptr;
		}
	}
	m_snowFlakes.clear();
}
void SnowManager::SpawnSnowFlake()
{
	Vector2 spawnPosition;
	spawnPosition.y = m_maxs.y;
	float ran = 1.f / (1.f + (rand() % 100));
	float addative = (m_maxs.x - m_mins.x) * ran;
	spawnPosition.x = m_mins.x + addative;
	Sprite* newFlake = new Sprite(m_snowFlakeResource->GetSpriteResourceID());
	newFlake->SetPosition(spawnPosition);
	m_snowFlakes.push_back(newFlake);
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Setters
void SnowManager::SetMaxNumberOfSnowFlakes(const size_t& maxSnowFlakes)
{
	m_maxNumSnowFlakes = maxSnowFlakes;
}
void SnowManager::SetSecondsBeforeSpawn(const float& seconds)
{
	m_secondsBeforeSpawn = seconds;
}
void SnowManager::SetSnowSpawnHeight(const float& height)
{
	m_maxs.y = height;
}
void SnowManager::SetSnowDespawnHeight(const float& height)
{
	m_mins.y = height;
}
void SnowManager::SetWidthSpawnRange(const float& min, const float& max)
{
	m_mins.x = min;
	m_maxs.x = max;
}

void SnowManager::SetFallingSpeed(const float& fallingSpeed)
{
	if (fallingSpeed > 0.f)
	{
		m_fallingSpeed = fallingSpeed;
	}
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Getters
const size_t SnowManager::GetMaxNumSnowFlakes() const
{
	return m_maxNumSnowFlakes;
}
const float SnowManager::GetSecondsBeforeSpawn() const
{
	return m_secondsBeforeSpawn;
}