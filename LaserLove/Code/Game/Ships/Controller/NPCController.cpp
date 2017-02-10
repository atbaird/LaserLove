#include "Game/Ships/Controller/NPCController.hpp"


Controller* CreateNPCController(Ship* ship)
{
	return new NPCController(ship);
}

const ControllerGenerator NPCController::s_npcControllerGenerator = ControllerGenerator(CONTROLLER_NPC_TYPE, CreateNPCController);


NPCController::NPCController(Ship* ownedShip)
	: Controller(CONTROLLER_NPC_TYPE, ownedShip)
{

}
NPCController::~NPCController()
{
	Controller::~Controller();
}


void NPCController::Update(const float& deltaSeconds)
{

	SetMoveForward(MovementInterface_Forward);
	SetShoot(true);

	Controller::Update(deltaSeconds);
}