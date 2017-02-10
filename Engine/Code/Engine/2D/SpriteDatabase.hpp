#pragma once

#ifndef SPRITEDATABASE_HPP
#define SPRITEDATABASE_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"
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

struct SpriteAnimationSorter
{
	//variables
	size_t m_spriteAnimationID;
	std::string m_spriteAnimationTag;
	

	//operators
	const bool operator==(const SpriteAnimationSorter& other) const;
	const bool operator<(const SpriteAnimationSorter& other) const;
};

class SpriteDatabase
{
private:
	static size_t availableSpriteResourceID;
	static size_t availableSpriteAnimationID;
	std::vector<SpriteResourceSorter> m_resourceIDs;
	std::vector<SpriteResource*> m_spriteResources;
	std::vector<SpriteAnimationSorter> m_animationIDs;
	std::vector<SpriteAnimationSequence*> m_spriteAnimations;
public:
	static const std::string s_CommonSpriteDatabaseSaveLocation;
	static SpriteDatabase* g_SpriteDatabase;

	//Constructors
	SpriteDatabase();
	~SpriteDatabase();

	//Register
	const bool RegisterNewSprite(Texture* spriteSheet, const size_t& spriteID, SpriteResource* resource);
	const bool RegisterNewAnimation(SpriteAnimationSequence* sequence, std::string& spriteTag);
	void ClearDatabase();

	//Getters
	SpriteResource* EditSprite(const size_t& spriteResourceID);
	const SpriteResource* GetSprite(const size_t& spriteResourceID);
	const size_t GetSpriteResourceIdxForSpriteSheetAndSpriteID(Texture* spriteSheet, const size_t& spriteID, bool& foundIt);
	const size_t GetSpriteResourceIDForSpriteSheetAndSpriteID(Texture* spriteSheet, const size_t& spriteID, bool& foundIt);
	const size_t GetSpriteAnimationIdxForTag(std::string& tag, bool& foundIt);
	const bool FindIfSpriteSheetAndIDAlreadyExists(Texture* spriteSheet, const size_t& spriteID);


	void SaveOutAllSpriteResourcesToXML(const std::string& xmlFileLocation = s_CommonSpriteDatabaseSaveLocation);
	const bool TryLoadAllSpriteResourcesFromXML(const std::string xmlFileLocation);
};
#endif