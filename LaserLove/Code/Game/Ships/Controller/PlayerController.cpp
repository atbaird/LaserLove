#include "Game/Ships/Controller/PlayerController.hpp"
#include "Engine/Core/Input.hpp"

Controller* CreatePlayerController(Ship* ship)
{
	return new PlayerController(ship);
}

const ControllerGenerator PlayerController::s_playerControllerGenerator = ControllerGenerator(CONTROLLER_PLAYER_TYPE, CreatePlayerController);

PlayerController::PlayerController(Ship* ownedShip)
	: Controller(CONTROLLER_PLAYER_TYPE, ownedShip),
	m_keyForward('W'),
	m_keyBack('S'),
	m_keyRight('D'),
	m_keyLeft('A'),
	m_shootKey(VK_SPACE)
{

}
PlayerController::~PlayerController()
{
	Controller::~Controller();
}

void PlayerController::Update(const float& deltaSeconds)
{
	Controller::Update(deltaSeconds);
	if (g_input == nullptr)
	{
		return;
	}

	//Forward and Back
	if (g_input->isKeyDown(m_keyForward) == true && g_input->isKeyDown(m_keyBack) == false)
	{
		SetMoveForward(MovementInterface_Forward);
	}
	else if (g_input->isKeyDown(m_keyForward) == false && g_input->isKeyDown(m_keyBack) == true)
	{
		SetMoveForward(MovementInterface_Backward);
	}
	else
	{
		SetMoveForward(MovementInterface_None);
	}

	if (g_input->isKeyDown(m_keyRight) == true && g_input->isKeyDown(m_keyLeft) == false)
	{
		SetMoveRight(MovementInterface_Forward);
	}
	else if (g_input->isKeyDown(m_keyRight) == false && g_input->isKeyDown(m_keyLeft) == true)
	{
		SetMoveRight(MovementInterface_Backward);
	}
	else
	{
		SetMoveRight(MovementInterface_None);
	}

	if (g_input->isKeyDown(m_shootKey))
	{
		SetShoot(true);
	}
	else
	{
		SetShoot(false);
	}
}