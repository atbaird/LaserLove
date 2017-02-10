#pragma once

#ifndef SPRITELOADER_HPP
#define SPRITELOADER_HPP
#include "Game/SpriteLoader/SpriteResourceList.hpp"

class SpriteLoader
{
public:
	static void LoadAllSpritesAsNew();
	static const SpriteResource* LoadInSprite(Texture* tex, const size_t& spriteID, const AABB2& spriteCoords, const Vector2& virtualSize);
};
#endif