#include "Engine/Core/Input.hpp"

//--------------------------------------------
Input* g_input = nullptr;

// Constructors
Input::Input(const int& numControllers, const int& numOfMice)
: m_maxNumControllers(numControllers),
m_maxNumOfMice(numOfMice)
{
	if (numOfMice == 0)
	{
		m_mice = nullptr;
	}
	else
	{
		m_mice = new Mouse(numOfMice);
	}
	if (numControllers == 0)
	{
		m_controllers = nullptr;
	}
	else
	{
		m_controllers = new XboxController*[m_maxNumControllers];
		for (int i = 0; i < m_maxNumControllers; i++)
		{
			m_controllers[i] = new XboxController(i);
		}
	}
	m_keyboard = new Keyboard();

}
Input::~Input()
{
	if (m_mice != nullptr)
	{
		delete m_mice;
	}
	if (m_controllers != nullptr)
	{
		for (int i = 0; i < m_maxNumControllers; i++)
		{
			if (m_controllers[i] != nullptr)
			{
				delete m_controllers[i];
			}
		}
		delete m_controllers;
	}
	if (m_keyboard != nullptr)
	{
		delete m_keyboard;
	}
}



//Keyboard


void Input::SetKeyDownStatus(const unsigned char& asKey, const bool& isDown)
{
	if (m_keyboard == nullptr)
	{
		return;
	}
	m_keyboard->setKeyDownStatus(asKey, isDown);
}
const bool Input::IsKeyDown(const unsigned char& asKey) const
{
	if (m_keyboard == nullptr)
	{
		return false;
	}
	return m_keyboard->isKeyDown(asKey);
}

//XboxControllers
const bool Input::isThisControllerReal(const int& index) const
{
	if (index < 0 
		|| index > m_maxNumControllers 
		|| m_controllers == nullptr
		|| m_controllers[index] == nullptr)
		return false;

	return true;
}
const int Input::amIPluggedIn(const int& index) const
{
	if (isThisControllerReal(index) == false)
		return 1;
	return m_controllers[index]->amIPluggedIn();
}
const DWORD Input::amIPluggedInDWORD(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return DWORD();
	}
	return m_controllers[index]->amIPluggedInDWORD();
}
void Input::Update()
{
	for (int i = 0; i < m_maxNumControllers; i++)
	{
		if (m_controllers[i] != nullptr)
		{
			m_controllers[i]->Update();
		}
	}
}
const Vector2 Input::getLeftAnalogPosition(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return Vector2::vec2_zeros;
	}
	return m_controllers[index]->getLeftAnalogPosition();
}
const Vector2 Input::getRightAnalogPosition(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return Vector2::vec2_zeros;
	}
	return m_controllers[index]->getRightAnalogPosition();
}
const float Input::getRightTriggerPosition(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return 0.f;
	}
	return m_controllers[index]->getRightTriggerPosition();
}
const float Input::getLeftTriggerPosition(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return 0.f;

	}
	return m_controllers[index]->getLeftTriggerPosition();
}
const bool Input::isLeftBumperDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isLeftBumperDown();
}
const bool Input::isRightBumperDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isRightBumperDown();
}
const bool Input::isDPadUpDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;

	}
	return m_controllers[index]->isDPadUpDown();
}
const bool Input::isDPadDownDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isDPadDownDown();
}
const bool Input::isDPadLeftDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isDPadLeftDown();
}
const bool Input::isDPadRightDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isDPadRightDown();
}
const bool Input::isAButtonDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isAButtonDown();
}
const bool Input::isBButtonDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isBButtonDown();
}
const bool Input::isXButtonDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isXButtonDown();
}
const bool Input::isYButtonDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isYButtonDown();
}
const bool Input::isStartButtonDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isStartButtonDown();
}
const bool Input::isBackButtonDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isBackButtonDown();
}
const bool Input::isLeftAnalogPressedDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isLeftAnalogPressedDown();
}
const bool Input::isRightAnalogPressedDown(const int& index) const
{
	if (isThisControllerReal(index) == false)
	{
		return false;
	}
	return m_controllers[index]->isRightAnalogPressedDown();
}

//Mice
void Input::ActivateDPIAwareness()
{
	if (m_mice == nullptr)
	{
		Mouse().activateDPIAwareness();
	}
	else
	{
		m_mice[0].activateDPIAwareness();
	}
}
const bool Input::IsThisMouseReal(const int& index) const
{
	if (m_mice == nullptr || index < 0 || index > m_maxNumOfMice)
		return false;

	return true;
}
void Input::setNumOfMice(const int& numOfMice)
{
	if (m_mice == nullptr)
	{
		return;
	}
	m_mice->setNumOfMice(numOfMice);
}
void Input::setWheelScroll(const bool& up, const bool& down)
{
	if (m_mice == nullptr)
	{
		return;
	}
	m_mice->setWheelScroll(up, down);
}
const bool Input::IsWheelScrollUp() const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->IsWheelScrollUp();
}
const bool Input::IsWheelScrollDown() const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->IsWheelScrollDown();
}
const bool Input::getMouseUp(const int& button) const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->getMouseUp(button);
}
const bool Input::getMouseDown(const int& button) const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->getMouseDown(button);
}
const bool Input::getMouseClick(const int& button) const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->getMouseClick(button);
}

const bool Input::LeftClickUp() const
{

	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->LeftClickUp();
}
const bool Input::LeftClickDown() const
{

	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->LeftClickDown();
}
const bool Input::LeftClick() const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->LeftClick();
}
const bool Input::RightClickUp() const
{

	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->RightClickUp();
}
const bool Input::RightClickDown() const
{

	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->RightClickDown();
}
const bool Input::RightClick() const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->RightClick();
}

const bool Input::MiddleClickDown() const
{

	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->MiddleClickDown();
}
const bool Input::MiddleClickUp() const
{

	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->MiddleClickUp();
}
const bool Input::MiddleClick() const
{

	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->MiddleClick();
}
const int Input::getNumOfMice() const
{
	if (m_mice == nullptr)
	{
		return 0;
	}
	return m_mice->getNumOfMice();
}
const bool Input::GetFirstCursorPosition(IntVector2& position) const
{
	if (m_mice == nullptr)
	{
		return false;
	}
	return m_mice->GetFirstCursorPosition(position);
}
const bool* Input::GetCursorPositions(IntVector2*& position)
{
	if (m_mice == nullptr)
	{
		if (m_maxNumOfMice > 0)
		{
			bool* booleans = new bool[m_maxNumOfMice];
			for (size_t i = 0; i < (size_t)m_maxNumOfMice; i++)
			{
				booleans[i] = false;
			}
			return booleans;
		}
		else
		{
			bool* booleans = new bool[1];
			booleans[0] = false;
			return booleans;
		}
	}
	return m_mice->GetCursorPositions(position);
}
void Input::setCursorPosition(const IntVector2& position, const int& index)
{
	if (IsThisMouseReal(index) == false)
	{
		return;
	}
	m_mice->setCursorPosition(position);
}
void Input::setCursorShow(const bool& show, const int& index)
{
	if (IsThisMouseReal(index) == false)
	{
		return;
	}
	m_mice->setCursorShow(show);
}