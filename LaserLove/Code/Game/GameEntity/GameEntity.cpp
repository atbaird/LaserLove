#include "Game/GameEntity/GameEntity.hpp"


void ClearAllGameEntities()
{
	if (GameEntity::s_gameEntities == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < GameEntity::s_gameEntities->size(); i = i)
	{
		delete GameEntity::s_gameEntities->at(i);
	}
	delete GameEntity::s_gameEntities;
	GameEntity::s_gameEntities = nullptr;
}
const GameListCleaner GameEntity::s_GameEntityCleaner = GameListCleaner(ENTITY_CLEANER, "GameEntityCleaner", ClearAllGameEntities);

std::vector<GameEntity*>* GameEntity::s_gameEntities = nullptr;
const size_t GameEntity::s_playerTeamID = 0;
const size_t GameEntity::s_enemyTeamID = 1;
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Constructors
GameEntity::GameEntity(const GameEntityType& type, GameEntity* instigator)
	: AnimatedSprite(),
	m_gameEntityType(type),
	m_moving(false),
	m_alive(true),
	m_collision(nullptr),
	m_instigator(instigator),
	m_teamID(0),
	m_justCreated(true)
{
	m_collision = new EllipseCollision(1.f, this);
	if (GameEntity::s_gameEntities == nullptr)
	{
		GameEntity::s_gameEntities = new std::vector<GameEntity*>();
	}
	GameEntity::s_gameEntities->push_back(this);
}
GameEntity::GameEntity(const GameEntityType& type, const SpriteResource* resource, GameEntity* instigator)
	: AnimatedSprite(resource),
	m_gameEntityType(type),
	m_moving(false),
	m_alive(true),
	m_collision(nullptr),
	m_instigator(instigator),
	m_teamID(0),
	m_justCreated(true)
{
	m_collision = new EllipseCollision(1.f, this);
	if (GameEntity::s_gameEntities == nullptr)
	{
		GameEntity::s_gameEntities = new std::vector<GameEntity*>();
	}
	GameEntity::s_gameEntities->push_back(this);
}
GameEntity::GameEntity(const GameEntityType& type, const size_t& spriteResourceID, GameEntity* instigator)
	: AnimatedSprite(spriteResourceID),
	m_gameEntityType(type),
	m_moving(false),
	m_alive(true),
	m_collision(nullptr),
	m_instigator(instigator),
	m_teamID(0),
	m_justCreated(true)
{
	m_collision = new EllipseCollision(1.f, this);
	if (GameEntity::s_gameEntities == nullptr)
	{
		GameEntity::s_gameEntities = new std::vector<GameEntity*>();
	}
	GameEntity::s_gameEntities->push_back(this);
}
GameEntity::GameEntity(const GameEntityType& type, SpriteAnimationSequence* sequence, GameEntity* instigator, const float& time)
	: AnimatedSprite(sequence, time),
	m_gameEntityType(type),
	m_moving(false),
	m_alive(true),
	m_collision(nullptr),
	m_instigator(instigator),
	m_teamID(0),
	m_justCreated(true)
{
	m_collision = new EllipseCollision(1.f, this);
	if (GameEntity::s_gameEntities == nullptr)
	{
		GameEntity::s_gameEntities = new std::vector<GameEntity*>();
	}
	GameEntity::s_gameEntities->push_back(this);
}
GameEntity::~GameEntity()
{
	AnimatedSprite::~AnimatedSprite();
	if (m_collision != nullptr)
	{
		delete m_collision;
		m_collision = nullptr;
	}
	if (s_gameEntities == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < GameEntity::s_gameEntities->size(); i++)
	{
		if (GameEntity::s_gameEntities->at(i) == this)
		{
			GameEntity::s_gameEntities->erase(GameEntity::s_gameEntities->begin() + i);
			break;
		}
	}
}

//Update
void GameEntity::Update(const float& deltaSeconds)
{
	AcclerationUpdate(deltaSeconds);
	VelocityUpdate(deltaSeconds);
	AnimatedSprite::Update(deltaSeconds);
	m_justCreated = false;
}
void GameEntity::FinalizationPass()
{
	if (m_instigator != nullptr && m_instigator->GetIsAlive() == false)
	{
		m_instigator = nullptr;
	}
}
void GameEntity::Render() const
{
	AnimatedSprite::Render();
	if (m_collision != nullptr)
	{
		m_collision->Render();
	}
}

void GameEntity::AcclerationUpdate(const float& deltaSeconds)
{
	if (abs(m_acceleration.GetLength()) > 0.f)
	{
		Vector2 tempVel = m_velocity;
		tempVel += (m_acceleration * deltaSeconds);

		//Did X velocity cross the 0.f threshold?
		if ((m_velocity.x > 0.f && tempVel.x < 0.f)
			|| (m_velocity.x < 0.f && tempVel.x > 0.f))
		{
			m_xVelCrossedZeroBoundary = true;
		}
		else
		{
			m_xVelCrossedZeroBoundary = false;
		}

		//Did Y velocity cross the 0.f threshold?
		if ((m_velocity.y > 0.f && tempVel.y < 0.f)
			|| (m_velocity.y < 0.f && tempVel.y > 0.f))
		{
			m_yVelCrossedZeroBoundary = true;
		}
		else
		{
			m_yVelCrossedZeroBoundary = false;
		}
		m_velocity = tempVel;
	}
}
void GameEntity::VelocityUpdate(const float& deltaSeconds)
{
	if (abs(m_velocity.GetLength()) > 0.f)
	{
		m_moving = true;
		Vector2 position = GetPosition();
		m_previousPosition = position;
		position += (m_velocity * deltaSeconds);
		SetPosition(position);
	}
	else
	{
		m_moving = false;
	}
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Setters
void GameEntity::SetGamenEntityType(const GameEntityType& type)
{
	m_gameEntityType = type;
}
void GameEntity::SetVelocity(const Vector2& velocity)
{
	m_velocity = velocity;
}
void GameEntity::SetAcceleration(const Vector2& acceleration)
{
	m_acceleration = acceleration;
}
void GameEntity::ZeroOutVelocity()
{
	m_velocity = Vector2::vec2_zeros;
}
void GameEntity::ZeroOutYVelocity()
{
	m_velocity.y = 0.f;
}
void GameEntity::ZeroOutXVelcoity()
{
	m_velocity.x = 0.f;
}
void GameEntity::ZeroOutAcceleration()
{
	m_acceleration = Vector2::vec2_zeros;
}
void GameEntity::ZeroOutYAcceleration()
{
	m_acceleration.y = 0.f;
}
void GameEntity::ZeroOutXAcceleration()
{
	m_acceleration.x = 0.f;
}
void GameEntity::SetIsAlive(const bool& alive)
{
	m_alive = alive;
}
void GameEntity::SetCollisionRadius(const float& radius)
{
	m_collision->SetRadius(radius);
}
void GameEntity::SetCollisionRadius(const Vector2& xyradius)
{
	m_collision->SetXYRadius(xyradius);
}
void GameEntity::SetTeamID(const size_t& teamID)
{
	m_teamID = teamID;
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//Getters
const GameEntityType GameEntity::GetGameEntityType() const
{
	return m_gameEntityType;
}
const Vector2 GameEntity::GetPreviousPosition() const
{
	return m_previousPosition;
}
const Vector2 GameEntity::GetVelocity() const
{
	return m_velocity;
}
const Vector2 GameEntity::GetAcceleration() const
{
	return m_acceleration;
}
const bool GameEntity::GetIsMoving() const
{
	return m_moving;
}
const bool GameEntity::GetYVelCrossedZeroBoundary() const
{
	return m_yVelCrossedZeroBoundary;
}
const bool GameEntity::GetXVelCrossedZeroBoundary() const
{
	return m_xVelCrossedZeroBoundary;
}
const bool GameEntity::GetIsAlive() const
{
	return m_alive;
}
const EllipseCollision* GameEntity::GetCollision() const
{
	return m_collision;
}
const CollisionIntersection GameEntity::TestCollisionBetweenThisAsOwnerAndOther(GameEntity* other)
{
	return EllipseCollision::TestCircleCollisionsIntersecting(GetCollision(), other->GetCollision());
}
const size_t GameEntity::GetTeamID() const
{
	return m_teamID;
}
const bool GameEntity::GetJustCreated() const
{
	return m_justCreated;
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Static Methods
void GameEntity::FinalizationPassAll()
{
	if (s_gameEntities == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < s_gameEntities->size(); i++)
	{
		if (s_gameEntities->at(i) != nullptr)
		{
			s_gameEntities->at(i)->FinalizationPass();
		}
	}

	for (size_t i = 0; i < s_gameEntities->size(); i++)
	{
		if (s_gameEntities->at(i) != nullptr && s_gameEntities->at(i)->GetIsAlive() == false)
		{
			delete s_gameEntities->at(i);
			i--;
		}
	}
}