#pragma once

#ifndef BULLET_HPP
#define BULLET_HPP
#include "Game/GameEntity/GameEntity.hpp"
#include "Game/Bullets/BulletType.hpp"

void ClearBullets();

class Controller;

class Bullet : public GameEntity
{
private:
	BulletType m_bulletType;
	Controller* m_instigatorController;
	float m_angularVelocity;
	float m_speed;
	size_t m_numTimesExplodeIntoMoreBullets;
	float m_dieAfterXSecondsIfNoCollide;
	float m_secondsAlive;
	float m_angularVelocityAddedAfterExplosion;
	bool m_canExplode;
	static const float m_commonBulletSpeed;
	static const GameListCleaner s_bulletCleaner;
public:
	static size_t s_playerKillsBeforeDeath;
	static bool m_clearingBullets;
	static std::vector<Bullet*>* s_bullets;
	//Constructors
	Bullet(const BulletType& type = INVALID_BULLET_TYPE, const Vector2& bulletNormalizedDirection = Vector2(0.f, 1.f), const float& angularVel = 0.f);
	Bullet(const size_t& spriteResourceID, const BulletType& type = INVALID_BULLET_TYPE, const Vector2& bulletNormalizedDirection = Vector2(0.f, 1.f), const float& angularVel = 0.f);
	Bullet(const SpriteResource* resource, const BulletType& type = INVALID_BULLET_TYPE, const Vector2& bulletNormalizedDirection = Vector2(0.f, 1.f), const float& angularVel = 0.f);
	Bullet(SpriteAnimationSequence* sequence, const float& time = 0.f, const BulletType& type = INVALID_BULLET_TYPE, const Vector2& bulletNormalizedDirection = Vector2(0.f, 1.f), const float& angularVel = 0.f);
	Bullet(const Bullet& otherBullet);
	virtual ~Bullet() override;

	//Update
	virtual void Update(const float& deltaSeconds) override;
	virtual void FinalizationPass() override;

	//Setters
	virtual void SetBulletType(const BulletType& type);
	virtual void SetInstigatorController(Controller* controller);
	virtual void SetAngularVelocity(const float& angVel);
	virtual void SetSpeed(const float& speed);
	virtual void SetNumTimesExplode(const size_t& numTimesExplode);
	virtual void SetSecondsBeforeDie(const float& secondsDieIfNoCollide);
	virtual void SetCanExplode(const bool& explode);
	virtual void SetAngularVelocityAddedAfterExplosion(const float& angularVelocityAddedAfterExplosion);

	//Getters
	virtual const BulletType GetBulletType() const;
	virtual const Controller* GetInstigatorController() const;
	virtual Controller* GetEditableInstigatorController() const;
	virtual const float GetAngularVelocity() const;
	virtual const float GetSpeed() const;
	virtual const size_t GetNumTimesExplode() const;
	virtual const float GetSecondsBeforeDie() const;
	virtual const bool GetCanExplode() const;
	virtual const float GetAngularVelocityAddedAfterExplosion() const;
};
#endif