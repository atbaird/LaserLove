#pragma once

#ifndef PLAYERSHIPSELECTIMAGEHANDLER_HPP
#define PLAYERSHIPSELECTIMAGEHANDLER_HPP
#include "Engine/2D/Sprite.hpp"

class PlayerShipSelectImageHandler
{
private:
	bool m_visible;
	std::vector<Sprite*> m_sprites;
	std::vector<Sprite*> m_instructions;
	size_t m_numOfPlayerShips;
protected:
public:
	//Constructors
	PlayerShipSelectImageHandler(const SpriteResource* resourceForOutline = nullptr);
	virtual ~PlayerShipSelectImageHandler();

	//Setters
	void SetVisible(bool visible);
	
	//Getters
	const bool GetVisible();

	//Updates
	void Update(bool selected, size_t selectedIdx);
};
#endif