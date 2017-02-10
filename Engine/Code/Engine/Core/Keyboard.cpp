#include "Engine/Core/Keyboard.hpp"


Keyboard::Keyboard()
{
	//Set all keys to "not down"
	for (int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex) {
		m_isKeyDown[keyIndex] = false;
	}
}
Keyboard::~Keyboard()
{
}

void Keyboard::setKeyDownStatus(const unsigned char& asKey, const bool& isNowDown)
{
	m_isKeyDown[asKey] = isNowDown;
}
const bool Keyboard::isKeyDown(const unsigned char& asKey) const
{
	return m_isKeyDown[asKey];
}
