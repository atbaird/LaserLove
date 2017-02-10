#pragma once
#include "Engine/Math/IntVector2.hpp"

#ifndef MOUSE_HPP
#define MOUSE_HPP

class Mouse
{
public:
	Mouse();
	Mouse(int numofMice);
	~Mouse();

	void setNumOfMice(int numOfMice);
	void activateDPIAwareness();
	const int getNumOfMice() const;

	void setWheelScroll(const bool& up, const bool& down);
	const bool IsWheelScrollUp() const;
	const bool IsWheelScrollDown() const;

	const bool getMouseUp(const int& button) const;
	const bool getMouseDown(const int& button) const;
	const bool getMouseClick(const int& button) const;
	const bool LeftClickUp() const;
	const bool LeftClickDown() const;
	const bool LeftClick() const;
	const bool RightClick() const;
	const bool RightClickUp() const;
	const bool RightClickDown() const;
	const bool MiddleClick() const;
	const bool MiddleClickUp() const;
	const bool MiddleClickDown() const;
	const bool GetFirstCursorPosition(IntVector2& position) const;
	const bool* GetCursorPositions(IntVector2*& position);
	void setCursorPosition(const IntVector2& position);
	void setCursorShow(const bool& show = false);
private:
	int m_numOfMice;
	bool m_upWheelScroll;
	bool m_downWheelScroll;
};
#endif // !MOUSE_HPP
