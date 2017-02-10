#pragma once
#include "Engine/Core/KeyboardKeys.hpp"

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

//-----------------------------------------------------
const int NUM_KEYS = 256;

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	void setKeyDownStatus(const unsigned char&, const bool&);
	const bool isKeyDown(const unsigned char&) const;

private:
	bool m_isKeyDown[NUM_KEYS];
};
#endif // !KEYBOARD_HPP
