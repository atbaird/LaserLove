#include "Game/Ships/Ship.hpp"
#include "Game/Ships/Controller/Controller.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/Bullets/BulletConfiguration.hpp"

void ShipCleaner()
{
	if (Ship::s_ships != nullptr)
	{
		for (size_t i = 0; i < Ship::s_ships->size(); i++)
		{
			Ship* ship = Ship::s_ships->at(i);
			if (ship != nullptr)
			{
				delete ship;
				ship = nullptr;
			}
		}
		Ship::s_ships->clear();
		delete Ship::s_ships;
		Ship::s_ships = nullptr;
	}
}

std::vector<Ship*>* Ship::s_ships = nullptr;
const GameListCleaner Ship::s_ShipCleaner = GameListCleaner(ENTITY_CLEANER, "ShipCleaner", ShipCleaner);
Ship* Ship::s_playerShip = nullptr;
const float Ship::s_playerFireRate = 0.5f;
const float Ship::s_NPCFireRate = 1.5f;
const float Ship::s_playerInvincibilitySeconds = 3.f;
const float Ship::s_NPCInvincibilitySeconds = 0.5f;
const float Ship::s_secondsBeforeEnableDisableShipVisibilityIfInvincibile = 0.1f;
int Ship::s_playerPoints = 0;
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//Constructors
Ship::Ship(const ShipType& shipType, Controller* controllerType)
	: GameEntity(TYPE_SHIP),
	m_shipType(shipType),
	m_controller(controllerType),
	m_canMove(true),
	m_currentBulletConfig(0),
	m_pointsWorth(0),
	m_secondsSinceLastHurt(0.f),
	m_secondsBeforeNoLongerInvincible(2.f),
	m_invincible(false),
	m_secondsSinceLastFlippedVisibility(s_secondsBeforeEnableDisableShipVisibilityIfInvincibile),
	m_justShotABullet(false),
	m_speed(1.f)
{
	SetCollisionRadius(Vector2(0.2f, 0.2f));
	if (Ship::s_ships == nullptr)
	{
		Ship::s_ships = new std::vector<Ship*>();
	}
	Ship::s_ships->push_back(this);
}
Ship::Ship(const SpriteResource* resource, const ShipType& shipType, Controller* controllerType)
	: GameEntity(TYPE_SHIP, resource),
	m_shipType(shipType),
	m_controller(controllerType),
	m_canMove(true),
	m_currentBulletConfig(0),
	m_pointsWorth(0),
	m_secondsSinceLastHurt(0.f),
	m_secondsBeforeNoLongerInvincible(2.f),
	m_invincible(false),
	m_secondsSinceLastFlippedVisibility(s_secondsBeforeEnableDisableShipVisibilityIfInvincibile),
	m_justShotABullet(false),
	m_speed(1.f)
{
	SetCollisionRadius(Vector2(0.2f, 0.2f));
	if (Ship::s_ships == nullptr)
	{
		Ship::s_ships = new std::vector<Ship*>();
	}
	Ship::s_ships->push_back(this);
}
Ship::Ship(const size_t& spriteResourceID, const ShipType& shipType, Controller* controllerType)
	: GameEntity(TYPE_SHIP, spriteResourceID),
	m_shipType(shipType),
	m_controller(controllerType),
	m_canMove(true),
	m_currentBulletConfig(0),
	m_pointsWorth(0),
	m_secondsSinceLastHurt(0.f),
	m_secondsBeforeNoLongerInvincible(2.f),
	m_invincible(false),
	m_secondsSinceLastFlippedVisibility(s_secondsBeforeEnableDisableShipVisibilityIfInvincibile),
	m_justShotABullet(false),
	m_speed(1.f)
{
	SetCollisionRadius(Vector2(0.02f, 0.02f));
	if (Ship::s_ships == nullptr)
	{
		Ship::s_ships = new std::vector<Ship*>();
	}
	Ship::s_ships->push_back(this);
}
Ship::~Ship()
{
	GameEntity::~GameEntity();
	if (m_controller != nullptr)
	{
		m_controller->ClearShip(false);
		m_controller = nullptr;
	}

	if (Ship::s_ships != nullptr)
	{
		for (size_t i = 0; i < Ship::s_ships->size(); i++)
		{
			if (Ship::s_ships->at(i) == this)
			{
				Ship::s_ships->erase(Ship::s_ships->begin() + i);
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//Update
void Ship::Update(const float& deltaSeconds)
{
	m_justShotABullet = false;
	if (m_controller != nullptr)
	{
		Vector2 movementVec = Vector2::vec2_zeros;
		if (m_secondsSinceLastHurt < m_secondsBeforeNoLongerInvincible)
		{
			m_secondsSinceLastHurt += deltaSeconds;
			if (m_secondsSinceLastFlippedVisibility >= s_secondsBeforeEnableDisableShipVisibilityIfInvincibile)
			{
				SetEnabled(!GetEnabled());
			}
			if (m_secondsSinceLastHurt >= m_secondsBeforeNoLongerInvincible)
			{
				m_invincible = false;
				SetEnabled(true);
				m_secondsSinceLastFlippedVisibility = (s_secondsBeforeEnableDisableShipVisibilityIfInvincibile);
			}
		}
		if (m_canMove == true)
		{
			m_controller->Update(deltaSeconds);
			float rotation = GetTrueForward();
			movementVec = m_controller->GetMovementAsNormalizedVector(rotation);
			Vector2 position = GetPosition() + (movementVec * deltaSeconds * m_speed);
			SetPosition(position);
		}
		if (m_controller->GetWantsShoot() == true)
		{
			if (m_secondsSinceLastFired >= m_secondsBeforeCanFireAgain)
			{
				m_secondsSinceLastFired = 0.f;
				if (m_bulletConfigurations.size() != 0 && (GetTeamID() == GameEntity::s_playerTeamID || GetOnScreen() == true))
				{
					const BulletConfiguration* config = m_bulletConfigurations.at(m_currentBulletConfig);
					if (config != nullptr)
					{
						config->SpawnBullets(this, movementVec);
						m_justShotABullet = true;
					}

					m_currentBulletConfig++;
					if (m_currentBulletConfig >= m_bulletConfigurations.size())
					{
						m_currentBulletConfig = 0;
					}
				}
			}
			else
			{
				m_secondsSinceLastFired += deltaSeconds;
			}
		}
		else
		{
			m_secondsSinceLastFired = m_secondsBeforeCanFireAgain;
		}
	}
	

	GameEntity::Update(deltaSeconds);
}
void Ship::FinalizationPass()
{
	if (GetIsAlive() == false && m_controller != nullptr)
	{
		delete m_controller;
	}

	GameEntity::FinalizationPass();
}


//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//Setters
void Ship::HurtShip()
{
	if (m_invincible == false)
	{
		m_currentHP -= 1;
		m_secondsSinceLastHurt = 0.f;
		m_invincible = true;
		if (m_currentHP <= 0)
		{
			SetIsAlive(false);
		}
	}
}
void Ship::SetController(Controller* controller, const bool& notifyParent)
{
	if (m_controller != nullptr)
	{
		m_controller->SetShip(nullptr, false);
	}

	m_controller = nullptr;
	m_controller = controller;

	if (m_controller != nullptr)
	{
		ControllerType type = m_controller->GetControllerType();
		if (type == CONTROLLER_PLAYER_TYPE)
		{
			m_secondsBeforeCanFireAgain = s_playerFireRate;
			m_secondsBeforeNoLongerInvincible = s_playerInvincibilitySeconds;
		}
		else if (type == CONTROLLER_NPC_TYPE)
		{
			m_secondsBeforeCanFireAgain = s_NPCFireRate;
			m_secondsBeforeNoLongerInvincible = s_NPCInvincibilitySeconds;
		}
		m_secondsSinceLastFired = m_secondsBeforeCanFireAgain;
		m_secondsSinceLastHurt = m_secondsBeforeNoLongerInvincible;
	}
	if (m_controller != nullptr && notifyParent)
	{
		m_controller->SetShip(nullptr, false);
	}
}
void Ship::ClearController(const bool& notifyParent)
{
	if (m_controller != nullptr && notifyParent == true)
	{
		m_controller->ClearShip(false);
	}
	m_controller = nullptr;
}
void Ship::SetBothHPs(const int& hp)
{
	m_maxHP = hp;
	m_currentHP = hp;
}
void Ship::SetMaxHP(const int& maxHP)
{
	m_maxHP = maxHP;
}
void Ship::SetCurrentHP(const int& curHP, const bool& setIsAliveIfValid)
{
	m_currentHP = curHP;
	if (m_currentHP > m_maxHP)
	{
		m_currentHP = m_maxHP;
	}
	else if (m_currentHP <= 0)
	{
		m_currentHP = 0;
		if (setIsAliveIfValid)
		{
			SetIsAlive(false);
		}
	}
}
void Ship::SetCanMove(const bool& move)
{
	m_canMove = move;
}
void Ship::SetBulletConfiguration(const std::vector<const BulletConfiguration*>& configurations)
{
	m_bulletConfigurations = configurations;
}
void Ship::AddBulletConfiguration(const BulletConfiguration* configuration)
{
	m_bulletConfigurations.push_back(configuration);
}
void Ship::SetShipType(const ShipType& type)
{
	m_shipType = type;
}
void Ship::SetSpeed(const float& speed)
{
	m_speed = speed;
}
void Ship::SetPointsWorth(const int& points)
{
	m_pointsWorth = points;
}

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//Getters
const ShipType Ship::GetShipType() const
{
	return m_shipType;
}
const ControllerType Ship::GetControllerType() const
{
	if (m_controller == nullptr)
	{
		return CONTROLLER_NONE_TYPE;
	}
	return m_controller->GetControllerType();
}
const Controller* Ship::GetController() const
{
	return m_controller;
}
Controller* Ship::GetEditableController() const
{
	return m_controller;
}
const int Ship::GetCurrentHP() const
{
	return m_currentHP;
}
const int Ship::GetMaxHP() const
{
	return m_maxHP;
}
const bool Ship::GetCanMove() const
{
	return m_canMove;
}
const float Ship::GetTrueForward() const
{
	return GetRotation() + 90.f;
}
const Vector2 Ship::GetForward() const
{
	const float forward = GetTrueForward();
	return Vector2(CosDegrees(forward), SinDegrees(forward));
}
const Vector2 Ship::GetRight() const
{
	const float right = GetRotation();
	return Vector2(CosDegrees(right), SinDegrees(right));
}
const bool Ship::GetIsInvincible() const
{
	return m_invincible;
}
const bool Ship::GetJustShotABullet() const
{
	return m_justShotABullet;
}
const int Ship::GetPointsWorth() const
{
	return m_pointsWorth;
}