#pragma once
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/AABB2.hpp"

#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

class SpriteSheet
{
private:
	Texture* 	m_spriteSheetTexture;	// Image with grid-based layout of sub-images
	IntVector2	m_spriteLayout;		// # of sprites across, and down, on the sheet
									//... and other data member variables as needed
public:
	//Constructors
	SpriteSheet();
	SpriteSheet(const std::string& imageFilePath, const int& tilesWide, const int& tilesHigh);

	//Getters
	const AABB2 GetTexCoordsForSpriteCoords(const int& spriteX, const int& spriteY) const; // mostly for atlases
	const AABB2 GetTexCoordsForSpriteCoords(IntVector2 sprite) const; // mostly for atlases
	const AABB2 GetTexCoordsForSpriteIndex(const int& spriteIndex) const; // mostly for sprite animations
	Texture* getSpriteSheetTexture() const;
	const int GetSpriteIndexForSpriteCoords(IntVector2 sprite) const;
	const int GetNumSprites() const;

	//Change
	void changeSpriteSheet(const std::string& imageFilePath, const int& tilesWide, const int& tilesHigh);
	//... and other member functions as needed


};

#endif // !SPRITESHEET_HPP
