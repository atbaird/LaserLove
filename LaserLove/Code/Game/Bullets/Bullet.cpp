#include "Game/Bullets/Bullet.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Game/Ships/Ship.hpp"

void ClearBullets()
{
	if (Bullet::s_bullets == nullptr)
	{
		return;
	}
	Bullet::m_clearingBullets = true;
	for (size_t i = 0; i < Bullet::s_bullets->size(); i++)
	{
		Bullet* bullet = Bullet::s_bullets->at(i);
		if (bullet != nullptr)
		{
			delete bullet;
		}
	}
	Bullet::s_bullets->clear();
	Bullet::m_clearingBullets = false;
	delete Bullet::s_bullets;
	Bullet::s_bullets = nullptr;
}

const float Bullet::m_commonBulletSpeed = 1.f;
std::vector<Bullet*>* Bullet::s_bullets = nullptr;
bool Bullet::m_clearingBullets = false;
const GameListCleaner Bullet::s_bulletCleaner = GameListCleaner(ENTITY_CLEANER, "BulletCleaner", ClearBullets);
size_t Bullet::s_playerKillsBeforeDeath = 0;

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Constructors
Bullet::Bullet(const BulletType& type, const Vector2& bulletNormalizedDirection, const float& angularVel)
	: GameEntity(TYPE_BULLET),
	m_bulletType(type),
	m_instigatorController(nullptr),
	m_angularVelocity(angularVel),
	m_speed(0.f),
	m_numTimesExplodeIntoMoreBullets(0),
	m_dieAfterXSecondsIfNoCollide(-1.f),
	m_secondsAlive(0.f),
	m_canExplode(true),
	m_angularVelocityAddedAfterExplosion(0.f)
{
	SetVelocity(bulletNormalizedDirection * m_commonBulletSpeed);
	if (Bullet::s_bullets == nullptr)
	{
		Bullet::s_bullets = new std::vector<Bullet*>();
	}
	Bullet::s_bullets->push_back(this);
}
Bullet::Bullet(const size_t& spriteResourceID, const BulletType& type, const Vector2& bulletNormalizedDirection, const float& angularVel)
	: GameEntity(TYPE_BULLET, spriteResourceID),
	m_bulletType(type),
	m_instigatorController(nullptr),
	m_angularVelocity(angularVel),
	m_speed(0.f),
	m_numTimesExplodeIntoMoreBullets(0),
	m_dieAfterXSecondsIfNoCollide(-1.f),
	m_secondsAlive(0.f),
	m_canExplode(true),
	m_angularVelocityAddedAfterExplosion(0.f)
{
	float rot = RadiansToDegrees(atan2(bulletNormalizedDirection.y, bulletNormalizedDirection.x));
	SetRotation(rot);
	SetVelocity(bulletNormalizedDirection * m_commonBulletSpeed);
	if (Bullet::s_bullets == nullptr)
	{
		Bullet::s_bullets = new std::vector<Bullet*>();
	}
	Bullet::s_bullets->push_back(this);
}
Bullet::Bullet(const SpriteResource* resource, const BulletType& type, const Vector2& bulletNormalizedDirection, const float& angularVel)
	: GameEntity(TYPE_BULLET, resource),
	m_bulletType(type),
	m_instigatorController(nullptr),
	m_angularVelocity(angularVel),
	m_speed(0.f),
	m_numTimesExplodeIntoMoreBullets(0),
	m_dieAfterXSecondsIfNoCollide(-1.f),
	m_secondsAlive(0.f),
	m_canExplode(true),
	m_angularVelocityAddedAfterExplosion(0.f)
{
	float rot = RadiansToDegrees(atan2(bulletNormalizedDirection.y, bulletNormalizedDirection.x));
	SetRotation(rot);
	SetVelocity(bulletNormalizedDirection * m_commonBulletSpeed);
	if (Bullet::s_bullets == nullptr)
	{
		Bullet::s_bullets = new std::vector<Bullet*>();
	}
	Bullet::s_bullets->push_back(this);
}

Bullet::Bullet(SpriteAnimationSequence* sequence, const float& time, const BulletType& type,
	const Vector2& bulletNormalizedDirection, const float& angularVel)
	: GameEntity(TYPE_BULLET, sequence, nullptr, time),
	m_bulletType(type),
	m_instigatorController(nullptr),
	m_angularVelocity(angularVel),
	m_speed(0.f),
	m_numTimesExplodeIntoMoreBullets(0),
	m_dieAfterXSecondsIfNoCollide(-1.f),
	m_secondsAlive(0.f),
	m_canExplode(true),
	m_angularVelocityAddedAfterExplosion(0.f)
{
	float rot = RadiansToDegrees(atan2(bulletNormalizedDirection.y, bulletNormalizedDirection.x));
	SetRotation(rot);
	SetVelocity(bulletNormalizedDirection * m_commonBulletSpeed);
	if (Bullet::s_bullets == nullptr)
	{
		Bullet::s_bullets = new std::vector<Bullet*>();
	}
	Bullet::s_bullets->push_back(this);
}
Bullet::Bullet(const Bullet& otherBullet)
	: GameEntity(otherBullet),
	m_bulletType(otherBullet.GetBulletType()),
	m_instigatorController(otherBullet.GetEditableInstigatorController()),
	m_angularVelocity(otherBullet.GetAngularVelocity()),
	m_speed(otherBullet.GetSpeed()),
	m_numTimesExplodeIntoMoreBullets(0),
	m_dieAfterXSecondsIfNoCollide(otherBullet.GetSecondsBeforeDie()),
	m_secondsAlive(0.f),
	m_canExplode(otherBullet.m_canExplode),
	m_angularVelocityAddedAfterExplosion(otherBullet.m_angularVelocityAddedAfterExplosion)
{

}
Bullet::~Bullet()
{
	GameEntity::~GameEntity();
	if (s_bullets == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < Bullet::s_bullets->size(); i++)
	{
		if (Bullet::s_bullets->at(i) == this)
		{
			Bullet::s_bullets->erase(Bullet::s_bullets->begin() + i);
			break;
		}
	}
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Update
void Bullet::Update(const float& deltaSeconds)
{
	if (GetTeamID() == GameEntity::s_enemyTeamID)
	{
		SetTint(Rgba::s_White);
	}
	else
	{
		SetTint(Rgba::s_Gray6);
	}
	m_secondsAlive += deltaSeconds;
	if (GetJustCreated() == false)
	{
		if (m_dieAfterXSecondsIfNoCollide > 0.f && m_secondsAlive >= m_dieAfterXSecondsIfNoCollide)
		{
			SetIsAlive(false);
			return;
		}
	}

	float angle = GetRotation();
	angle += (m_angularVelocity * deltaSeconds);
	SetRotation(angle);
	Vector2 vel = Vector2(CosDegrees(angle), SinDegrees(angle)) * m_speed;
	SetVelocity(vel);

	if (GetJustCreated() == false && Ship::s_ships != nullptr)
	{
		for (size_t i = 0; i < Ship::s_ships->size(); i++)
		{
			Ship* ship = Ship::s_ships->at(i);
			if (ship != nullptr)
			{
				if (ship->GetTeamID() != GetTeamID() && ship->GetIsInvincible() == false)
				{
					const EllipseCollision* shipCollision = ship->GetCollision();
					const EllipseCollision* bulletCollision = GetCollision();
					CollisionIntersection intersection = EllipseCollision::TestCircleCollisionsIntersecting(shipCollision, bulletCollision);
					if (intersection.m_intersected == true)
					{
						ship->HurtShip();
						if (GetTeamID() == GameEntity::s_playerTeamID && ship->GetTeamID() != GetTeamID() 
							&& (ship->GetIsAlive() == false))
						{
							s_playerKillsBeforeDeath++;
							Ship::s_playerPoints = Ship::s_playerPoints + ship->GetPointsWorth();
						}
						SetIsAlive(false);
						return;
					}
				}
			}
		}
	}

	GameEntity::Update(deltaSeconds);
}
void Bullet::FinalizationPass()
{
	if (GetIsAlive() == false && m_clearingBullets == false && m_numTimesExplodeIntoMoreBullets > 0 && m_canExplode == true)
	{
		const size_t newNumTimesExplode = m_numTimesExplodeIntoMoreBullets - 1;

		for (size_t i = 0; i < 8; i++)
		{
			Bullet* currentBullet = nullptr;
			//Create new bullet per 8 directions
			if (GetSequence() != nullptr)
			{
				currentBullet = new Bullet(GetEditableSequence(), 0.f);
			}
			else
			{
				currentBullet = new Bullet(GetSpriteResource());
			}
			currentBullet->SetPosition(GetPosition());
			currentBullet->SetSpeed(GetSpeed());
			currentBullet->SetTeamID(GetTeamID());
			currentBullet->SetSecondsBeforeDie(GetSecondsBeforeDie());
			currentBullet->SetAngularVelocity(GetAngularVelocity() + GetAngularVelocityAddedAfterExplosion());
			currentBullet->SetAngularVelocityAddedAfterExplosion(GetAngularVelocityAddedAfterExplosion());
			currentBullet->SetInstigatorController(GetEditableInstigatorController());
			currentBullet->SetBulletType(GetBulletType());
			currentBullet->SetNumTimesExplode(newNumTimesExplode);
			currentBullet->SetCollisionRadius(GetCollision()->GetXYRadius());
			currentBullet->SetRotation(0.f + ((float)i * 45.f));
			currentBullet->SetAddedRotationForImage(GetAddedRotationForImage());
		}
	}
}


//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Setters
void Bullet::SetBulletType(const BulletType& type)
{
	m_bulletType = type;
}
void Bullet::SetInstigatorController(Controller* controller)
{
	m_instigatorController = nullptr;
	m_instigatorController = controller;
}
void Bullet::SetAngularVelocity(const float& angVel)
{
	m_angularVelocity = angVel;
}
void Bullet::SetSpeed(const float& speed)
{
	m_speed = speed;
}
void Bullet::SetNumTimesExplode(const size_t& numTimesExplode)
{
	m_numTimesExplodeIntoMoreBullets = numTimesExplode;
}
void Bullet::SetSecondsBeforeDie(const float& secondsDieIfNoCollide)
{
	m_dieAfterXSecondsIfNoCollide = secondsDieIfNoCollide;
}
void Bullet::SetCanExplode(const bool& explode)
{
	m_canExplode = explode;
}
void Bullet::SetAngularVelocityAddedAfterExplosion(const float& angularVelocityAddedAfterExplosion)
{
	m_angularVelocityAddedAfterExplosion = angularVelocityAddedAfterExplosion;
}

//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//Getters
const BulletType Bullet::GetBulletType() const
{
	return m_bulletType;
}
const Controller* Bullet::GetInstigatorController() const
{
	return m_instigatorController;
}
Controller* Bullet::GetEditableInstigatorController() const
{
	return m_instigatorController;
}
const float Bullet::GetAngularVelocity() const 
{
	return m_angularVelocity;
}
const float Bullet::GetSpeed() const
{
	return m_speed;
}
const size_t Bullet::GetNumTimesExplode() const
{
	return m_numTimesExplodeIntoMoreBullets;
}
const float Bullet::GetSecondsBeforeDie() const
{
	return m_dieAfterXSecondsIfNoCollide;
}
const bool Bullet::GetCanExplode() const
{
	return m_canExplode;
}
const float Bullet::GetAngularVelocityAddedAfterExplosion() const
{
	return m_angularVelocityAddedAfterExplosion;
}