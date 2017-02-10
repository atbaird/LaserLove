#pragma once

#ifndef SHIP_HPP
#define SHIP_HPP
#include "Game/GameEntity/GameEntity.hpp"
#include "Game/Ships/ShipType.hpp"
#include "Game/Bullets/BulletConfiguration.hpp"

enum MovementInterface
{
	MovementInterface_Forward, //if moveHorizontal, consider this the same as Rightwards movement
	MovementInterface_None,
	MovementInterface_Backward, //if moveHorizontal, consider this the same as Leftwards movement
	NUM_OF_MovementInterfaces
};

class Controller;
enum ControllerType;

void ShipCleaner();

class Ship : public GameEntity
{
private:
	ShipType m_shipType;
	Controller* m_controller;
	std::vector<const BulletConfiguration*> m_bulletConfigurations;

	static const float s_playerFireRate;
	static const float s_NPCFireRate;
	static const float s_playerInvincibilitySeconds;
	static const float s_NPCInvincibilitySeconds;
	static const float s_secondsBeforeEnableDisableShipVisibilityIfInvincibile;

	int m_currentHP;
	int m_maxHP;
	int m_pointsWorth;
	bool m_canMove;
	bool m_justShotABullet;
	float m_speed;
	float m_secondsSinceLastFired;
	float m_secondsBeforeCanFireAgain;
	float m_secondsSinceLastHurt;
	float m_secondsBeforeNoLongerInvincible;
	float m_secondsSinceLastFlippedVisibility;
	bool m_invincible;
	size_t m_currentBulletConfig;
	static const GameListCleaner s_ShipCleaner;
public:
	static int s_playerPoints;
	static std::vector<Ship*>* s_ships;
	static Ship* s_playerShip;

	//Constructors
	Ship(const ShipType& shipType, Controller* controllerType = nullptr);
	Ship(const SpriteResource* resource, const ShipType& shipType, Controller* controllerType = nullptr);
	Ship(const size_t& spriteResourceID, const ShipType& shipType, Controller* controllerType = nullptr);
	virtual ~Ship() override;

	//Update
	virtual void Update(const float& deltaSeconds) override;
	virtual void FinalizationPass() override;

	//Setters
	virtual void HurtShip();
	virtual void SetController(Controller* controller, const bool& notifyParent = true);
	virtual void ClearController(const bool& notifyParent = true);
	virtual void SetBothHPs(const int& hp);
	virtual void SetMaxHP(const int& maxHP);
	virtual void SetCurrentHP(const int& curHP, const bool& setIsAliveIfValid = true);
	virtual void SetCanMove(const bool& move);
	virtual void SetBulletConfiguration(const std::vector<const BulletConfiguration*>& configurations);
	virtual void AddBulletConfiguration(const BulletConfiguration* configuration);
	virtual void SetShipType(const ShipType& type);
	virtual void SetSpeed(const float& speed);
	virtual void SetPointsWorth(const int& points);

	//Getters
	virtual const ShipType GetShipType() const;
	virtual const ControllerType GetControllerType() const;
	virtual const Controller* GetController() const;
	virtual Controller* GetEditableController() const;
	virtual const int GetCurrentHP() const;
	virtual const int GetMaxHP() const;
	virtual const bool GetCanMove() const;
	virtual const float GetTrueForward() const;
	virtual const Vector2 GetForward() const;
	virtual const Vector2 GetRight() const;
	virtual const bool GetIsInvincible() const;
	virtual const bool GetJustShotABullet() const;
	virtual const int GetPointsWorth() const;
};
#endif