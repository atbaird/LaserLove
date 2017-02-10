#pragma once

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Game/Ships/ControllerType.hpp"
#include "Game/Ships/Ship.hpp"
#include "Game/GameListCleaner/GameListCleaner.hpp"

class Controller;

typedef Controller* (ControllerCreationFunc)(Ship* ship);


class ControllerGenerator
{
private:
	ControllerType m_controllerType;
	ControllerCreationFunc* m_ControllerCreationFunc;

	static const GameListCleaner s_ClearControllerGenerators;
public:
	ControllerGenerator(const ControllerType& type, ControllerCreationFunc* controllerCreationFunc);

	static Controller* CreateController(const ControllerType& type, Ship* ship = nullptr);
	static std::map<ControllerType, ControllerGenerator>* GetControllerGenerators();
	static void DeleteControllerGenerators();
	

	static void ClearAllControllerGenerators();
protected:
	static std::map<ControllerType, ControllerGenerator>* s_ControllerGenerators;
};

void ClearAllControllers();

class Controller
{
private:
	ControllerType m_type;
	Ship* m_ownedShip;
	MovementInterface m_moveForward;
	MovementInterface m_moveRightHorizontal;
	bool m_shoot;
	bool m_destroying;
	static const GameListCleaner s_controllerCleaner;
public:
	static std::vector<Controller*>* s_controllers;
	//Constructors
	Controller(ControllerType type = CONTROLLER_NONE_TYPE, Ship* ownedShip = nullptr);
	virtual ~Controller();

	//Update
	virtual void Update(const float& deltaSeconds);

	//Setters
	void SetShip(Ship* ship, const bool& notifyParent = true);
	void ClearShip(const bool& notifyParent = true);
	void DestroyShip();
	void SetDestroying(const bool& destroying);
	const bool SetMoveForward(const MovementInterface& forward);
	const bool SetMoveRight(const MovementInterface& right);
	void SetShoot(const bool& shoot);

	//Getters
	const ControllerType GetControllerType() const;
	const Ship* GetOwnedShip() const;
	Ship* GetEditableShip() const;
	const bool GetDestroying() const;
	const MovementInterface GetMoveForward() const;
	const MovementInterface GetMoveRight() const;
	const Vector2 GetMovementAsNormalizedVector(const float& rotationDeg = 0.f) const;
	const bool GetWantsShoot() const;
};
#endif