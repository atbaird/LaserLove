#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h> // include the Xinput API

#include "Engine/Math/Vector2.hpp"
#pragma comment( lib, "xinput9_1_0" ) // Link in the xinput.lib static library #Eiserloh: Explicitly use XInput v9.1.0, since v1.4 is not supported under Windows 7
typedef unsigned long DWORD;

//XboxController base inputs returns:
//Analog Sticks return numbers between -32768 and +32768
//triggers return numbers between 0 and 255

//what this class will return instead for analog sticks and triggers:
//-1 to 1 for analog, 0 to 1 for triggers.

class XboxController
{
public:
	//constructors
	XboxController();
	XboxController(int controllerNumber);

	//plugged in
	const int amIPluggedIn();
	const unsigned long amIPluggedInDWORD(); //DWORD = unsigned long.

	//updates
	void Update();


	//getters
	const Vector2 getLeftAnalogPosition() const;
	const Vector2 getRightAnalogPosition() const;
	const float getRightTriggerPosition() const;
	const float getLeftTriggerPosition() const;
	const bool isLeftBumperDown() const;
	const bool isRightBumperDown() const;

	const bool isDPadUpDown() const;
	const bool isDPadDownDown() const;
	const bool isDPadLeftDown() const;
	const bool isDPadRightDown() const;

	const bool isAButtonDown() const;
	const bool isBButtonDown() const;
	const bool isXButtonDown() const;
	const bool isYButtonDown() const;
	const bool isStartButtonDown() const;
	const bool isBackButtonDown() const;
	const bool isLeftAnalogPressedDown() const;
	const bool isRightAnalogPressedDown() const;

	//variables
private:
	int whatControllerAmI;
	XINPUT_STATE xboxControllerState;
};