#pragma once

#ifndef NPCCONTROLLER_HPP
#define NPCCONTROLLER_HPP
#include "Game/Ships/Controller/Controller.hpp"

Controller* CreateNPCController(Ship* ship);

class NPCController : public Controller
{
private:
	static const ControllerGenerator s_npcControllerGenerator;
public:
	NPCController(Ship* ownedShip = nullptr);
	virtual ~NPCController() override;

	virtual void Update(const float& deltaSeconds) override;
};
#endif