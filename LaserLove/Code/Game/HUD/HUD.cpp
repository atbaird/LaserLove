#include "Game/HUD/HUD.hpp"
#include "Engine/2D/SpriteRenderer.hpp"
#include "Engine/2D/Sprite.hpp"
#include "Game/Ships/Ship.hpp"
#include "Game/SpriteLoader/CommonVariables.hpp"
#include "Game/SpriteLoader/SpriteResourceList.hpp"

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Constructors

const Vector2 HUD::s_healthBoxSize = Vector2(2.f, 0.5f);
const Vector2 HUD::s_foregroundHealthBoxDecrease = Vector2(0.1f, 0.01f);
const Vector2 HUD::s_healthBoxStart = Vector2(-2.3f, -1.1f);
const Vector2 HUD::s_localShiftOver = Vector2(0.975f, 0.155f);
HUD::HUD(ShaderProgram* prog)
	: m_prog(prog),
	m_loaded(false),
	m_resource(nullptr)
{
	LoadSprites();
}
HUD::~HUD()
{
	ClearSprites();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Update and Render
void HUD::Update(const float& deltaSeconds, const gameState& currentState)
{
	deltaSeconds;
	if (m_loaded == false)
	{
		return;
	}

	if (Ship::s_playerShip == nullptr)
	{
		m_widgets.at(0)->SetEnabled(false);
		m_widgets.at(1)->SetEnabled(false);
	}
	else
	{
		if (currentState != State_Game)
		{
			m_widgets.at(0)->SetEnabled(false);
			m_widgets.at(1)->SetEnabled(false);
		}
		else
		{
			m_widgets.at(0)->SetEnabled(true);
			m_widgets.at(1)->SetEnabled(true);
			Vector2 blPos = m_widgets.at(0)->GetPosition() - (m_widgets.at(0)->GetTrueScale() / 2.f);
			int maxHP = Ship::s_playerShip->GetMaxHP();
			int currentHP = Ship::s_playerShip->GetCurrentHP();
			float percentage = (float)currentHP / (float)maxHP;
			Vector2 newScale = Vector2((m_overallHealthBoxPercentage.x * percentage), m_overallHealthBoxPercentage.y);
			m_widgets.at(1)->SetScale(newScale);
			Vector2 trueScale = m_widgets.at(1)->GetTrueScale();
			Vector2 newCenter = (blPos - s_healthBoxStart - s_localShiftOver ) + Vector2((trueScale.x / 2.f), 0.f);
			m_widgets.at(1)->SetLocalPosition(newCenter);
			m_widgets.at(1)->Render();
			
		}
	}
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Setters
void HUD::SetWidgets(const std::vector<Sprite*>& widgets, const bool& setShaderProgram)
{
	if (setShaderProgram == true)
	{
		for (size_t i = 0; i < widgets.size(); i++)
		{
			Sprite* sprite = widgets.at(i);
			if (sprite != nullptr)
			{
				sprite->CopyMaterialFromResourceAndSetShaderProgram(m_prog);
			}
		}
	}
	m_widgets = widgets;
}
void HUD::AddWidget(Sprite* widget, const bool& setShaderProgram)
{
	if (setShaderProgram == true)
	{
		widget->CopyMaterialFromResourceAndSetShaderProgram(m_prog);
	}
	m_widgets.push_back(widget);
}
void HUD::SetShaderProgram(ShaderProgram* prog)
{
	m_prog = nullptr;
	m_prog = prog;
}
void HUD::LoadSprites()
{
	m_resource = SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_whiteSpace);
	//Create Sprites
	Sprite* healthBoxBackground = new Sprite(m_resource);
	Sprite* healthBox = new Sprite(m_resource);

	//LoadPositions and Size
	healthBoxBackground->SetPosition(s_healthBoxStart + (s_healthBoxSize / 2.f));
	healthBoxBackground->SetScale(s_healthBoxSize);
	healthBoxBackground->SetTint(Rgba(0.7f, 0.7f, 0.7f, 0.7f));
	healthBox->SetParent(healthBoxBackground);
	healthBox->SetLocalPosition(Vector2(0.f, 0.f));
	healthBox->SetScale(s_healthBoxSize - s_foregroundHealthBoxDecrease);
	healthBox->SetTint(Rgba(0.9f, 0.f, 0.f, 0.8f));
	m_overallHealthBoxPercentage = s_healthBoxSize - s_foregroundHealthBoxDecrease;
	m_blHealthBoxForegroundStart = s_healthBoxSize + s_foregroundHealthBoxDecrease;


	//Insert into widgets
	m_widgets.push_back(healthBoxBackground);
	m_widgets.push_back(healthBox);

	//load widget defaults
	for (size_t i = 0; i < m_widgets.size(); i++)
	{
		m_widgets.at(i)->ChangeLayers(CommonVariables::s_textLayer);
		m_widgets.at(i)->CopyMaterialFromResourceAndSetShaderProgram(m_prog);
	}
	m_loaded = true;
}
void HUD::ClearSprites()
{
	m_loaded = false;
	for (size_t i = 0; i < m_widgets.size(); i++)
	{
		delete m_widgets[i];
		m_widgets[i] = nullptr;
	}
	m_widgets.clear();
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//Getters
const std::vector<Sprite*> HUD::GetWidgets() const
{
	return m_widgets;
}
const ShaderProgram* HUD::GetShaderProgram() const
{
	return m_prog;
}
ShaderProgram* HUD::GetEditableShaderProgram() const
{
	return m_prog;
}
