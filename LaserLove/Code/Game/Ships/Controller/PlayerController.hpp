#pragma once

#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP
#include "Game/Ships/Controller/Controller.hpp"

Controller* CreatePlayerController(Ship* ship);

class PlayerController : public Controller
{
private:
	static const ControllerGenerator s_playerControllerGenerator;
	char m_keyForward;
	char m_keyBack;
	char m_keyRight;
	char m_keyLeft;
	char m_shootKey;
public:
	PlayerController(Ship* ownedShip = nullptr);
	virtual ~PlayerController() override;

	virtual void Update(const float& deltaSeconds) override;
};
#endif