#include "Game/Ships/Controller/Controller.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/ExtendedMath.hpp"

std::map<ControllerType, ControllerGenerator>* ControllerGenerator::s_ControllerGenerators = nullptr;

void ControllerGenerator::ClearAllControllerGenerators()
{
	if (s_ControllerGenerators == nullptr)
	{
		return;
	}
	delete s_ControllerGenerators;
	s_ControllerGenerators = nullptr;
}

const GameListCleaner ControllerGenerator::s_ClearControllerGenerators = GameListCleaner(LIBRARY_CLEANER, "ControllerGeneratorCleaner", ClearAllControllerGenerators);

ControllerGenerator::ControllerGenerator(const ControllerType& type, ControllerCreationFunc* controllerCreationFunc)
	: m_ControllerCreationFunc(controllerCreationFunc),
	m_controllerType(type)
{
	if (s_ControllerGenerators == nullptr)
	{
		s_ControllerGenerators = new std::map<ControllerType, ControllerGenerator>();
	}

	std::map<ControllerType, ControllerGenerator>::iterator it = s_ControllerGenerators->find(type);
	if (it == s_ControllerGenerators->end())
	{
		s_ControllerGenerators->insert(std::pair<ControllerType, ControllerGenerator>(type, *this));
	}
	else
	{
		//Complain
		GUARANTEE_RECOVERABLE(false, std::to_string(type) + " ControllerGenerator already exists!");
	}
}

Controller* ControllerGenerator::CreateController(const ControllerType& type, Ship* ship)
{
	if (s_ControllerGenerators == nullptr)
	{
		return nullptr;
	}
	std::map<ControllerType, ControllerGenerator>::const_iterator it = s_ControllerGenerators->find(type);
	if (it == s_ControllerGenerators->end() || it->second.m_ControllerCreationFunc == nullptr)
	{
		return nullptr;
	}
	return it->second.m_ControllerCreationFunc(ship);
}
std::map<ControllerType, ControllerGenerator>* ControllerGenerator::GetControllerGenerators()
{
	return s_ControllerGenerators;
}
void ControllerGenerator::DeleteControllerGenerators()
{
	if (s_ControllerGenerators != nullptr)
	{
		delete s_ControllerGenerators;
		s_ControllerGenerators = nullptr;
	}
}


//----------------------------------------------------------------------------------------------------------------------

void ClearAllControllers()
{
	if (Controller::s_controllers == nullptr)
	{
		return;
	}
	for (size_t i = 0; Controller::s_controllers != nullptr && i < Controller::s_controllers->size(); i)
	{
		Controller* controller = Controller::s_controllers->at(i);
		if (controller != nullptr)
		{
			delete controller;
		}
	}
	if (Controller::s_controllers != nullptr)
	{
		Controller::s_controllers->clear();
		delete Controller::s_controllers;
		Controller::s_controllers = nullptr;
	}
}

std::vector<Controller*>* Controller::s_controllers = nullptr;
const GameListCleaner Controller::s_controllerCleaner = GameListCleaner(ENTITY_CLEANER, "ControllerCleaner", ClearAllControllers);

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Constructors
Controller::Controller(ControllerType type, Ship* ownedShip)
	: m_type(type),
	m_ownedShip(ownedShip),
	m_moveForward(MovementInterface_None),
	m_moveRightHorizontal(MovementInterface_None),
	m_destroying(false),
	m_shoot(false)
{
	if (s_controllers == nullptr)
	{
		s_controllers = new std::vector<Controller*>();
	}
	s_controllers->push_back(this);
	if (ownedShip != nullptr)
	{
		ownedShip->SetController(this, false);
	}
}
Controller::~Controller()
{
	if (m_ownedShip != nullptr)
	{
		m_ownedShip->ClearController(false);
		m_ownedShip = nullptr;
	}

	if (s_controllers == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < s_controllers->size(); i++)
	{
		if (s_controllers->at(i) == this)
		{
			s_controllers->erase(s_controllers->begin() + i);
			break;
		}
	}
	if (s_controllers->size() == 0)
	{
		delete s_controllers;
		s_controllers = nullptr;
	}
}

void Controller::Update(const float& deltaSeconds)
{
	deltaSeconds;
	if (m_destroying == true)
	{
		m_ownedShip->ClearController(false);
		m_ownedShip = nullptr;
	}

}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Setters
void Controller::SetShip(Ship* ship, const bool& notifyParent)
{
	if (m_ownedShip != nullptr && notifyParent == true)
	{
		m_ownedShip->SetController(nullptr, false);
	}

	m_ownedShip = nullptr;
	m_ownedShip = ship;

	if (m_ownedShip != nullptr && notifyParent == true)
	{
		m_ownedShip->SetController(this, false);
	}

}
void Controller::ClearShip(const bool& notifyParent)
{
	if (notifyParent == true && m_ownedShip != nullptr)
	{
		m_ownedShip->ClearController(false);
	}

	m_ownedShip = nullptr;
}
void Controller::DestroyShip()
{
	m_ownedShip->SetIsAlive(false);
	m_ownedShip = nullptr;
}
void Controller::SetDestroying(const bool& destroying)
{
	m_destroying = destroying;
}
const bool Controller::SetMoveForward(const MovementInterface& forward)
{
	bool gotSet = false;
	if (forward == MovementInterface_None
		|| forward == MovementInterface_Forward
		|| forward == MovementInterface_Backward)
	{
		m_moveForward = forward;
		gotSet = true;
	}
	return gotSet;
}
const bool Controller::SetMoveRight(const MovementInterface& right)
{
	bool gotSet = false;
	if (right == MovementInterface_None
		|| right == MovementInterface_Forward
		|| right == MovementInterface_Backward)
	{
		m_moveRightHorizontal = right;
		gotSet = true;
	}
	return gotSet;
}
void Controller::SetShoot(const bool& shoot)
{
	m_shoot = shoot;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//Getters
const ControllerType Controller::GetControllerType() const
{
	return m_type;
}
const Ship* Controller::GetOwnedShip() const
{
	return m_ownedShip;
}
Ship* Controller::GetEditableShip() const
{
	return m_ownedShip;
}
const bool Controller::GetDestroying() const
{
	return m_destroying;
}
const MovementInterface Controller::GetMoveForward() const
{
	return m_moveForward;
}
const MovementInterface Controller::GetMoveRight() const
{
	return m_moveRightHorizontal;
}
const Vector2 Controller::GetMovementAsNormalizedVector(const float& rotationDeg) const
{
	Vector2 movement = Vector2::vec2_zeros;
	const float rightwardsDirection = rotationDeg - 90.f;
	const Vector2 forwardDir = Vector2(CosDegrees(rotationDeg), SinDegrees(rotationDeg));
	const Vector2 rightDir = Vector2(CosDegrees(rightwardsDirection), SinDegrees(rightwardsDirection));

	if (m_moveForward == MovementInterface_Forward)
	{
		movement += forwardDir;
	}
	else if (m_moveForward == MovementInterface_Backward)
	{
		movement -= forwardDir;
	}

	if (m_moveRightHorizontal == MovementInterface_Forward)
	{
		movement += rightDir;
	}
	else if (m_moveRightHorizontal == MovementInterface_Backward)
	{
		movement -= rightDir;
	}

	movement.normalize();

	return movement;
}
const bool Controller::GetWantsShoot() const
{
	return m_shoot;
}