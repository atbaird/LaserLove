#include "Engine/Core/Mouse.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


Mouse::Mouse()
: m_numOfMice(0),
m_downWheelScroll(false),
m_upWheelScroll(false)
{

}
Mouse::Mouse(int numofMice)
: m_numOfMice(numofMice),
m_downWheelScroll(false),
m_upWheelScroll(false)
{
}
Mouse::~Mouse()
{
}


const bool Mouse::getMouseUp(const int& button) const
{
	button;
	return false;
}
const bool Mouse::getMouseDown(const int& button) const
{
	button;
	return false;
}
const bool Mouse::getMouseClick(const int& button) const
{
	if (GetAsyncKeyState(button))
	{
		return true;
	}
	return false;
}
const bool Mouse::LeftClickUp() const
{
	return getMouseUp(1);
}
const bool Mouse::LeftClickDown() const
{
	return getMouseUp(1);
}
const bool Mouse::LeftClick() const
{
	return getMouseClick(1);
}
const bool Mouse::RightClickUp() const
{
	return getMouseUp(2);
}
const bool Mouse::RightClickDown() const
{
	return getMouseDown(2);
}
const bool Mouse::RightClick() const
{
	return getMouseClick(2);
}
const bool Mouse::MiddleClickUp() const
{
	return getMouseUp(3);
}
const bool Mouse::MiddleClickDown() const
{
	return getMouseDown(3);
}
const bool Mouse::MiddleClick() const
{
	return getMouseClick(3);
}
void Mouse::activateDPIAwareness()
{
	SetProcessDPIAware();
}
void Mouse::setNumOfMice(int numOfMice)
{
	m_numOfMice = numOfMice;
}
const int Mouse::getNumOfMice() const
{
	return m_numOfMice;
}

void Mouse::setWheelScroll(const bool& up, const bool& down)
{
	m_upWheelScroll = up;
	m_downWheelScroll = down;
}
const bool Mouse::IsWheelScrollUp() const
{
	return m_upWheelScroll;
}
const bool Mouse::IsWheelScrollDown() const
{
	return m_downWheelScroll;
}

const bool Mouse::GetFirstCursorPosition(IntVector2& position) const
{
	POINT pos[1];
	BOOL success = GetCursorPos(&pos[0]);
	if (success == false)
		return false;

	position.x = pos[0].x;
	position.y = pos[0].y;
	return true;
}
const bool* Mouse::GetCursorPositions(IntVector2*& positions) // don't forget to delete the bool* after your done.
{
	size_t numToIterate = (size_t)m_numOfMice;
	POINT* pos = new POINT[numToIterate];
	bool* bools = new bool[numToIterate];
	for (int i = 0; i < m_numOfMice; i++)
	{
		BOOL success = GetCursorPos(&pos[i]);
		if (success == false)
		{
			positions[i].x = 0;
			positions[i].y = 0;
		}
		else
		{
			positions[i].x = pos[i].x;
			positions[i].y = pos[i].y;
		}
	}
	delete pos;
	return bools;
}
void Mouse::setCursorPosition(const IntVector2& position)
{
	SetCursorPos(position.x, position.y);
}

void Mouse::setCursorShow(const bool& show)
{
	ShowCursor(show);
}