#pragma once

#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP
#include "Engine/2D/AnimatedSprite.hpp"
#include "Game/GameEntity/GameEntityType.hpp"
#include "Game/Collision/EllipseCollision.hpp"
#include "Game/GameListCleaner/GameListCleaner.hpp"
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"


void ClearAllGameEntities();

class GameEntity : public AnimatedSprite
{
private:
	GameEntityType m_gameEntityType;
	EllipseCollision* m_collision;
	GameEntity* m_instigator;
	Vector2 m_velocity;
	Vector2 m_acceleration;
	Vector2 m_previousPosition;
	size_t m_teamID;
	bool m_justCreated;
	bool m_moving;
	bool m_yVelCrossedZeroBoundary;
	bool m_xVelCrossedZeroBoundary;
	bool m_alive;
	static const GameListCleaner s_GameEntityCleaner;
public:
	static const size_t s_playerTeamID;
	static const size_t s_enemyTeamID;
	static std::vector<GameEntity*>* s_gameEntities;
	//Constructors
	GameEntity(const GameEntityType& type = INVALID_GAMEENTITY_TYPE, GameEntity* instigator = nullptr);
	GameEntity(const GameEntityType& type, const SpriteResource* resource, GameEntity* instigator = nullptr);
	GameEntity(const GameEntityType& type, const size_t& spriteResourceID, GameEntity* instigator = nullptr);
	GameEntity(const GameEntityType& type, SpriteAnimationSequence* sequence, GameEntity* instigator = nullptr, const float& time = 0.f);
	virtual ~GameEntity() override;

	//Update
	virtual void Update(const float& deltaSeconds) override;
	virtual void FinalizationPass();
	virtual void Render() const override;
private:
	void AcclerationUpdate(const float& deltaSeconds);
	void VelocityUpdate(const float& deltaSeconds);
public:

	//Setters
	virtual void SetGamenEntityType(const GameEntityType& type);
	virtual void SetVelocity(const Vector2& velocity);
	virtual void SetAcceleration(const Vector2& acceleration);
	virtual void ZeroOutVelocity();
	virtual void ZeroOutYVelocity();
	virtual void ZeroOutXVelcoity();
	virtual void ZeroOutAcceleration();
	virtual void ZeroOutYAcceleration();
	virtual void ZeroOutXAcceleration();
	virtual void SetIsAlive(const bool& alive);
	virtual void SetCollisionRadius(const float& radius);
	virtual void SetCollisionRadius(const Vector2& xyradius);
	virtual void SetTeamID(const size_t& teamID);

	//Getters
	virtual const GameEntityType GetGameEntityType() const;
	virtual const Vector2 GetPreviousPosition() const;
	virtual const Vector2 GetVelocity() const;
	virtual const Vector2 GetAcceleration() const;
	virtual const bool GetIsMoving() const;
	virtual const bool GetYVelCrossedZeroBoundary() const;
	virtual const bool GetXVelCrossedZeroBoundary() const;
	virtual const bool GetIsAlive() const;
	virtual const EllipseCollision* GetCollision() const;
	virtual const CollisionIntersection TestCollisionBetweenThisAsOwnerAndOther(GameEntity* other);
	virtual const size_t GetTeamID() const;
	virtual const bool GetJustCreated() const;

	//static methods
	static void FinalizationPassAll();
};
#endif