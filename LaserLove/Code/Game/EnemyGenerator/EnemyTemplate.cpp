#include "Game/EnemyGenerator/EnemyTemplate.hpp"
#include "Game/Ships/Controller/Controller.hpp"
#include "Game/Ships/Ship.hpp"
#include "Game/SpriteLoader/CommonVariables.hpp"

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//Constructors
EnemyTemplate::EnemyTemplate()
	: m_name(""),
	m_health(1),
	m_scale(Vector2::vec2_ones),
	m_resource(nullptr),
	m_speed(0.5f),
	m_points(0)
{

}
EnemyTemplate::EnemyTemplate(const std::string& name, const int& health, 
	const std::vector<const BulletConfiguration*>& configs, const Vector2& scale, const Vector2& collisionScale, const SpriteResource* resource)
	: m_name(name),
	m_health(health),
	m_bulletConfigurations(configs),
	m_scale(scale),
	m_collisionScale(collisionScale),
	m_resource(resource),
	m_speed(0.5f),
	m_points(0)
{

}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//Setters
void EnemyTemplate::SetName(const std::string& name)
{
	m_name = name;
}
void EnemyTemplate::SetHealth(const int& health)
{
	m_health = health;
}
void EnemyTemplate::AddBulletConfig(const BulletConfiguration* config)
{
	m_bulletConfigurations.push_back(config);
}
void EnemyTemplate::SetBulletConfigs(const std::vector<const BulletConfiguration*> configs)
{
	m_bulletConfigurations = configs;
}
void EnemyTemplate::SetScale(const Vector2& scale)
{
	m_scale = scale;
}
void EnemyTemplate::SetCollisionScale(const Vector2& collisionScale)
{
	m_collisionScale = collisionScale;
}
void EnemyTemplate::SetSpriteResource(const SpriteResource* resource)
{
	m_resource = nullptr;
	m_resource = resource;
}
void EnemyTemplate::SetSpeed(const float& speed)
{
	m_speed = speed;
}

void EnemyTemplate::SetPointsWorth(const int& points)
{
	m_points = points;
}

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//Getters
const std::string EnemyTemplate::GetName() const
{
	return m_name;
}
const int EnemyTemplate::GetHealth() const
{
	return m_health;
}
const std::vector<const BulletConfiguration*> EnemyTemplate::GetBulletConfigurations() const
{
	return m_bulletConfigurations;
}
const Vector2 EnemyTemplate::GetScale() const
{
	return m_scale;
}
const Vector2 EnemyTemplate::GetCollisionScale() const
{
	return m_collisionScale;
}
const SpriteResource* EnemyTemplate::GetSpriteResource() const
{
	return m_resource;
}
const float EnemyTemplate::GetSpeed() const
{
	return m_speed;
}

const int EnemyTemplate::GetPointsWorth() const 
{
	return m_points;
}


//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//Spawners
Ship* EnemyTemplate::SpawnShipNoController(const Vector2& position, const float& rotation) const
{
	Ship* ship = new Ship(m_resource, SHIPTYPE_ENEMY, nullptr);
	ship->SetPosition(position);
	ship->SetScale(m_scale);
	ship->SetBothHPs(m_health);
	ship->SetCollisionRadius(m_collisionScale);
	ship->ChangeLayers(CommonVariables::s_enemyLayer);
	ship->SetBulletConfiguration(m_bulletConfigurations);
	ship->SetRotation(rotation);
	ship->SetSpeed(m_speed);
	ship->SetPointsWorth(m_points);
	//ship->SetVelocity(ship->GetForward() * m_speed);

	return ship;
}
Ship* EnemyTemplate::SpawnShipWithPlayerController(const Vector2& position, const float& rotation) const
{
	Ship* ship = SpawnShipNoController(position, rotation);
	ControllerGenerator::CreateController(CONTROLLER_PLAYER_TYPE, ship);
	ship->SetTeamID(GameEntity::s_playerTeamID);
	return ship;
}
Ship* EnemyTemplate::SpawnShipWithNPCController(const Vector2& position, const float& rotation) const
{
	Ship* ship = SpawnShipNoController(position, rotation);
	ControllerGenerator::CreateController(CONTROLLER_NPC_TYPE, ship);
	ship->SetTeamID(GameEntity::s_enemyTeamID);
	return ship;
}