#include "Game/ScoreImageUpdater.hpp"
#include "Game/SpriteLoader/ResourceNames.hpp"
#include "Game/SpriteLoader/SpriteResourceList.hpp"

ScoreImageUpdater::ScoreImageUpdater(size_t digits)
	: m_visible(false),
	m_digits(digits)
{
	//Load Resources
	m_digitResources.resize(10);
	for (size_t i = 0; i < m_digitResources.size(); i++)
	{
		m_digitResources[i] = SpriteResourceList::GetSpriteResourceByName(std::to_string(i));
	}

	//load sprites
	m_sprites.resize(digits);
	Vector2 start = Vector2(1.2f, -0.9f);
	Vector2 current = start;
	Vector2 shiftOver = Vector2(0.06f, 0.f);
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i] = new Sprite(m_digitResources[0]);
		m_sprites[i]->SetScale(Vector2(0.3f, 0.3f));
		m_sprites[i]->SetPosition(current);
		m_sprites[i]->SetEnabled(false);
		current += shiftOver;
	}
}
ScoreImageUpdater::~ScoreImageUpdater()
{
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		if (m_sprites.at(i) != nullptr)
		{
			delete m_sprites[i];
			m_sprites[i] = nullptr;
		}
	}
	m_sprites.clear();
}

void ScoreImageUpdater::Update(size_t playerScore)
{
	int currentIdx = m_sprites.size() - 1;
	while (playerScore > 0 && currentIdx >= 0)
	{
		size_t digit = playerScore % 10;
		playerScore = playerScore / 10;
		m_sprites.at(currentIdx)->ChangeSpriteResource(m_digitResources[digit]);
		currentIdx--;
	}
	while (currentIdx >= 0)
	{
		m_sprites.at(currentIdx)->ChangeSpriteResource(m_digitResources[0]);
		currentIdx--;
	}
}
void ScoreImageUpdater::ToggleVisible(const bool& visible)
{
	m_visible = visible;
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i]->SetEnabled(m_visible);
	}
}