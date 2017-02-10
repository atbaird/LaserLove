#pragma once
#include "Engine/Core/Mouse.hpp"
#include "Engine/Core/XboxController.hpp"
#include "Engine/Core/Keyboard.hpp"


#ifndef INPUT_HPP
#define INPUT_HPP

//-----------------------------------------------------
class Input;

//-----------------------------------------------------
extern Input* g_input;

class Input
{
public:
	//Constructors
	Input(const int& numControllers = 0, const int& numOfMice = 1);
	~Input();

	//Keyboard
	void SetKeyDownStatus(const unsigned char&, const bool&);
	const bool IsKeyDown(const unsigned char&) const;

	//XboxControllers
	const bool isThisControllerReal(const int& index) const;
	const int amIPluggedIn(const int& index) const;
	const unsigned long amIPluggedInDWORD(const int& index) const;
	void Update();
	const Vector2 getLeftAnalogPosition(const int& index) const;
	const Vector2 getRightAnalogPosition(const int& index) const;
	const float getRightTriggerPosition(const int& index) const;
	const float getLeftTriggerPosition(const int& index) const;
	const bool isLeftBumperDown(const int& index) const;
	const bool isRightBumperDown(const int& index) const;
	const bool isDPadUpDown(const int& index) const;
	const bool isDPadDownDown(const int& index) const;
	const bool isDPadLeftDown(const int& index) const;
	const bool isDPadRightDown(const int& index) const;
	const bool isAButtonDown(const int& index) const;
	const bool isBButtonDown(const int& index) const;
	const bool isXButtonDown(const int& index) const;
	const bool isYButtonDown(const int& index) const;
	const bool isStartButtonDown(const int& index) const;
	const bool isBackButtonDown(const int& index) const;
	const bool isLeftAnalogPressedDown(const int& index) const;
	const bool isRightAnalogPressedDown(const int& index) const;

	//Mice
	void ActivateDPIAwareness();
	const bool IsThisMouseReal(const int& index) const;
	void setNumOfMice(const int& numOfMice);
	void setWheelScroll(const bool& up, const bool& down);
	const bool IsWheelScrollUp() const;
	const bool IsWheelScrollDown() const;
	const bool getMouseUp(const int& button) const;
	const bool getMouseDown(const int& button) const;
	const bool getMouseClick(const int& button) const;
	const bool LeftClick() const;
	const bool LeftClickUp() const;
	const bool LeftClickDown() const;
	const bool RightClickUp() const;
	const bool RightClickDown() const;
	const bool RightClick() const;
	const bool MiddleClickDown() const;
	const bool MiddleClickUp() const;
	const bool MiddleClick() const;
	const int getNumOfMice() const;
	const bool GetFirstCursorPosition(IntVector2& position) const;
	const bool* GetCursorPositions(IntVector2*& position);
	void setCursorPosition(const IntVector2& position, const int& index = 0);
	void setCursorShow(const bool& show = false, const int& index = 0);

	int m_maxNumControllers;
	int m_maxNumOfMice;
	Keyboard* m_keyboard = nullptr;
	XboxController** m_controllers = nullptr;
	Mouse* m_mice = nullptr;
};
#endif // !INPUT_HPP
