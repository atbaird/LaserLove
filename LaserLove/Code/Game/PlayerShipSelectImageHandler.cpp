#include "Game/PlayerShipSelectImageHandler.hpp"
#include "Game/EnemyGenerator/EnemyShipGenerator.hpp"
#include "Game/SpriteLoader/ResourceNames.hpp"
#include "Game/SpriteLoader/SpriteResourceList.hpp"
#include "Engine/2D/SpriteResource.hpp"

//Constructors
PlayerShipSelectImageHandler::PlayerShipSelectImageHandler(const SpriteResource* resourceForOutline)
{
	m_numOfPlayerShips = EnemyShipGenerator::NumberOfPlayerShipTypes();
	if (m_numOfPlayerShips > 10)
	{
		m_numOfPlayerShips = 10;
	}
	size_t numToResize2 = m_numOfPlayerShips * 2;
	m_sprites.resize(numToResize2);
	Vector2 totalSize = EnemyShipGenerator::GetTotalSizeByScaleOfPlayerShips();
	Vector2 startPosition = Vector2(-totalSize.x / 2.f, 0.f);
	Vector2 current1 = startPosition;
	Vector2 current2 = startPosition;
	for (size_t i = 0; i < numToResize2; i++)
	{
		if (i < m_numOfPlayerShips)
		{
			m_sprites[i] = new Sprite(EnemyShipGenerator::GetResourceByPlayerShipIdx((int)i));
			Vector2 scale = EnemyShipGenerator::GetScaleByPlayerShipIdx((int)i);
			m_sprites[i]->SetScale(scale);
			m_sprites[i]->SetPosition(current1);
			m_sprites[i]->SetEnabled(false);
			current1.x += scale.x;
		}
		else
		{
			m_sprites[i] = new Sprite(resourceForOutline);
			Vector2 scale = EnemyShipGenerator::GetScaleByPlayerShipIdx((int)i - m_numOfPlayerShips);
			m_sprites[i]->SetScale(scale);
			m_sprites[i]->SetPosition(current2);
			m_sprites[i]->SetEnabled(false);
			current2.x += scale.x;
		}
	}

	m_instructions.resize(5);
	const SpriteResource* num1 = SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_zero);
	const SpriteResource* num2 = SpriteResourceList::GetSpriteResourceByName(std::to_string(m_numOfPlayerShips - 1));
	const SpriteResource* press = SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_Press);
	const SpriteResource* through = SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_through);
	const SpriteResource* toSelect = SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_ToSelectAShip);
	m_instructions[0] = new Sprite(press);
	m_instructions[1] = new Sprite(num1);
	m_instructions[2] = new Sprite(through);
	m_instructions[3] = new Sprite(num2);
	m_instructions[4] = new Sprite(toSelect);
	Vector2 start = Vector2(-1.0f, -0.5f);
	m_instructions[0]->SetPosition(start + Vector2(0.f, 0.f));
	m_instructions[1]->SetPosition(start + Vector2(0.25f, 0.f));
	m_instructions[2]->SetPosition(start + Vector2(0.6f, 0.f));
	m_instructions[3]->SetPosition(start + Vector2(0.9f, 0.f));
	m_instructions[4]->SetPosition(start + Vector2(1.5f, 0.f));
	for (size_t i = 0; i < m_instructions.size(); i++)
	{
		m_instructions[i]->SetScale(Vector2(0.5f, 0.5f));
		m_instructions[i]->SetEnabled(false);
	}
}

PlayerShipSelectImageHandler::~PlayerShipSelectImageHandler()
{
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		if (m_sprites.at(i) != nullptr)
		{
			delete m_sprites[i];
			m_sprites[i] = nullptr;
		}
	}
	for (size_t i = 0; i < m_instructions.size(); i++)
	{
		if (m_instructions[i] != nullptr)
		{
			delete m_instructions[i];
			m_instructions[i] = nullptr;
		}
	}
	m_sprites.clear();
}


//Setters
void PlayerShipSelectImageHandler::SetVisible(bool visible)
{
	m_visible = visible;
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		if (m_sprites[i] != nullptr)
		{
			m_sprites[i]->SetEnabled(m_visible);
		}
	}
	for (size_t i = 0; i < m_instructions.size(); i++)
	{
		if (m_instructions[i] != nullptr)
		{
			m_instructions[i]->SetEnabled(m_visible);
		}
	}
}

//Getters
const bool PlayerShipSelectImageHandler::GetVisible()
{
	return m_visible;
}

//Updates
void PlayerShipSelectImageHandler::Update(bool selected, size_t selectedIdx)
{
	if (selected == false)
	{
		for (size_t i = m_numOfPlayerShips; i < m_sprites.size(); i++)
		{
			m_sprites[i]->SetTint(Rgba::s_White);
		}
	}
	else
	{
		for (size_t i = m_numOfPlayerShips; i < m_sprites.size(); i++)
		{
			if (m_sprites[i] == nullptr)
			{
				continue;
			}
			if ((m_numOfPlayerShips + selectedIdx) == i)
			{
				m_sprites[i]->SetTint(Rgba::s_Red);
			}
			else
			{
				m_sprites[i]->SetTint(Rgba::s_White);
			}
		}
	}
}