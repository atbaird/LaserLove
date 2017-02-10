#pragma once

#ifndef SPRITEDATABASE_HPP
#define SPRITEDATABASE_HPP
#include "Engine/2D/SpriteResource.hpp"
#include <vector>

struct SpriteResourceSorter
{
	Texture* tex;
	size_t spriteID;
	size_t spriteResourceID;

	const bool operator==(const SpriteResourceSorter& other) const;
	const bool operator<(const SpriteResourceSorter& other) const;
};
struct SpriteResourceIDTransfer
{
	size_t originalSpriteResourceID;
	size_t newSpriteResourceID;
};

class SpriteDatabase
{
private:
	static size_t availableSpriteResourceID;
	std::vector<SpriteResourceSorter> m_resourceIDs;
	std::vector<SpriteResource*> m_spriteResources;
public:
	static const std::string s_CommonSpriteDatabaseSaveLocation;
	static SpriteDatabase* g_SpriteDatabase;

	//Constructors
	SpriteDatabase();
	~SpriteDatabase();

	//Register
	const bool RegisterNewSprite(Texture* spriteSheet, const size_t& spriteID, SpriteResource* resource);
	void ClearDatabase();

	//Getters
	SpriteResource* EditSprite(const size_t& spriteResourceID);
	const SpriteResource* GetSprite(const size_t& spriteResourceID);
	const size_t GetSpriteResourceIdxForSpriteSheetAndSpriteID(Texture* spriteSheet, const size_t& spriteID, bool& foundIt);
	const size_t GetSpriteResourceIDForSpriteSheetAndSpriteID(Texture* spriteSheet, const size_t& spriteID, bool& foundIt);


	void SaveOutAllSpriteResourcesToXML(const std::string& xmlFileLocation = s_CommonSpriteDatabaseSaveLocation);
	const bool TryLoadAllSpriteResourcesFromXML(const std::string xmlFileLocation);
};
#endif