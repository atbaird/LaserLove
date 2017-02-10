#include "Engine/Core/XboxController.hpp"
#include <cmath>

//constructors
XboxController::XboxController()
{
	whatControllerAmI = 0;
}
XboxController::XboxController(int controllerNumber)
{
	whatControllerAmI = controllerNumber;
}

//plugged in
const DWORD XboxController::amIPluggedInDWORD()
{
	//method returns DWORD, aka the error status of the xboxcontroller.
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(whatControllerAmI, &xboxControllerState);
	return errorStatus;
}
const int XboxController::amIPluggedIn()
{
	//This method is suppose to return 0 if the controller is plugged in and working
	//otherwise it will return 1 if it is not plugged in, or -1 if there is some sort of error with the device.
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(whatControllerAmI, &xboxControllerState);
	if (errorStatus == ERROR_SUCCESS)
	{
		return 0;
	}
	else if (errorStatus == ERROR_DEVICE_NOT_CONNECTED)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

//update
void XboxController::Update()
{
	//suppose to update the status of the controller.
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(whatControllerAmI, &xboxControllerState);
	errorStatus;
}

//getters
const Vector2 XboxController::getLeftAnalogPosition() const
{
	//gets and determines the left analog's position.
	float lTriggerLeftOrRight = xboxControllerState.Gamepad.sThumbLX;
	float lTriggerUpOrDown = xboxControllerState.Gamepad.sThumbLY;
	float start = 5000.f;
	float end = 32768.f;
	float xNegOrNot = 0.f;
	if (lTriggerLeftOrRight != 0.f)
	{
		xNegOrNot = lTriggerLeftOrRight / abs(lTriggerLeftOrRight);
	}
	float yNegOrNot = 0.f;
	if (lTriggerUpOrDown != 0.f)
	{
		yNegOrNot = lTriggerUpOrDown / abs(lTriggerUpOrDown);
	}
	lTriggerUpOrDown = (abs(lTriggerUpOrDown) - start);
	if (lTriggerUpOrDown < 0.f)
	{
		lTriggerUpOrDown = 0.f;
	}
	lTriggerLeftOrRight = (abs(lTriggerLeftOrRight) - start);
	if (lTriggerLeftOrRight < 0.f)
	{
		lTriggerLeftOrRight = 0.f;
	}
	lTriggerUpOrDown = ((lTriggerUpOrDown / (end - start))) * yNegOrNot;
	lTriggerLeftOrRight = ((lTriggerLeftOrRight / (end - start))) * xNegOrNot;
	return Vector2(lTriggerLeftOrRight, lTriggerUpOrDown);
}

const Vector2 XboxController::getRightAnalogPosition() const
{
	//gets and determines the Right Analog's position.
	float rTriggerLeftOrRight = xboxControllerState.Gamepad.sThumbRX;
	float rTriggerUpOrDown = xboxControllerState.Gamepad.sThumbRY;
	float start = 5000.f;
	float end = 32768;
	float xNegOrNot = 0.f;
	if (rTriggerLeftOrRight != 0.f)
	{
		xNegOrNot = rTriggerLeftOrRight / abs(rTriggerLeftOrRight);
	}
	float yNegOrNot = 0.f;
	if (rTriggerUpOrDown != 0.f)
	{
		yNegOrNot = rTriggerUpOrDown / abs(rTriggerUpOrDown);
	}
	rTriggerUpOrDown = (abs(rTriggerUpOrDown) - start);
	if (rTriggerUpOrDown < 0)
	{
		rTriggerUpOrDown = 0;
	}
	rTriggerLeftOrRight = (abs(rTriggerLeftOrRight) - start);
	if (rTriggerLeftOrRight < 0)
	{
		rTriggerLeftOrRight = 0;
	}
	rTriggerUpOrDown = ((rTriggerUpOrDown / (end - start))) * yNegOrNot;
	rTriggerLeftOrRight = ((rTriggerLeftOrRight / (end - start))) * xNegOrNot;
	return Vector2(rTriggerLeftOrRight, rTriggerUpOrDown);
}

const float XboxController::getRightTriggerPosition() const
{
	//returns the right trigger position as a value between 0 and 1.
	return (xboxControllerState.Gamepad.bRightTrigger) / 255.f;
}

const float XboxController::getLeftTriggerPosition() const
{
	//returns the left trigger position as a value between 0 and 1.
	return (xboxControllerState.Gamepad.bLeftTrigger) / 255.f;
}


const bool XboxController::isLeftBumperDown() const
{
	//returns whether or not the left bumper is down.
	int LeftBumper = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_LEFT_SHOULDER; 
	if (LeftBumper == 256)
	{
		return true;
	}
	return false;
}
const bool XboxController::isRightBumperDown() const
{
	//returns whether or not the right bumper is down.
	int RightBumper = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_RIGHT_SHOULDER;
	if (RightBumper == 512)
	{
		return true;
	}
	return false;
}

const bool XboxController::isDPadUpDown() const
{
	//returns whether or not the Dpad Up is pressed down or not.
	int dPadUp = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_DPAD_UP;
	if (dPadUp == 1)
	{
		return true;
	}
	return false;
}
const bool XboxController::isDPadDownDown() const
{
	//returns whether or not the DPad Down is pressed down or not.
	int dPadDown = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_DPAD_DOWN;
	if (dPadDown == 2)
	{
		return true;
	}
	return false;
}
const bool XboxController::isDPadLeftDown() const
{
	//returns whether or not the DPad Left is pressed down or not.
	int dPadLeft = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_DPAD_LEFT;
	if (dPadLeft == 4)
	{
		return true;
	}
	return false;
}
const bool XboxController::isDPadRightDown() const
{
	//returns whether or not the DPad Right is pressed down or not.
	int dPadRight = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_DPAD_RIGHT;
	if (dPadRight == 8)
	{
		return true;
	}
	return false;
}

const bool XboxController::isAButtonDown() const
{
	//determines whether the A button is down.
	int Abutton = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_A;
	if (Abutton == 4096)
	{
		return true;
	}
	return false;
}

const bool XboxController::isBButtonDown() const
{
	//determines whether the B button is down.
	int Bbutton = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_B;
	if (Bbutton == 8192)
	{
		return true;
	}
	return false;
}

const bool XboxController::isYButtonDown() const
{
	//determines whether the Y button is down.
	int Ybutton = (xboxControllerState.Gamepad.wButtons) & XINPUT_GAMEPAD_Y;
	if (Ybutton == 32768)
	{
		return true;
	}
	return false;
}

const bool XboxController::isXButtonDown() const
{
	//determines whether the X button is down.
	int Xbutton = (xboxControllerState.Gamepad.wButtons & (int)XINPUT_GAMEPAD_X);
	if (Xbutton == 16384)
	{
		return true;
	}
	return false;
}

const bool XboxController::isStartButtonDown() const
{
	//determines whether the start button is down.
	int startButton = (xboxControllerState.Gamepad.wButtons & (int)XINPUT_GAMEPAD_START);
	if (startButton == 16)
	{
		return true;
	}
	return false;
}
const bool XboxController::isBackButtonDown() const
{
	//determines whether the back button is down or not.
	int backButton = (xboxControllerState.Gamepad.wButtons & (int)XINPUT_GAMEPAD_BACK);
	if (backButton == 32)
	{
		return true;
	}
	return false;
}


const bool XboxController::isLeftAnalogPressedDown() const
{
	//determines whether the Left Analog stick is pressed down or not.
	int LeftAnalog = (xboxControllerState.Gamepad.wButtons & (int)XINPUT_GAMEPAD_LEFT_THUMB);
	if (LeftAnalog == 64)
	{
		return true;
	}
	return false;
}
const bool XboxController::isRightAnalogPressedDown() const
{
	//determines whether the Right Analog stick is pressed down or not.
	int RightAnalog = (xboxControllerState.Gamepad.wButtons & (int)XINPUT_GAMEPAD_RIGHT_THUMB);
	if (RightAnalog == 128)
	{
		return true;
	}
	return false;
}